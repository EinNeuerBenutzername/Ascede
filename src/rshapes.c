/**********************************************************************************************
*
*   ALTERED
*
*   rshapes - Basic functions to draw 2d shapes and check collisions
*
*   CONFIGURATION:
*
*   #define SUPPORT_QUADS_DRAW_MODE
*       Use QUADS instead of TRIANGLES for drawing when possible.
*       Some lines-based shapes could still use lines
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2013-2021 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "ascede.h"     // Declares module functions

// Check if config flags have been externally provided on compilation line
#if !defined(EXTERNAL_CONFIG_FLAGS)
    #include "config.h"         // Defines module configuration flags
#endif

#include "rlgl.h"       // OpenGL abstraction layer to OpenGL 1.1, 2.1, 3.3+ or ES2

#include <math.h>       // Required for: sinf(), asinf(), cosf(), acosf(), sqrtf(), fabsf()
#include <float.h>      // Required for: FLT_EPSILON

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------

// Error rate to calculate how many segments we need to draw a smooth circle,
// taken from https://stackoverflow.com/a/2244088
#ifndef SMOOTH_CIRCLE_ERROR_RATE
    #define SMOOTH_CIRCLE_ERROR_RATE  0.5f
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Not here...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
Texture2D texShapes = { 1, 1, 1, 1, 7 };        // Texture used on shapes drawing (usually a white pixel)
Rectangle texShapesRec = { 0, 0, 1, 1 };        // Texture source rectangle used on shapes drawing

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
static float EaseCubicInOut(float t, float b, float c, float d);    // Cubic easing

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
void Shape_SetTexture(Texture2D texture, Rectangle source)
{
    texShapes = texture;
    texShapesRec = source;
}

// Draw a line
void Shape_DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color)
{
    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2i(startPosX, startPosY);
        rlVertex2i(endPosX, endPosY);
    rlEnd();
}

// Draw a line  (Vector version)
void Shape_DrawLineV(Vector2 startPos, Vector2 endPos, Color color)
{
    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(startPos.x, startPos.y);
        rlVertex2f(endPos.x, endPos.y);
    rlEnd();
}

// Draw a line defining thickness
void Shape_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color)
{
    Vector2 delta = { endPos.x - startPos.x, endPos.y - startPos.y };
    float length = sqrtf(delta.x*delta.x + delta.y*delta.y);

    if ((length > 0) && (thick > 0))
    {
        float scale = thick/(2*length);
        Vector2 radius = { -scale*delta.y, scale*delta.x };
        Vector2 strip[4] = {
            { startPos.x - radius.x, startPos.y - radius.y },
            { startPos.x + radius.x, startPos.y + radius.y },
            { endPos.x - radius.x, endPos.y - radius.y },
            { endPos.x + radius.x, endPos.y + radius.y }
        };

        Shape_DrawTriangleStrip(strip, 4, color);
    }
}

// Draw a color-filled rectangle
void Shape_DrawRec(int posX, int posY, int width, int height, Color color)
{
    Shape_DrawRecV((Vector2){ (float)posX, (float)posY }, (Vector2){ (float)width, (float)height }, color);
}

// Draw a color-filled rectangle (Vector version)
// NOTE: On OpenGL 3.3 and ES2 we use QUADS to avoid drawing order issues
void Shape_DrawRecV(Vector2 position, Vector2 size, Color color)
{
    Shape_DrawRecPro((Rectangle){ position.x, position.y, size.x, size.y }, (Vector2){ 0.0f, 0.0f }, 0.0f, color);
}

// Draw a color-filled rectangle
void Shape_DrawRecRec(Rectangle rec, Color color)
{
    Shape_DrawRecPro(rec, (Vector2){ 0.0f, 0.0f }, 0.0f, color);
}

// Draw a color-filled rectangle with pro parameters
void Shape_DrawRecPro(Rectangle rec, Vector2 origin, float rotation, Color color)
{
    rlCheckRenderBatchLimit(4);

    Vector2 topLeft = { 0 };
    Vector2 topRight = { 0 };
    Vector2 bottomLeft = { 0 };
    Vector2 bottomRight = { 0 };

    // Only calculate rotation if needed
    if (rotation==0.0f)
    {
        float x = rec.x - origin.x;
        float y = rec.y - origin.y;
        topLeft = (Vector2){ x, y };
        topRight = (Vector2){ x + rec.width, y };
        bottomLeft = (Vector2){ x, y + rec.height };
        bottomRight = (Vector2){ x + rec.width, y + rec.height };
    }
    else
    {
        float sinRotation = sinf(rotation*DEG2RAD);
        float cosRotation = cosf(rotation*DEG2RAD);
        float x = rec.x;
        float y = rec.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx*cosRotation - dy*sinRotation;
        topLeft.y = y + dx*sinRotation + dy*cosRotation;

        topRight.x = x + (dx + rec.width)*cosRotation - dy*sinRotation;
        topRight.y = y + (dx + rec.width)*sinRotation + dy*cosRotation;

        bottomLeft.x = x + dx*cosRotation - (dy + rec.height)*sinRotation;
        bottomLeft.y = y + dx*sinRotation + (dy + rec.height)*cosRotation;

        bottomRight.x = x + (dx + rec.width)*cosRotation - (dy + rec.height)*sinRotation;
        bottomRight.y = y + (dx + rec.width)*sinRotation + (dy + rec.height)*cosRotation;
    }

    rlSetTexture(texShapes.id);
    rlBegin(RL_QUADS);

        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
        rlVertex2f(topLeft.x, topLeft.y);

        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
        rlVertex2f(bottomLeft.x, bottomLeft.y);

        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
        rlVertex2f(bottomRight.x, bottomRight.y);

        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
        rlVertex2f(topRight.x, topRight.y);

    rlEnd();
    rlSetTexture(0);
}

//// Draw a vertical-gradient-filled rectangle
//// NOTE: Gradient goes from bottom (color1) to top (color2)
//void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2)
//{
//    DrawRectangleGradientEx((Rectangle){ (float)posX, (float)posY, (float)width, (float)height }, color1, color2, color2, color1);
//}
//
//// Draw a horizontal-gradient-filled rectangle
//// NOTE: Gradient goes from bottom (color1) to top (color2)
//void DrawRectangleGradientH(int posX, int posY, int width, int height, Color color1, Color color2)
//{
//    DrawRectangleGradientEx((Rectangle){ (float)posX, (float)posY, (float)width, (float)height }, color1, color1, color2, color2);
//}
//
//// Draw a gradient-filled rectangle
//// NOTE: Colors refer to corners, starting at top-lef corner and counter-clockwise
//void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4)
//{
//    rlSetTexture(texShapes.id);
//
//    rlPushMatrix();
//        rlBegin(RL_QUADS);
//            rlNormal3f(0.0f, 0.0f, 1.0f);
//
//            // NOTE: Default raylib font character 95 is a white square
//            rlColor4ub(col1.r, col1.g, col1.b, col1.a);
//            rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//            rlVertex2f(rec.x, rec.y);
//
//            rlColor4ub(col2.r, col2.g, col2.b, col2.a);
//            rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//            rlVertex2f(rec.x, rec.y + rec.height);
//
//            rlColor4ub(col3.r, col3.g, col3.b, col3.a);
//            rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//            rlVertex2f(rec.x + rec.width, rec.y + rec.height);
//
//            rlColor4ub(col4.r, col4.g, col4.b, col4.a);
//            rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//            rlVertex2f(rec.x + rec.width, rec.y);
//        rlEnd();
//    rlPopMatrix();
//
//    rlSetTexture(0);
//}
//
//// Draw rectangle with rounded edges
//void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color)
//{
//    // Not a rounded rectangle
//    if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1 ))
//    {
//        Shape_DrawRecRec(rec, color);
//        return;
//    }
//
//    if (roundness >= 1.0f) roundness = 1.0f;
//
//    // Calculate corner radius
//    float radius = (rec.width > rec.height)? (rec.height*roundness)/2 : (rec.width*roundness)/2;
//    if (radius <= 0.0f) return;
//
//    // Calculate number of segments to use for the corners
//    if (segments < 4)
//    {
//        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
//        float th = acosf(2*powf(1 - SMOOTH_CIRCLE_ERROR_RATE/radius, 2) - 1);
//        segments = (int)(ceilf(2*PI/th)/4.0f);
//        if (segments <= 0) segments = 4;
//    }
//
//    float stepLength = 90.0f/(float)segments;
//
//    /*
//    Quick sketch to make sense of all of this,
//    there are 9 parts to draw, also mark the 12 points we'll use
//
//          P0____________________P1
//          /|                    |\
//         /1|          2         |3\
//     P7 /__|____________________|__\ P2
//       |   |P8                P9|   |
//       | 8 |          9         | 4 |
//       | __|____________________|__ |
//     P6 \  |P11              P10|  / P3
//         \7|          6         |5/
//          \|____________________|/
//          P5                    P4
//    */
//    // Coordinates of the 12 points that define the rounded rect
//    const Vector2 point[12] = {
//        {(float)rec.x + radius, rec.y}, {(float)(rec.x + rec.width) - radius, rec.y}, { rec.x + rec.width, (float)rec.y + radius },     // PO, P1, P2
//        {rec.x + rec.width, (float)(rec.y + rec.height) - radius}, {(float)(rec.x + rec.width) - radius, rec.y + rec.height},           // P3, P4
//        {(float)rec.x + radius, rec.y + rec.height}, { rec.x, (float)(rec.y + rec.height) - radius}, {rec.x, (float)rec.y + radius},    // P5, P6, P7
//        {(float)rec.x + radius, (float)rec.y + radius}, {(float)(rec.x + rec.width) - radius, (float)rec.y + radius},                   // P8, P9
//        {(float)(rec.x + rec.width) - radius, (float)(rec.y + rec.height) - radius}, {(float)rec.x + radius, (float)(rec.y + rec.height) - radius} // P10, P11
//    };
//
//    const Vector2 centers[4] = { point[8], point[9], point[10], point[11] };
//    const float angles[4] = { 180.0f, 90.0f, 0.0f, 270.0f };
//
//#if defined(SUPPORT_QUADS_DRAW_MODE)
//    rlCheckRenderBatchLimit(16*segments/2 + 5*4);
//
//    rlSetTexture(texShapes.id);
//
//    rlBegin(RL_QUADS);
//        // Draw all of the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
//        for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
//        {
//            float angle = angles[k];
//            const Vector2 center = centers[k];
//
//            // NOTE: Every QUAD actually represents two segments
//            for (int i = 0; i < segments/2; i++)
//            {
//                rlColor4ub(color.r, color.g, color.b, color.a);
//                rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//                rlVertex2f(center.x, center.y);
//                rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//                rlVertex2f(center.x + sinf(DEG2RAD*angle)*radius, center.y + cosf(DEG2RAD*angle)*radius);
//                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//                rlVertex2f(center.x + sinf(DEG2RAD*(angle + stepLength))*radius, center.y + cosf(DEG2RAD*(angle + stepLength))*radius);
//                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//                rlVertex2f(center.x + sinf(DEG2RAD*(angle + stepLength*2))*radius, center.y + cosf(DEG2RAD*(angle + stepLength*2))*radius);
//                angle += (stepLength*2);
//            }
//
//            // NOTE: In case number of segments is odd, we add one last piece to the cake
//            if (segments%2)
//            {
//                rlColor4ub(color.r, color.g, color.b, color.a);
//                rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//                rlVertex2f(center.x, center.y);
//                rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//                rlVertex2f(center.x + sinf(DEG2RAD*angle)*radius, center.y + cosf(DEG2RAD*angle)*radius);
//                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//                rlVertex2f(center.x + sinf(DEG2RAD*(angle + stepLength))*radius, center.y + cosf(DEG2RAD*(angle + stepLength))*radius);
//                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//                rlVertex2f(center.x, center.y);
//            }
//        }
//
//        // [2] Upper Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[0].x, point[0].y);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[8].x, point[8].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[9].x, point[9].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[1].x, point[1].y);
//
//        // [4] Right Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[2].x, point[2].y);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[9].x, point[9].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[10].x, point[10].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[3].x, point[3].y);
//
//        // [6] Bottom Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[11].x, point[11].y);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[5].x, point[5].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[4].x, point[4].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[10].x, point[10].y);
//
//        // [8] Left Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[7].x, point[7].y);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[6].x, point[6].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[11].x, point[11].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[8].x, point[8].y);
//
//        // [9] Middle Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[8].x, point[8].y);
//        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[11].x, point[11].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
//        rlVertex2f(point[10].x, point[10].y);
//        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
//        rlVertex2f(point[9].x, point[9].y);
//
//    rlEnd();
//    rlSetTexture(0);
//#else
//    rlCheckRenderBatchLimit(12*segments + 5*6); // 4 corners with 3 vertices per segment + 5 rectangles with 6 vertices each
//
//    rlBegin(RL_TRIANGLES);
//
//        // Draw all of the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
//        for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
//        {
//            float angle = angles[k];
//            const Vector2 center = centers[k];
//            for (int i = 0; i < segments; i++)
//            {
//                rlColor4ub(color.r, color.g, color.b, color.a);
//                rlVertex2f(center.x, center.y);
//                rlVertex2f(center.x + sinf(DEG2RAD*angle)*radius, center.y + cosf(DEG2RAD*angle)*radius);
//                rlVertex2f(center.x + sinf(DEG2RAD*(angle + stepLength))*radius, center.y + cosf(DEG2RAD*(angle + stepLength))*radius);
//                angle += stepLength;
//            }
//        }
//
//        // [2] Upper Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlVertex2f(point[0].x, point[0].y);
//        rlVertex2f(point[8].x, point[8].y);
//        rlVertex2f(point[9].x, point[9].y);
//        rlVertex2f(point[1].x, point[1].y);
//        rlVertex2f(point[0].x, point[0].y);
//        rlVertex2f(point[9].x, point[9].y);
//
//        // [4] Right Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlVertex2f(point[9].x, point[9].y);
//        rlVertex2f(point[10].x, point[10].y);
//        rlVertex2f(point[3].x, point[3].y);
//        rlVertex2f(point[2].x, point[2].y);
//        rlVertex2f(point[9].x, point[9].y);
//        rlVertex2f(point[3].x, point[3].y);
//
//        // [6] Bottom Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlVertex2f(point[11].x, point[11].y);
//        rlVertex2f(point[5].x, point[5].y);
//        rlVertex2f(point[4].x, point[4].y);
//        rlVertex2f(point[10].x, point[10].y);
//        rlVertex2f(point[11].x, point[11].y);
//        rlVertex2f(point[4].x, point[4].y);
//
//        // [8] Left Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlVertex2f(point[7].x, point[7].y);
//        rlVertex2f(point[6].x, point[6].y);
//        rlVertex2f(point[11].x, point[11].y);
//        rlVertex2f(point[8].x, point[8].y);
//        rlVertex2f(point[7].x, point[7].y);
//        rlVertex2f(point[11].x, point[11].y);
//
//        // [9] Middle Rectangle
//        rlColor4ub(color.r, color.g, color.b, color.a);
//        rlVertex2f(point[8].x, point[8].y);
//        rlVertex2f(point[11].x, point[11].y);
//        rlVertex2f(point[10].x, point[10].y);
//        rlVertex2f(point[9].x, point[9].y);
//        rlVertex2f(point[8].x, point[8].y);
//        rlVertex2f(point[10].x, point[10].y);
//    rlEnd();
//#endif
//}

