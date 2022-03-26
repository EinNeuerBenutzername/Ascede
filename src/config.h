/**********************************************************************************************
*
*   ALTERED
*
*   ascede configuration flags
*
*   This file defines all the configuration flags for the different ascede modules
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2018-2021 Ahmad Fatoum & Ramon Santamaria (@raysan5)
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
#define PLATFORM_DESKTOP
//------------------------------------------------------------------------------------
// Module: core - Configuration Flags
//------------------------------------------------------------------------------------
#define SUPPORT_MOUSE_GESTURES      1
#define SUPPORT_SSH_KEYBOARD_RPI    1
//#define SUPPORT_WINMM_HIGHRES_TIMER 1
//#define SUPPORT_EVENTS_AUTOMATION     1
#define SUPPORT_MONITOR_CONTROLS    1
#define SUPPORT_GAMEPAD_CONTROLS    1

// core: Configuration values
//------------------------------------------------------------------------------------
#if defined(__linux__)
    #define MAX_FILEPATH_LENGTH     4096
#else
    #define MAX_FILEPATH_LENGTH      512
#endif

#define MAX_GAMEPADS                   4
#define MAX_GAMEPAD_AXIS               8
#define MAX_GAMEPAD_BUTTONS           32
#define MAX_TOUCH_POINTS               8
#define MAX_KEY_PRESSED_QUEUE         16

//------------------------------------------------------------------------------------
// Module: rlgl - Configuration values
//------------------------------------------------------------------------------------

// Enable OpenGL Debug Context (only available on OpenGL 4.3)
//#define RLGL_ENABLE_OPENGL_DEBUG_CONTEXT       1

// Show OpenGL extensions and capabilities detailed logs on init
//#define RLGL_SHOW_GL_DETAILS_INFO              1

#define RL_DEFAULT_BATCH_BUFFER_ELEMENTS    8192      // Default internal render batch elements limits
#define RL_DEFAULT_BATCH_BUFFERS               1      // Default number of batch buffers (multi-buffering)
#define RL_DEFAULT_BATCH_DRAWCALLS           256      // Default number of batch draw calls (by state changes: mode, texture)
#define RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS     4      // Maximum number of textures units that can be activated on batch drawing (Shader_SetValueTexture())

#define RL_MAX_MATRIX_STACK_SIZE              32      // Maximum size of internal Matrix stack

#define RL_MAX_SHADER_LOCATIONS               32      // Maximum number of shader locations supported

#define RL_CULL_DISTANCE_NEAR               0.01      // Default projection matrix near cull distance
#define RL_CULL_DISTANCE_FAR              1000.0      // Default projection matrix far cull distance

// Default shader vertex attribute names to set location points
// NOTE: When a new shader is loaded, the following locations are tried to be set for convenience
#define RL_DEFAULT_SHADER_ATTRIB_NAME_POSITION     "vertexPosition"    // Binded by default to shader location: 0
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD     "vertexTexCoord"    // Binded by default to shader location: 1
#define RL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL       "vertexNormal"      // Binded by default to shader location: 2
#define RL_DEFAULT_SHADER_ATTRIB_NAME_COLOR        "vertexColor"       // Binded by default to shader location: 3
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT      "vertexTangent"     // Binded by default to shader location: 4
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2    "vertexTexCoord2"   // Binded by default to shader location: 5

#define RL_DEFAULT_SHADER_UNIFORM_NAME_MVP         "mvp"               // model-view-projection matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_VIEW        "matView"           // view matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION  "matProjection"     // projection matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_MODEL       "matModel"          // model matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL      "matNormal"         // normal matrix (transpose(inverse(matModelView))
#define RL_DEFAULT_SHADER_UNIFORM_NAME_COLOR       "colDiffuse"        // color diffuse (base tint color, multiplied by texture color)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0  "texture0"          // texture0 (texture slot active 0)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1  "texture1"          // texture1 (texture slot active 1)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2  "texture2"          // texture2 (texture slot active 2)


//------------------------------------------------------------------------------------
// Module: shapes - Configuration Flags
//------------------------------------------------------------------------------------
// Use QUADS instead of TRIANGLES for drawing when possible
// Some lines-based shapes could still use lines
#define SUPPORT_QUADS_DRAW_MODE     1


//------------------------------------------------------------------------------------
// Module: textures - Configuration Flags
//------------------------------------------------------------------------------------
// Selecte desired fileformats to be supported for image data loading
#define SUPPORT_FILEFORMAT_PNG      1
#define SUPPORT_FILEFORMAT_BMP      1
#define SUPPORT_FILEFORMAT_TGA      1
#define SUPPORT_FILEFORMAT_JPG      1
//#define SUPPORT_FILEFORMAT_GIF      1
//#define SUPPORT_FILEFORMAT_PSD      1
//#define SUPPORT_FILEFORMAT_DDS      1
//#define SUPPORT_FILEFORMAT_HDR      1
//#define SUPPORT_FILEFORMAT_KTX      1
//#define SUPPORT_FILEFORMAT_ASTC     1
//#define SUPPORT_FILEFORMAT_PKM      1
//#define SUPPORT_FILEFORMAT_PVR      1

#define SUPPORT_IMAGE_EXPORT        1
// If not defined, still some functions are supported: Image_SetFormat(), Image_Crop(), Image_ToPOT()
#define SUPPORT_IMAGE_MANIPULATION  1
#define SUPPORT_IMAGE_DRAWING       1

//------------------------------------------------------------------------------------
// Module: text - Configuration Flags
//------------------------------------------------------------------------------------
// Default font is loaded on window initialization to be available for the user to render simple text
// NOTE: If enabled, uses external module functions to load default raylib font
#define SUPPORT_DEFAULT_FONT        1
// Selected desired font fileformats to be supported for loading
//#define SUPPORT_FILEFORMAT_FNT      1
#define SUPPORT_FILEFORMAT_TTF      1

// text: Configuration values
//------------------------------------------------------------------------------------
#define MAX_TEXT_BUFFER_LENGTH      1024        // Size of internal static buffers used on some functions:

//------------------------------------------------------------------------------------
// Module: audio - Configuration Flags
//------------------------------------------------------------------------------------
// Desired audio fileformats to be supported for loading
#define SUPPORT_FILEFORMAT_WAV      1
#define SUPPORT_FILEFORMAT_OGG      1
#define SUPPORT_FILEFORMAT_XM       1
#define SUPPORT_FILEFORMAT_MOD      1
#define SUPPORT_FILEFORMAT_MP3      1
#define SUPPORT_FILEFORMAT_FLAC     1

// audio: Configuration values
//------------------------------------------------------------------------------------
#define AUDIO_DEVICE_FORMAT    ma_format_f32    // Device output format (miniaudio: float-32bit)
#define AUDIO_DEVICE_CHANNELS              2    // Device output channels: stereo
#define AUDIO_DEVICE_SAMPLE_RATE           0    // Device sample rate (device default)

#define MAX_AUDIO_BUFFER_POOL_CHANNELS    16    // Maximum number of audio pool channels

//------------------------------------------------------------------------------------
// Module: utils - Configuration Flags
//------------------------------------------------------------------------------------
// Standard file io library (stdio.h) included
#define SUPPORT_STANDARD_FILEIO
// Show TRACELOG() output messages
//#define SUPPORT_TRACELOG            1
//#define SUPPORT_TRACELOG_DEBUG      1

// utils: Configuration values
//------------------------------------------------------------------------------------
#define MAX_TRACELOG_MSG_LENGTH          128    // Max length of one trace-log message
