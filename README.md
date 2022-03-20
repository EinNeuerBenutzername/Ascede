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

#### Major updates

- [x] removed 3D support
  - this includes every single function in `model` and `mesh` modules and several other functions like `BeginMode3D()` and `EndMode3D()`.
- [x] removed camera support
- [x] killed snapshot and screen recording
- [ ] remove text manipulation
  - some strings allocate memory internally for returned strings. just why...
- [x] remake timing & frame control
- [ ] **rearrange API**
  - [x] **window** & **monitor**, **cursor**
  - [x] **mouse**, **touch**, **gamepad**, **keys**
  - [x] **time**, **events**
  - [ ] buffer, textures
  - [ ] draw modes, shaders
  - [ ] filesystem, data
  - [ ] shapes, colors
  - [ ] text, font
  - [ ] image
  - [ ] audio
- [ ] improve audio precision
- [ ] add instancing

#### Minor updates

- [x] removed string manipulation support
- [x] removed bad RNG
  - `GetCurrentMonitor()` is required in `ToggleFullscreen()`
- [x] removed VR support
- [x] removed value storage
- [x] removed all functions drawing circular shapes
- [ ] **change the default font**

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

...

#### Very Problematic

- [ ] Timing functions, esp. `Time_Sleep()`, extremely lack precision.

## API

#### v0.4.5.3 Cheatsheet

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

ASCAPI int RNG_Gen(int min, int max);
// Get a random value in range [min, max]
ASCAPI void RNG_Init(unsigned int seed);
// Set the seed of the RNG
ASCAPI void RNG_SetState(int state);
// Set the current state of the RNG
ASCAPI void RNG_GetState(void);
// Get the current state of the RNG

ASCAPI void Texture_SetFilter(Texture2D texture, int filter);
// Set texture filter, e.g. TEXTURE_FILTER_POINT, TEXTURE_FILTER_BILINEAR
ASCAPI void Texture_SetWrap(Texture2D texture, int wrap);
// Set texture wrap, e.g. TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_CLAMP

ASCAPI float Time_GetFPS(void);
// Get current FPS
ASCAPI float Time_GetFrame(void);
// Get time in seconds for the last frame drawn
ASCAPI double Time_Get();
// Get elapsed time in seconds
ASCAPI void Time_Sleep(float ms);
// Halt the program for several milliseconds
ASCAPI void Time_SoftSleep(float ms);
// Worse precision but less cpu usage
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

## Related projects

[**raylib**](https://www.raylib.com) : a C99 framework, on which this library is based.

[**raylib-instancing**](https://github.com/ChrisDill/raylib-instancing) : self-explanatory.

[**mtwister**](https://github.com/ESultanik/mtwister) : a Mersenne Twister implementation