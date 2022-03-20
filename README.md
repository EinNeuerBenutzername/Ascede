# Ascede

## General

Ascede is a C99 2D framework, forked from the awesome game framework: [raylib](https://www.raylib.com). You can take a look if you don't know it yet.

This project is intended for my personal use and I'm not going to put a lot of effort into this project.

What I'm doing here is basically cutting out some parts of raylib to enhance my development. My goal is to reduce functionality and even scalability in exchange for shorter development cycles and probably smaller binary sizes. **I strongly suggest you try the original raylib library instead of using this library,** because it is more functional and is under constant development, whereas Ascede is less functional, more unstable and rarely updated.

- I won't fix any bugs in Ascede that don't affect my own use, because this is a personal project. I won't bother with Issues, either.
- Ascede is 2D-only. It has no 3D support and most probably never will.
- Ascede's target platforms are desktop platforms, mainly Windows. It would rarely be tested on Linux and MacOS.
- I won't accept collaborations and pull requests because that would complicate my work.

## Status

#### Major goals

- [x] removed 3D support
  - this includes every single function in `model` and `mesh` modules and several other functions like `BeginMode3D()` and `EndMode3D()`.
- [x] remove camera support
- [x] killed snapshot and screen recording
- [x] remove text manipulation
  -  these functions remain: `TextIsEqual()`, `TextLength()`, `TextFormat()` and `TextToLower()`.
  - functions for UTF-8 and Unicode conversion are removed as well.
- [x] remake timing & frame control
- [ ] **rearrange API**
  - [x] **window** & **monitor**, **cursor**
  - [x] **mouse**, **touch**, **gamepad**, **keys**
  - [x] **time**, **events**
  - [ ] buffer, textures, **rendertextures**
  - [ ] draw modes, shaders
  - [ ] filesystem, data
  - [ ] shapes, colors
  - [ ] text, font
  - [ ] image
  - [ ] audio
- [ ] improve audio precision
- [ ] improve FPS controls
  - currently it shows the totally accurate FPS, but the number is totally unstable.
  - there should be a way to stabilize the number.
- [ ] add instancing
- [ ] further improve memory controls and prevent leakage
  - I honestly don't think that Ray's done a good job on that. Raylib is just filled with dangerous functions that might cause memory leakage.

#### Minor goals

- [x] removed string manipulation support
- [x] removed bad RNG
  - `GetCurrentMonitor()` is required in `ToggleFullscreen()`
- [x] removed VR support
- [x] removed value storage
- [x] removed all functions drawing circular shapes
  - this is because raylib draws circles by drawing 36 triangles, which is extremely slow.
  - on my PC, raylib draws 50k bunnies in the `bunnymark` example but could only draw ~2.6k circles per frame (all 60 FPS). Not very performant.
- [x] removed all functions drawing outlines of shapes
  - I find them just ugly.
- [ ] change the default font

#### Specific function changes

- [x] removed `EndDrawing()`

  - `Buffer_Update()` does the "end drawing" part and the `Events_EndLoop` does the rest.
  - I hate this design.

- [x] removed the following lines from `WindowShouldClose()`

  ```C
      // While window minimized, stop loop execution
      while (IsWindowState(FLAG_WINDOW_MINIMIZED) && !IsWindowState(FLAG_WINDOW_ALWAYS_RUN)) glfwWaitEvents();
  ```


- [x] killed timing controls in `BeginDrawing()`, which I didn't notice before. Why are they just everywhere?
- [x] removed `LoadTextureCubemap()`.
- [x] removed `UpdateTexture()` and `UpdateTextureRec`.
- [x] removed `DrawTexturePoly()`.
- [ ] these functions are removed to prevent memory leak:
  - `CompressData()`, `DecompressData()`

...

#### Problematic / worth noticing

- [ ] timing functions, especially `Time_Sleep()`, lack precision on Windows.
  - this is probably because raylib's FPS control function in `EndDrawing()` just claims that it had slept for the required period of time while it actually hadn't.
- [ ] `Texture_LoadFromImage()`

## API

#### v0.5.6.6 Cheatsheet

```c
ASCAPI void Buffer_Init();
// Setup framebuffer to start drawing
ASCAPI void Buffer_Clear(Color color);
// Clear framebuffer with a background color
ASCAPI void Buffer_Update();
// End drawing and swap screen buffer

ASCAPI const char * Clipboard_Get(void);
// Get clipboard text content
ASCAPI void Clipboard_Set(const char *text);
// Set clipboard text content

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


ASCAPI void Events_Poll();
// Poll events
ASCAPI void Events_Wait();
// Wait for events
ASCAPI void Events_EndLoop();
// End the loop

ASCAPI Font Font_Load(const char *fileName);
// Load a font from file into VRAM
ASCAPI Font Font_LoadEx(const char *fileName, int fontSize, int *fontChars, int glyphCount);
// Load a font from file with extended parameters

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

ASCAPI Image Image_Load(const char *fileName);
// Load image from file into RAM
ASCAPI Image Image_LoadRaw(const char *fileName, int width, int height, int format, int headerSize);
// Load image from RAW file data
ASCAPI Image Image_LoadAnim(const char *fileName, int *frames); 
// Load image sequence from file (frames appended to image.data)
ASCAPI Image Image_LoadFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);
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
// Export image data to file
ASCAPI bool Image_ExportCode(Image image, const char *fileName);
// Export image as code file defining an array of bytes, returns true on success

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

ASCAPI RenderTexture2D RenderTexture_Load(int width, int height);
// Create a render texture (framebuffer)
ASCAPI void RenderTexture_Free(RenderTexture2D target);
// Unload render texture
ASCAPI void RenderTexture_Init(RenterTexture2D target);
// Begin drawing to render texture
ASCAPI void RenderTexture_Update(void);
// End drawing current render texture

ASCAPI int RNG_Gen(int min, int max);
// Get a random value in range [min, max]
ASCAPI void RNG_Init(unsigned int seed);
// Set the seed of the RNG
ASCAPI void RNG_SetState(int state);
// Set the current state of the RNG
ASCAPI void RNG_GetState(void);
// Get the current state of the RNG

ASCAPI void Shape_DrawRec(int posX, int posY, int width, int height, Color color);
// Draw a color-filled rectangle
ASCAPI void Shape_DrawRecV(Vector2 position, Vector2 size, Color color);
// Draw a color-filled rectangle with vectorized parameters
ASCAPI void Shape_DrawRecRec(Rectangle rec, Color color);
// Draw a color-filld rectangle

ASCAPI void Text_Draw(const char *text, int posX, int posY, int fontSize, Color color);
// Draw text with default font
ASCAPI void Text_DrawEx(Font font, const char *text, Vector2 position, float fontsize, float spacing, Color tint);
// Draw text with custom font and extended parameters
ASCAPI void Text_DrawPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
// Draw text with even more parameters
ASCAPI void Text_DrawCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint);
// Draw one character (codepoint)

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


ASCAPI float Time_GetFPS(void);
// Get current FPS
ASCAPI float Time_GetFrame(void);
// Get time in seconds for the last frame drawn
ASCAPI double Time_Get();
// Get elapsed time in seconds
ASCAPI void Time_Sleep(float ms);
// Halt the program for several milliseconds
ASCAPI void Time_SoftSleep(float ms);
// Not very good precision but much less cpu usage
ASCAPI void Time_Wait(float targetFPS);
// Call this at the end of a loop
ASCAPI void Time_SoftWait(float targetFPS);
// Time_Wait() that calls Time_SoftSleep() instead of Time_Sleep()

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
    
        Buffer_Init();                  // ready to start drawing
        Buffer_Clear(WHITE);            // clear background with the color white
        Buffer_Update();                // end drawing
    
        Time_SoftWait(60);              // the target fps is 60
        Events_EndLoop();               // do the cleanups
    }
    return 0;
}
```

#### Notes

```c
// A Texture2D is the same as a Texture.
typedef Texture Texture2D;

// A RenderTexture2D consists of two textures.
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;

// To use a RenderTexture2D
Window_Init(400, 300);
RenderTexture2D rtx = RenderTexture_Load(400, 300); // Create the rendertexture
// Then, in each loop:
RenderTexture_Init(rtx); // Start drawing on rtx
    // Draw something...
RenderTexture_Update(); // Update current render texture
Texture_Draw(rtx.texture, 0, 0, WHITE); // Draw this render texture on screen

// If you want to scale or rotate your texture, consider setting its filters to TEXTURE_FILTER_BILINEAR or TEXTURE_FILTER_TRILINEAR to avoid bleeding edges.
Image img = LoadImage("bunny.png");
Texture2D texture = Texture_LoadFromImage(img);
Texture_SetFilter(texture, TEXTURE_FILTER_BILINEAR);

// Ascede uses double-buffering, so if the framebuffer is only updated once, this function would produce a blank image of the current, unupdated framebuffer. To avoid that, either at least update the framebuffer twice before using this function or use this function before Buffer_Update().
Image_Screenshot()
    
// FPS controls
// The Events_Poll() or Events_Wait() functions should always be used first in a loop, and the Events_Endloop() is required to end a loop.
Events_Poll();
Events_Wait();
Events_EndLoop();
// With Events_Wait(), it's not very necessary to use Time_*() functions. But with Events_Poll(), Time_Wait() and similar functions are required to lower CPU usage. Time_SoftWait() has even lower CPU usage.
Time_SoftWait(60); // Target FPS: 60
```

## Related projects

[**raylib**](https://www.raylib.com) : a C99 framework, on which this library is based.

[**raylib-instancing**](https://github.com/ChrisDill/raylib-instancing) : self-explanatory.

[**mtwister**](https://github.com/ESultanik/mtwister) : a Mersenne Twister implementation