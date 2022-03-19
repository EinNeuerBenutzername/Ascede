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
- [ ] remake timing & frame control
- [ ] **rearrange API**
- [ ] improve audio precision
- [ ] add instancing

#### Minor updates

- [x] removed string manipulation support
- [x] removed bad RNG
- [x] (temporarily) disabled most monitor controls
  - `GetCurrentMonitor()` is required in `ToggleFullscreen()`
- [x] removed VR support
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

  - why here? unintuitive and unnecessary.

## API

#### v0.3.2.2 Cheatsheet

```c
ASCAPI void Buffer_Init();
// setup framebuffer to start drawing
ASCAPI void Buffer_Clear(Color color);
// clear framebuffer with a background color
ASCAPI void Buffer_Update();
// end drawing and swap screen buffer

ASCAPI void Events_Poll();
// poll events
ASCAPI void Events_Wait();
// wait for events
ASCAPI void Events_EndLoop();
// end the loop

ASCAPI Font Font_Load(const char *fileName);
// load a font from file into VRAM
ASCAPI Font Font_LoadEx(const char *fileName, int fontSize, int *fontChars, int glyphCount);
// load a font from file with extended parameters

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

ASCAPI int RNG_Gen(int min, int max);
// Get a random value in range [min, max]
ASCAPI void RNG_INIT(unsigned int seed);
// Set the seed of the RNG
ASCAPI void RNG_SetState(int state);
// Set the current state of the RNG
ASCAPI void RNG_GetState(void);
// Get the current state of the RNG

ASCAPI void Texture_SetFilter(Texture2D texture, int filter);
// Set texture filter, e.g. TEXTURE_FILTER_POINT, TEXTURE_FILTER_BILINEAR
ASCAPI void Texture_SetWrap(Texture2D texture, int wrap);
// Set texture wrap, e.g.

ASCAPI double Time_Get();
// get elapsed time in seconds
ASCAPI void Time_Sleep(float ms);
// halt the program for several milliseconds
ASCAPI void Time_SoftSleep(float ms);
// worse precision but less cpu usage
ASCAPI void Time_Wait(float targetFPS);
// call this at the end of a loop
ASCAPI void Time_SoftWait(float targetFPS);
// Time_Wait() that calls Time_SoftSleep() instead of Time_Sleep()

ASCAPI void Window_Init(size_t width, size_t height, const char *title);
// Initialize window and OpenGL context
ASCAPI bool Window_ShouldClose(void);
// Check if the window should close (call this only once in each loop)
ASCAPI void Window_Close(void);
// Close window and unload OpenGL context
ASCAPI bool Window_IsReady(void);
// Check if window has been initialized
ASCAPI bool Window_IsFullScreen(void);
// Check if window is currently fullscreen
ASCAPI bool Window_IsHidden(void);
// Check if window is currently hidden
ASCAPI bool Window_IsMinimized(void);
// Check if window is currently minimized
ASCAPI bool Window_IsMaximized(void);
// Check if window is currently maximized
ASCAPI bool Window_IsFocused(void);
// Check if window is currently focused
ASCAPI bool Window_IsResized(void);
// Check if window has been resized since last frame
ASCAPI bool Window_SetTitle(const char *title);
// Set the title for window
ASCAPI void Window_SetPos(int x, int y);
// Set window position
ASCAPI void Window_SetMinsize(int width, int height);
// Set window minimal dimensions
ASCAPI void Window_SetSize(size_t width, size_t height);
// Set window dimensions
ASCAPI Vector2 Window_GetPos(void);
// Get window position
```

#### Basic Example

```c
#include "src/ascede.h" // ascede!
int main(){
    Window_Init(800,600,"Basic Window");// initialize a window first to create OpenGL context
    Key_SetExitHotkey(KEY_ESCAPE);      // when Esc is pressed, the program is closed
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