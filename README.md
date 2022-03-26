# Ascede

## General

Ascede is a C99 2D framework, forked from the awesome game framework: [raylib](https://www.raylib.com). You can take a look if you don't know it yet.

This project is intended for my personal use and I'm not going to put a lot of effort into this project.

What I'm doing here is basically cutting out some parts of raylib to enhance my development. My goal is to reduce functionality and even scalability in exchange for shorter development cycles and probably smaller binary sizes. **I strongly suggest you try the original raylib library instead of using this library,** because it is more functional and is under constant development, whereas Ascede is less functional, more unstable and rarely updated.

- I won't fix any bugs in Ascede that don't affect my own use, because this is a personal project. I won't bother with Issues, either.
- Ascede is 2D-only. It has no 3D support and most probably never will.
- Ascede's target platforms are desktop platforms, mainly Windows. It would rarely be tested on Linux and MacOS.
- I won't accept collaborations and pull requests because that would complicate my work.

If you find these clauses acceptable, you may start using Ascede. Please remember to check the "**Important notes for novice users**" below. It introduces several probably crucial tips and tricks in Ascede.

## Status

#### Audio module unavailable yet

#### Major goals

- [x] 1. Removed 3D support
  - This includes every single function in `model` and `mesh` modules and several other functions like `BeginMode3D()` and `EndMode3D()`.
  
- [x] 2. Removed camera support

- [x] 3. Killed snapshot and screen recording

- [x] 4. Removed text manipulation
  -  These functions remain: `TextIsEqual()`, `TextLength()`, `TextFormat()` and `TextToLower()`.
  - Functions for UTF-8 and Unicode conversion are removed as well.
  
- [x] 5. Remake timing & frame control

- [x] 6. Improved FPS controls

- [x] 7. Further improve memory controls and prevent leakage

- [x] 8. Rearrange API
  - [x] window, monitor, cursor
  - [x] mouse, touch, gamepad, keys
  - [x] time, events
  - [x] buffer, texture, rendertexture, shader
  - [x] filesystem
  - [x] shape, color, image
  - [x] text, font
  
- [x] 9. Make minimal binary size on Windows less than 750kB
  
  - Current minimal size: 730.5kB (Codeblocks 20.03 / GCC 8.1)
  - Configuration: `-static-libgcc -static-libstdc++ -m32 -flto -Os -s`
  
- [ ] Add instancing

- [ ] Add audio channeling

- [ ] Improve audio precision

#### Minor adjustments

- [x] 1. Removed string manipulation support
- [x] 2. Removed bad, `stdlib.h`-dependent RNG
- [x] 3. Removed VR support
- [x] 4. Removed value storage
- [x] 5. Removed all functions drawing circular shapes
  - this is because raylib draws circles by drawing 36 triangles, which is extremely slow.
  - on my PC, raylib draws 50k bunnies in the `bunnymark` example but could only draw ~2.6k circles per frame (all 60 FPS). Not very performant.
- [x] 6. Removed all functions drawing outlines of shapes
  - I find them just ugly.
- [x] 7. Removed collision detection functions
- [ ] Change the default font?

#### Specific function changes

- [x] 1. Removed `EndDrawing()`
  - `Buffer_Update()` does the "end drawing" part and the `Events_EndLoop` does the rest.
  - I hate this design.
- [x] 2. Removed the following lines from `WindowShouldClose()`
  ```C
      // While window minimized, stop loop execution
      while (IsWindowState(FLAG_WINDOW_MINIMIZED) && !IsWindowState(FLAG_WINDOW_ALWAYS_RUN)) glfwWaitEvents();
  ```
- [x] 3. Killed timing controls in `BeginDrawing()`, which I didn't notice before. Why are they just everywhere?
- [x] 4. Removed `LoadTextureCubemap()`.
- [x] 5. Removed `UpdateTexture()` and `UpdateTextureRec`.
- [x] 6. Removed `DrawTexturePoly()`.
- [x] 7. These functions are removed to prevent memory leak:
  - `CompressData()`, `DecompressData()`
- [x] 8. In `InitGraphicsDevice()`, commented this line:
  ```C
      const int fps = (CORE.Time.target > 0) ? (1.0/CORE.Time.target) : 60;
  ```
  - I don't actually know what this does.
- [x] 9. Removed `DrawPixel()` and `DrawPixelV()`.
  - In fact, these functions draw lines with a length of 1 pixel. Thus I don't find them necessary.
- [x] 10. Removed `DrawLineBezier()`, `DrawLineBezierQuad()`, `DrawLineBezierCubic()`, `DrawLineStrip()` and`DrawTriangleFan()`.
- [x] 11. Removed `DrawTextureQuad()`, `DrawTextureTiled()` and `DrawTextureNPatch()`.
- [x] 12. Removed `DrawRectangleGradientV()`, `DrawRectangleGradientH()`, `DrawRectangleGradientEx()` and `DrawRectangleRounded()`. 
- [x] 13. Extracted `ImageTextEx()`, `ImageResize()`, `ImageResizeNN()` and `ImageResizeCanvas()` from `#ifdef SUPPORT_IMAGE_MANIPULATION`. Thus `stb_image_resize.h` is included anyway.
- [x] 14. Removed `ColorToInt()`, `ColorNormalize()`, `ColorFromNormalized()`, `ColorToHSV()`, `ColorFromHSV()`, `ColorAlpha()` and `GetColor()`.
- [x] 15. Removed `OpenURL()`.
- [x] 16. Made `LoadImageColors()`, `LoadImagePalette()`, `UnloadImageColors()`, ``UnloadImagePalette()`, `GetImageAlphaBorder()` and `GetImageColor()` static.
- [x] 17. Made Glyph-related functions static.

- [x] 18. Removed `GetDirectoryPath()` and `GetPrevDirectoryPath()` because I'm pretty sure that they're not that useful.
- [ ] 

...

#### Problematic / worth noticing

- [x] Timing functions, especially `Time_Sleep()`, lack precision on Windows.
  - This is probably because raylib's FPS control function in `EndDrawing()` just claims that it had slept for the required period of time while it actually hadn't.
  - To solve the problem, I introduced two functions: `Time_GetFPS()`, which is only an indicator of whether the program is running without latency, and `Time_GetRealFPS()`, a function that returns the actual FPS.
  - Usually, we don't need to know whether the program is running on 60.00 FPS or 60.12 FPS. Ascede uses a tolerant FPS controlling mechanic that adjusts sleeping time after each loop, so the overall FPS would be accurate as long as there aren't lags.
  - In development, I strongly recommend using `Time_GetFPS()` instead of `Time_GetRealFPS()`.
- [ ] `Texture_LoadFromImage()`

## API

#### v0.9.7.19 Cheatsheet

###### Buffer

```c
ASCAPI void Buffer_Begin(void);
// Setup framebuffer to start drawing
ASCAPI void Buffer_Clear(Color color);
// Clear framebuffer with a background color
ASCAPI void Buffer_Update(void);
// End drawing and swap screen buffer
ASCAPI void Buffer_BeginBlend(int mode);
// Begin blending mode (alpha, additive, multiplied, subtract, custom)
ASCAPI void Buffer_UpdateBlend(void);
// End blending mode (reset to default: alpha blending)
ASCAPI void Buffer_BeginScissor(int x, int y, int width, int height);
// Begin scissor mode (define screen area for following drawing)
ASCAPI void Buffer_EndScissor(void);
// End scissor mode
```

###### Clipboard

```c
ASCAPI const char * Clipboard_Get(void);
// Get clipboard text content
ASCAPI void Clipboard_Set(const char *text);
// Set clipboard text content
```

###### Color

```c
ASCAPI Color Color_Fade(Color color, float alpha);
// Get color with alpha applied, alpha goes from 0.0f to 1.0f
ASCAPI Color Color_AlphaBlend(Color dst, Color src, Color tint);
// Get src alpha-blended into dst color with tint
ASCAPI Color Color_GetPixel(void *srcPtr, int format);
// Get Color from a source pixel pointer of certain format
ASCAPI void Color_SetPixel(void *dstPtr, Color color, int format);
// Set color formatted into destination pixel pointer
ASCAPI void Color_GetPixelDataSize(int width, int height, int format);
// Get pixel data size in bytes for certain format
```

###### Cursor

```c
ASCAPI void Cursor_Show(void);
// Show cursor
ASCAPI void Cursor_Hide(void);
// Hide cursor
ASCAPI bool Cursor_IsHidden(void);
// Check if cursor is hidden
ASCAPI void Cursor_Enable(void);
// Unlock cursor
ASCAPI void Cursor_Disable(void);
// Lock cursor
ASCAPI bool Cursor_IsOnScreen(void);
// Check if cursor is on screen
```

###### Events

```c
ASCAPI void Events_Poll();
// Poll events
ASCAPI void Events_Wait();
// Wait for events
ASCAPI void Events_EndLoop();
// End the loop and do clean-ups
```

###### Filesystem

```C
ASCAPI unsigned char *File_Load(const char *fileName, unsigned int *bytesRead);
// Load file data as byte array (read)
ASCAPI void File_Free(unsigned char *data);
// Unload file data allocated by File_Load()
ASCAPI bool File_Save(const char *fileName, void *data, unsigned int bytesToWrite);
// Save data to file from byte array (write), returns true on success
ASCAPI char *File_LoadStr(const char *fileName);
// Load text data from file (read), returns a '\0' terminated string
ASCAPI void File_FreeStr(char *text);
// Unload file text data allocated by File_LoadStr()
ASCAPI bool File_SaveStr(const char *fileName, char *text);
// Save text data to file (write), string must be '\0' terminated, returns true on success
ASCAPI bool File_Exists(const char *fileName);
// Check if file exists
ASCAPI bool File_DirExists(const char *dirPath);
// Check if directory exists
ASCAPI bool File_IsExt(const char *fileName, const char *ext);
// Check file extension (including the dot: '.png', '.wav')
ASCAPI const char *File_GetExt(const char *fileName);
// Get pointer to extension for a filename string (includes dot: '.png')
ASCAPI const char *File_GetName(const char *filePath);
// Get pointer to filename for a path string
ASCAPI const char *File_GetNameNX(const char *filePath);
// Get filename string without extension (uses static string)
ASCAPI const char *File_GetWorkingDir(void);
// Get current working directory (uses static string)
ASCAPI bool File_SetWorkingDir(const char *dir);
// Change working directory, return true on success
ASCAPI char **File_LoadDirFileList(const char *dirPath, int *count);
// Get filenames in a directory path (max 512 files)
ASCAPI void File_FreeDirFileList(void);
// Clear directory files paths buffers
ASCAPI bool File_IsDropped(void);
// Check if a file has been dropped into window
ASCAPI char **File_GetDroppedList(int *count);
// Get dropped files names (memory should be freed)
ASCAPI void File_ClearDroppedList(void);
// Clear dropped files paths buffer (free memory)
ASCAPI long File_GetModTime(const char *fileName);
// Get file modification time (last write time)
```

###### Font

```c
ASCAPI Font Font_GetDefault(void);
// Get the default font
ASCAPI Font Font_Load(const char *fileName);
// Load a font from file into VRAM
ASCAPI Font Font_LoadEx(const char *fileName, int fontSize, int *fontChars, int glyphCount);
// Load a font from file with extended parameters
ASCAPI Font Font_LoadMem(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount);
// Load a font from memory, fileType is a string i.e. '.ttf'
ASCAPI Font Font_LoadFromImage(Image image, Color key, int firstChar);
// Load a font from an Image (XNA style)
ASCAPI Font Font_Free(Font font);
// Free font from VRAM
```

###### Gamepad

```c
ASCAPI bool Gamepad_IsAvailable(int gamepad);
// Check if a gamepad is available
ASCAPI const char *Gamepad_GetName(int gamepad);
// Get gamepad internal name
ASCAPI int Gamepad_CountAxis(int gamepad);
// Get gamepad axis count
ASCAPI float Gamepad_GetAxisMovment(int gamepad, int axis);
// Get axis movment value for a gamepad axis
ASCAPI int Gamepad_SetMappings(const char *mappings);
// Set gamepad mappings
ASCAPI bool Gamepad_IsPressed(int gamepad, int button);
// Check if a gamepad button is pressed
ASCAPI bool Gamepad_IsDown(int gamepad, int button);
// Check if a gamepad button is being pressed
ASCAPI bool Gamepad_IsReleased(int gamepad, int button);
// Check if a gamepad button is released
ASCAPI bool Gamepad_IsUp(int gamepad, int button);
// Check if a gamepad button is not being pressed
ASCAPI int Gamepad_GetPressed(void);
// Get the last gamepad button pressed
```

###### Image General

```c
ASCAPI Image Image_Load(const char *fileName);
// Load image from file into RAM
ASCAPI Image Image_LoadRaw(const char *fileName, int width, int height, int format, int headerSize);
// Load image from RAW file data
ASCAPI Image Image_LoadAnim(const char *fileName, int *frames); 
// Load image sequence from file (frames appended to image.data)
ASCAPI Image Image_LoadMem(const char *fileType, const unsigned char *fileData, int dataSize);
// Load image from memory buffer, fileType refers to extension: i.e. '.png'
ASCAPI Image Image_LoadFromTexture(Texture2D texture);
// Load image from GPU texture data
ASCAPI Image Image_Gen(int width, int height, Color color);
// Generate an image with plain colors
ASCAPI void Image_Free(Image image);
// Unload image from RAM
ASCAPI Image Image_Screenshot(void);
// Load an image from current framebuffer
ASCAPI bool Image_Export(Image image, const char *fileName);
// Export image data to file (.png, .bmp, .tga, .jpg)
ASCAPI bool Image_ExportCode(Image image, const char *fileName);
// Export image as code file defining an array of bytes, returns true on success
```

###### Image Manipulation

```c
ASCAPI Image Image_Copy(Image image);
// Create a duplicate for an image
ASCAPI Image Image_FromImage(Image image, Rectangle rec);
// Create an image from another image piece
ASCAPI Image Image_FromText(const char *text, int fontSize, Color color);
// Create an image from default text
ASCAPI Image Image_FromTextEx(Font font, const char *text, float fontSize, float spacing, Color tint);
// Create an image from text
ASCAPI void Image_SetFormat(Image *image, int newFormat);
// Convert image format
ASCAPI void Image_ToPOT(Image *image, Color fill);
// Convert image to power-of-two
ASCAPI void Image_Crop(Image *image, Rectangle crop);
// Crop an image to a defined rectangle
ASCAPI void Image_AlphaCrop(Image *image, float threshold);
// Crop image depending on alpha value
ASCAPI void Image_AlphaClear(Image *image, Color color, float threshold);
// Clear alpha channel to desired color
ASCAPI void Image_AlphaMask(Image *image, Image alphaMask);
// Apply alpha mask to image
ASCAPI void Image_AlphaPremultiply(Image *image);
// Premultiply alpha channel
ASCAPI void Image_Resize(Image *image, int newWidth, int newHeight);
// Resize image (Bicubic scaling algorithm)
ASCAPI void Image_ResizeNN(Image *image, int newWidth,int newHeight);
// Resize image (Nearest-Neighbor scaling algorithm)
ASCAPI void Image_ResizeCanvas(Image *image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill);
// Resize canvas and fill with color
ASCAPI void Image_GenMipmaps(Image *image);
// Compute all mipmap levels for a provided image
ASCAPI void Image_Dither(Image *image, int rBpp, int gBpp, int bBpp, int aBpp);
// Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
ASCAPI void Image_FlipV(Image *image);
// Vertically flip an image
ASCAPI void Image_FlipH(Image *image);
// Horizontally flip an image
ASCAPI void Image_RotateCW(Image *image);
// Rotate image clockwise 90deg
ASCAPI void Image_RotateCCW(Image *image);
// Rotate image counter-clockwise 90deg
ASCAPI void Image_ColorTint(Image *image, Color color);
// Modify image color: tint
ASCAPI void Image_ColorInvert(Image *image);
// Modify image color: invert
ASCAPI void Image_ColorGrayscale(Image *image);
// Modify image color: grayscale
ASCAPI void Image_ColorContrast(Image *image, float contrast);
// Modify image color: contrast (-100 to 100)
ASCAPI void Image_ColorBrightness(Image *image, int brightness);
// Modify image color: brightness (-255 to 255)
ASCAPI void Image_ColorReplace(Image *image, Color color, Color replace);
// Modify image color: replace color
```

###### Image Drawing

```C
ASCAPI void Image_Clear(Image *dst, Color color);
// Clear image background
ASCAPI void Image_DrawPixel(Image *dst, int posX, int posY, Color color);
// Draw pixel within an image
ASCAPI void Image_DrawPixelV(Image *dst, Vector2 position, Color color);
// Draw pixel within an image (Vector version)
ASCAPI void Image_DrawLine(Image *dst, int startPosX, int startPosY, int endPosX, int endPosY, Color color);
// Draw line within an image
ASCAPI void Image_DrawLineV(Image *dst, Vector2 start, Vector2 end, Color color);
// Draw line within an image (Vector version)
ASCAPI void Image_DrawCircle(Image *dst, int centerX, int centerY, int radius, Color color);
// Draw circle within an image
ASCAPI void Image_DrawCircleV(Image *dst, Vector2 center, int radius, Color color);
// Draw circle within an image (Vector version)
ASCAPI void Image_DrawRectangle(Image *dst, int posX, int posY, int width, int height, Color color);
// Draw rectangle within an image
ASCAPI void Image_DrawRectangleV(Image *dst, Vector2 position, Vector2 size, Color color);
// Draw rectangle within an image (Vector version)
ASCAPI void Image_DrawRectangleRec(Image *dst, Rectangle rec, Color color);
// Draw rectangle within an image
ASCAPI void Image_DrawText(Image *dst, const char *text, int posX, int posY, int fontSize, Color color);
// Draw text (using default font) within an image (destination)
ASCAPI void Image_DrawTextEx(Image *dst, Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
// Draw text (custom sprite font) within an image (destination)
ASCAPI void Image_Draw(Image *dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);
// Draw a source image within a destination image (tint applied to source)
```

###### Key

```c
ASCAPI bool Key_IsPressed(int key);
// Check if a key has been pressed
ASCAPI bool Key_IsReleased(int key);
// Check if a key has been released
ASCAPI bool Key_IsDown(int key);
// Check if a key is being pressed
ASCAPI bool Key_IsUp(int key);
// Check if a key is not being pressed
ASCAPI void Key_SetExitHotKey(int key);
// Set a custom key to end the program
ASCAPI int Key_Get(void);
// Get the keycode for the next pressed key in the queue
ASCAPI int Key_GetChar(void);
// Get the next character (unicode) in the input queue
```

###### Miscellaneous

```C
ASCAPI void Callback_SetTraceLog(TraceLogCallback callback);
// Set custom trace log
ASCAPI void Callback_SetLoadFileData(LoadFileDataCallback callback);
// Set custom file binary data loader
ASCAPI void Callback_SetSaveFileData(SaveFileDataCallback callback);
// Set custom file binary data saver
ASCAPI void Callback_SetLoadFileText(LoadFileTextCallback callback);
// Set custom file text data loader
ASCAPI void Callback_SetSaveFileText(SaveFileTextCallback callback);
// Set custom file text data saver
ASCAPI void TraceLog(int logLevel, const char *text, ...);
// Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
ASCAPI void TraceLog_SetLevel(int logLevel);
// Set the current threshold (minimum) log level
void ASC_ERROR();
// Error callback of memory management module, closes the program
void *ASC_MALLOC(size_t bytes);
// Allocate uninitialized memory trunk
void *ASC_CALLOC(size_t num, size_t bytes);
// Allocate memory
void *ASC_REALLOC(void *pointer,size_t bytes);
// Reallocate memory
void ASC_FREE(void *pointer);
// Free allocated memory
void ASC_FREEALL();
// Free all allocated memory
```

###### Monitor

```c
ASCAPI void Monitor_Set(int monitor);
// Set monitor for current window (fullscreened)
ASCAPI int Monitor_Count(void);
// Count connected monitors
ASCAPI int Monitor_Get(void);
// Get current monitor index
ASCAPI Vector2 Monitor_GetPos(int monitor);
// Get monitor position
ASCAPI int Monitor_GetWidth(int monitor);
// Get monitor width
ASCAPI int Monitor_GetHeight(int monitor);
// Get monitor height
ASCAPI int Monitor_GetPhysicalWidth(int monitor);
// Get physical width (millimetres)
ASCAPI int Monitor_GetRefreshRate(int monitor);
// Get monitor refresh rate
ASCAPI const char* Monitor_GetName(int monitor);
// Gt human-readable, UTF-8 encoded name
```

###### Mouse

```c
ASCAPI bool Mouse_IsPressed(int button);
// Check if a mouse button is pressed
ASCAPI bool Mouse_IsDown(int button);
// Check if a mouse button is being pressed
ASCAPI bool Mouse_IsReleased(int button);
// Check if a mouse button is released
ASCAPI bool Mouse_IsUp(int button);
// Check if a mouse button is not being pressed
ASCAPI int Mouse_GetX(void);
// Get mouse position X
ASCAPI int Mouse_GetY(void);
// Get mouse position Y
ASCAPI Vector2 Mouse_GetPos(void);
// Get mouse position
ASCAPI Vector2 Mouse_GetDelta(void);
// Get mouse movement since last frame
ASCAPI void Mouse_SetPos(int x, int y);
// Set mouse position
ASCAPI void Mouse_SetOffset(int x, int y);
// Set mouse position
ASCAPI void Mouse_SetScale(float scaleX, float scaleY);
// Set mouse position
ASCAPI float Mouse_GetWheelDelta(void);
// Get wheel movement since last frame
ASCAPI void Mouse_SetCursor(void);
// Set mouse cursor
```

###### RenderTexture

```c
ASCAPI RenderTexture2D RenderTexture_Load(int width, int height);
// Create a render texture (framebuffer)
ASCAPI void RenderTexture_Free(RenderTexture2D target);
// Unload render texture
ASCAPI void RenderTexture_Begin(RenterTexture2D target);
// Begin drawing to render texture
ASCAPI void RenderTexture_Update(void);
// End drawing current render texture
```

###### RNG

```c
ASCAPI double RNG_GenD(void);
// Get a random double in range [0,1]
ASCAPI int RNG_Gen(int min, int max);
// Get a random value in range [min, max]
ASCAPI void RNG_Init(unsigned int seed);
// Set the seed of the RNG
ASCAPI void RNG_SetState(int state);
// Set the current state of the RNG
ASCAPI void RNG_GetState(void);
// Get the current state of the RNG
```

###### Shader

```c
ASCAPI Shader Shader_Load(const char *vsFileName, const char *fsFileName);
// Load shader from files and bind default locations
ASCAPI Shader Shader_LoadData(const char *vsCode, const char *fsCode);
// Load shader from code strings and bind default locations
ASCAPI void Shader_Free(Shader shader);
// Free shader from VRAM
ASCAPI void Shader_Begin(Shader shader);
// Begin custom shader drawing
ASCAPI void Shader_Update(void)
// End custom shader mode (returns to default shader)
ASCAPI int Shader_GetLoc(Shader shader, const char *uniformName);
// Get shader uniform location
ASCAPI int Shader_GetLocAttrib(Shader shader, const char *attribName);
// Get shader attribute location
ASCAPI void Shader_SetValue(Shader shader, int locIndex, const void *value, int uniformType);
// Set shader uniform value
ASCAPI void Shader_SetValueV(Shader shader, int locIndex, const void *value, int uniformType, int count);
// Set shader uniform value vector
ASCAPI void Shader_SetValueMatrix(Shader shader, int locIndex, Matrix mat);
// Set shader uniform value (matrix 4x4)
ASCAPI void Shader_SetValueTexture(Shader shader, int locIndex, Texture2D texture);
// Set shader uniform value for texture (sampler2d)
```

###### Shape

```c
ASCAPI void Shape_SetTexture(Texture2D texture, Rectangle source);
// Set texture and rectangle to be used on shapes drawing
ASCAPI void Shape_DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
// Draw a line
ASCAPI void Shape_DrawLineV(Vector2 startPos, Vector2 endPos, Color color);
// Draw a line with vectorized parameters
ASCAPI void Shape_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);
// Draw a line with extended parameters
ASCAPI void Shape_DrawRec(int posX, int posY, int width, int height, Color color);
// Draw a color-filled rectangle
ASCAPI void Shape_DrawRecV(Vector2 position, Vector2 size, Color color);
// Draw a color-filled rectangle with vectorized parameters
ASCAPI void Shape_DrawRecRec(Rectangle rec, Color color);
// Draw a color-filld rectangle
ASCAPI void Shape_DrawRecPro(Rectangle rec, Vector2 origin, float rotation, Color color);
// Draw a color-filled rectangle with pro parameters
ASCAPI void Shape_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
// Draw a color-filled triangle, vertex in counter-clockwise order
ASCAPI void Shape_DrawTriangleStrip(Vector2 *points, int pointCount, Color color);
// Draw a triangl strip defined by a point array
ASCAPI void Shape_DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
// Draw a polygon
```

###### Text

```c
ASCAPI void Text_Draw(const char *text, int posX, int posY, int fontSize, Color color);
// Draw text with default font
ASCAPI void Text_DrawEx(Font font, const char *text, Vector2 position, float fontsize, float spacing, Color tint);
// Draw text with custom font and extended parameters
ASCAPI void Text_DrawPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
// Draw text with even more parameters
ASCAPI void Text_DrawCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint);
// Draw one character (codepoint)
ASCAPI bool Text_IsEqual(const char *text1, const char *text2);
// Check if two text string are equal
ASCAPI unsigned int Text_Length(const char *text);
// Get text length, checks for '\0' ending
ASCAPI const char *Text_Format(const char *text, ...);
// Text formatting with variables (sprintf() style)
ASCAPI const char *Text_ToLower(const char *text); 
// Get lower case version of provided string
ASCAPI int Text_GetCodepoint(const char *text, int *bytesProcessed);
// Get codepoint of a UTF-8 character
ASCAPI int Text_GetWidth(const char *text, int fontSize); 
// Measure string width for font
ASCAPI Vector2 Text_GetWidthEx(Font font, const char *text, float fontSize, float spacing);
```

###### Texture

```c
ASCAPI Texture2D Texture_Load(const char *fileName);
// Load a texture from file
ASCAPI Texture2D Texture_LoadFromImage(Image image);
// Load a texture from an image
ASCAPI void Texture_Free(Texture2D texture);
// Unload texture from GPU
ASCAPI void Texture_Update(Texture2D texture, const void *pixels);
// Update texture with new data
ASCAPI void Texture_SetFilter(Texture2D texture, int filter);
// Set texture filter, e.g. TEXTURE_FILTER_POINT, TEXTURE_FILTER_BILINEAR
ASCAPI void Texture_SetWrap(Texture2D texture, int wrap);
// Set texture wrap, e.g. TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_CLAMP
ASCAPI void Texture_GenMipmaps(Texture2D *texture);
// Generate GPU mipmaps for the texture
ASCAPI void Texture_Draw(Texture2D texture, int posX, int posY, Color tint);
// Draw a Texture2D. When the tint parameter is WHITE, it does not change the texture's colors.
ASCAPI void Texture_DrawV(Texture2D texture, Vector2 position, Color tint);
// Draw a Texture2D with vectorized position parameter.
ASCAPI void Texture_DrawRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
// Draw a part of a texture (defined by a rectangle)
ASCAPI void Texture_DrawEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
// Draw a Texture2D with extended parameters
ASCAPI void Texture_DrawPro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
// Draw a part of a texture (defined by a rectangle) with 'pro' parameters
```

###### Time

```c
ASCAPI float Time_GetFPS(void);
// Get current FPS
ASCAPI float Time_GetFrame(void);
// Get time in seconds for the last frame drawn
ASCAPI float Time_GetRealFPS(void);
// Get real current FPS
ASCAPI float Time_GetRealFrame(void);
// Get accurate time in seconds for the last frame drawn
ASCAPI double Time_Get(void);
// Get elapsed time in seconds
ASCAPI void Time_Sleep(float ms);
// Halt the program for several milliseconds
ASCAPI void Time_Wait(float targetFPS);
// Call this at the end of a loop
```

###### Touch

```c
ASCAPI void Touch_GetX(void);
// Get touch position for touch point 0 or mouse position on desktop, relative to screen size
ASCAPI void Touch_GetY(void);
// Get touch position for touch point 0 or mouse position on desktop, relative to screen size
ASCAPI Vector2 Touch_GetPos(int index);
// Get touch position for a touch point, relative to screen size
ASCAPI int Touch_GetID(int index);
// Gt touch point indentifier
ASCAPI int Touch_Count(void);
// Get number of touch points
```

###### Window

```c
ASCAPI void Window_Init(size_t width, size_t height, const char *title);
// Initialize window and OpenGL context
ASCAPI bool Window_ShouldClose(void);
// Check if the window should close (call this only once in each loop)
ASCAPI void Window_Close(void);
// Close window and unload OpenGL context
ASCAPI bool Window_IsReady(void);
// Check if window has been initialized
ASCAPI bool Window_IsFullscreen(void);
// Check if window is currently fullscreen
ASCAPI bool Window_ToggleFullscreen(void)
// Fullscreen <-> windowed
ASCAPI bool Window_IsHidden(void);
// Check if window is currently hidden
ASCAPI bool Window_IsMinimized(void);
// Check if window is currently minimized/iconified
ASCAPI bool Window_IsMaximized(void);
// Check if window is currently maximized
ASCAPI bool Window_IsFocused(void);
// Check if window is currently focused
ASCAPI bool Window_IsResized(void);
// Check if window has been resized since last frame
ASCAPI void Window_Minimize(void);
// Minimize/iconify window if resizable
ASCAPI void Window_Maximize(void);
// Maximize window if resizable
ASCAPI void Window_Restore(void);
// Make window not minimized or maximized
ASCAPI bool Window_GetFlags(unsigned int flags);
// Check if specific flags are enabled
ASCAPI bool Window_SetFlags(unsigned int flags);
// Configure window with flags after window creation
ASCAPI bool Window_PresetFlags(unsigned int flags);
// Configure window with flags before window creation
ASCAPI bool Window_ResetFlags(unsigned int flags);
// Clear window configuration state flags
ASCAPI bool Window_SetTitle(const char *title);
// Set the title for window
ASCAPI void Window_SetPos(int x, int y);
// Set window position
ASCAPI void Window_SetMinSize(int width, int height);
// Set window minimal dimensions
ASCAPI void Window_SetSize(size_t width, size_t height);
// Set window dimensions
ASCAPI size_t Window_GetWidth(void);
// Get current screen width
ASCAPI size_t Window_GetHeight(void);
// Get current screen height
ASCAPI Vector2 Window_GetDPI(void);
// Get window scale DPI factor
ASCAPI Vector2 Window_GetPos(void);
// Get window position
ASCAPI void *Window_GetHandle(void);
// Get native window handle
ASCAPI void Window_SetIcon(Image image);
// Set window icon into an image
```

#### Basic Example

```c
#include "src/ascede.h" // ascede!
int main(){
    Window_Init(800,600,"Basic Window");// initialize a window first to create OpenGL context
    Key_SetExitHotkey(KEY_ESCAPE);      // the program exits when the key is pressed
    while(!Window_ShouldClose()){
        Events_Poll();                  // poll input events at the start of a loop
        // Main loop starts
        Buffer_Begin();                  // ready to start drawing
        Buffer_Clear(WHITE);            // clear background with the color white
        Buffer_Update();                // end drawing
        // Main loop ends
        Time_Wait(60);                  // the target fps is 60
        Events_EndLoop();               // do the cleanups
    }
    return 0;
}
```

#### Important notes for novice users

###### Texture2D and RenderTexture2D
```C
// A Texture2D is the same as a Texture.
typedef Texture Texture2D;
```

```C
// A RenderTexture2D consists of two textures.
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;
```

```C
// To use a RenderTexture2D
Window_Init(400, 300);
RenderTexture2D rtx = RenderTexture_Load(400, 300); // Create the rendertexture
// Then, in each loop:
RenderTexture_Begin(rtx); // Start drawing on rtx
Buffer_Clear(WHITE);
    // Draw something...