// Draw a triangle
// NOTE: Vertex must be provided in counter-clockwise order
void Shape_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
{
    rlCheckRenderBatchLimit(4);

#if defined(SUPPORT_QUADS_DRAW_MODE)
    rlSetTexture(texShapes.id);

    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
        rlVertex2f(v1.x, v1.y);

        rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
        rlVertex2f(v2.x, v2.y);

        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
        rlVertex2f(v2.x, v2.y);

        rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
        rlVertex2f(v3.x, v3.y);
    rlEnd();

    rlSetTexture(0);
#else
    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(v1.x, v1.y);
        rlVertex2f(v2.x, v2.y);
        rlVertex2f(v3.x, v3.y);
    rlEnd();
#endif
}

// Draw a triangle strip defined by points
// NOTE: Every new vertex connects with previous two
void Shape_DrawTriangleStrip(Vector2 *points, int pointCount, Color color)
{
    if (pointCount >= 3)
    {
        rlCheckRenderBatchLimit(3*(pointCount - 2));

        rlBegin(RL_TRIANGLES);
            rlColor4ub(color.r, color.g, color.b, color.a);

            for (int i = 2; i < pointCount; i++)
            {
                if ((i%2) == 0)
                {
                    rlVertex2f(points[i].x, points[i].y);
                    rlVertex2f(points[i - 2].x, points[i - 2].y);
                    rlVertex2f(points[i - 1].x, points[i - 1].y);
                }
                else
                {
                    rlVertex2f(points[i].x, points[i].y);
                    rlVertex2f(points[i - 1].x, points[i - 1].y);
                    rlVertex2f(points[i - 2].x, points[i - 2].y);
                }
            }
        rlEnd();
    }
}