RenderTexture_Update(); // Update current render texture
Texture_Draw(rtx.texture, 0, 0, WHITE); // Draw this render texture on screen
```

```C
// If you want to scale or rotate your texture, consider setting its 
// filters to TEXTURE_FILTER_BILINEAR or TEXTURE_FILTER_TRILINEAR to avoid 
// bleeding edges.
Image img = LoadImage("bunny.png");
Texture2D texture = Texture_LoadFromImage(img);
Texture_SetFilter(texture, TEXTURE_FILTER_BILINEAR);
```

```C
// Screenshots

// Ascede uses double-buffering, so if the framebuffer is only updated 
// once, this function would produce a blank image of the current, 
// unupdated framebuffer. To avoid that, either at least update the 
// framebuffer twice before using this function or use this function 
// before Buffer_Update().
Image_Screenshot()
```

###### Timing and FPS controls

```C
// FPS controls

// The Events_Poll() or Events_Wait() functions should always be used 
// first in a loop, and the Events_Endloop() is required to end a loop.
while(!Window_ShouldClose()){
    Events_Wait();
    // the main loop here...
    Events_EndLoop();
}

// With Events_Wait(), it's not very necessary to use Time_*() functions. 
// But with Events_Poll(), Time_Wait() is required to lower CPU usage.
while(!Window_ShouldClose()){
    Events_Poll();
    // the main loop here...
    Time_Wait(60); // Target FPS: 60
    Events_EndLoop();
}
```

```C
// FPS statistics

// These are not actual FPS and frame time, but indicators of wheter the 
// game is working properly. They are calculated ONLY in Time_Wait().
Time_GetFPS();
Time_GetFrame();
// If there isn't a Time_Wait() in your loop, please consider adding one. 
// These functions would return the same values as their "real" 
// alternatives when the loop is without Time_Wait() functions.

// These functions below return actual FPS statistics. They are calculated 
// in Events_Endloop() and are totally accurate.
Time_GetRealFPS();
Time_GetRealFrame();
```

## Related projects

[**raylib**](https://www.raylib.com) : a C99 framework, on which this library is based.

[**raylib-instancing**](https://github.com/ChrisDill/raylib-instancing) : self-explanatory.

[**mtwister**](https://github.com/ESultanik/mtwister) : a Mersenne Twister implementation