// Draw a regular polygon of n sides (Vector version)
void Shape_DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)
{
    if (sides < 3) sides = 3;
    float centralAngle = 0.0f;

#if defined(SUPPORT_QUADS_DRAW_MODE)
    rlCheckRenderBatchLimit(4*sides); // Each side is a quad
#else
    rlCheckRenderBatchLimit(3*sides);
#endif

    rlPushMatrix();
        rlTranslatef(center.x, center.y, 0.0f);
        rlRotatef(rotation, 0.0f, 0.0f, 1.0f);

#if defined(SUPPORT_QUADS_DRAW_MODE)
        rlSetTexture(texShapes.id);

        rlBegin(RL_QUADS);
            for (int i = 0; i < sides; i++)
            {
                rlColor4ub(color.r, color.g, color.b, color.a);

                rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
                rlVertex2f(0, 0);

                rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
                rlVertex2f(sinf(DEG2RAD*centralAngle)*radius, cosf(DEG2RAD*centralAngle)*radius);

                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
                rlVertex2f(sinf(DEG2RAD*centralAngle)*radius, cosf(DEG2RAD*centralAngle)*radius);

                centralAngle += 360.0f/(float)sides;
                rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
                rlVertex2f(sinf(DEG2RAD*centralAngle)*radius, cosf(DEG2RAD*centralAngle)*radius);
            }
        rlEnd();
        rlSetTexture(0);
#else
        rlBegin(RL_TRIANGLES);
            for (int i = 0; i < sides; i++)
            {
                rlColor4ub(color.r, color.g, color.b, color.a);

                rlVertex2f(0, 0);
                rlVertex2f(sinf(DEG2RAD*centralAngle)*radius, cosf(DEG2RAD*centralAngle)*radius);

                centralAngle += 360.0f/(float)sides;
                rlVertex2f(sinf(DEG2RAD*centralAngle)*radius, cosf(DEG2RAD*centralAngle)*radius);
            }
        rlEnd();
#endif
    rlPopMatrix();
}
