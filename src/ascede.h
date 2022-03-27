/**********************************************************************************************
*
*   ALTERED
*
*   forked from raylib v4.0
*
*   FEATURES:
*       - NO external dependencies, all required libraries included with raylib
*       - Multiplatform: Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly,
*                        MacOS, Haiku, Android, Raspberry Pi, DRM native, HTML5.
*       - Written in plain C code (C99) in PascalCase/camelCase notation
*       - Hardware accelerated with OpenGL (1.1, 2.1, 3.3, 4.3 or ES2 - choose at compile)
*       - Unique OpenGL abstraction layer (usable as standalone module): [rlgl]
*       - Multiple Fonts formats supported (TTF, XNA fonts, AngelCode fonts)
*       - Outstanding texture formats support, including compressed formats (DXT, ETC, ASTC)
*       - Shaders support, including Model shaders and Postprocessing shaders
*       - Powerful math module for Vector, Matrix and Quaternion operations: [raymath]
*       - Audio loading and playing with streaming support (WAV, OGG, MP3, FLAC, XM, MOD)
*       - VR stereo rendering with configurable HMD device parameters
*       - Bindings to multiple programming languages available!
*
*   NOTES:
*       - One default Font is loaded on InitWindow()->LoadFontDefault() [core, text]
*       - One default Texture2D is loaded on rlglInit(), 1x1 white pixel R8G8B8A8 [rlgl] (OpenGL 3.3 or ES2)
*       - One default Shader is loaded on rlglInit()->rlLoadShaderDefault() [rlgl] (OpenGL 3.3 or ES2)
*       - One default RenderBatch is loaded on rlglInit()->rlLoadRenderBatch() [rlgl] (OpenGL 3.3 or ES2)
*
*   DEPENDENCIES (included):
*       [rcore] rglfw (Camilla Löwy - github.com/glfw/glfw) for window/context management and input (PLATFORM_DESKTOP)
*       [rlgl] glad (David Herberth - github.com/Dav1dde/glad) for OpenGL 3.3 extensions loading (PLATFORM_DESKTOP)
*       [raudio] miniaudio (David Reid - github.com/mackron/miniaudio) for audio device/context management
*
*   OPTIONAL DEPENDENCIES (included):
*       [rcore] msf_gif (Miles Fogle) for GIF recording
*       [rcore] sinfl (Micha Mettke) for DEFLATE decompression algorythm
*       [rcore] sdefl (Micha Mettke) for DEFLATE compression algorythm
*       [rtextures] stb_image (Sean Barret) for images loading (BMP, TGA, PNG, JPEG, HDR...)
*       [rtextures] stb_image_write (Sean Barret) for image writing (BMP, TGA, PNG, JPG)
*       [rtextures] stb_image_resize (Sean Barret) for image resizing algorithms
*       [rtext] stb_truetype (Sean Barret) for ttf fonts loading
*       [rtext] stb_rect_pack (Sean Barret) for rectangles packing
*       [raudio] dr_wav (David Reid) for WAV audio file loading
*       [raudio] dr_flac (David Reid) for FLAC audio file loading
*       [raudio] dr_mp3 (David Reid) for MP3 audio file loading
*       [raudio] stb_vorbis (Sean Barret) for OGG audio loading
*       [raudio] jar_xm (Joshua Reisenauer) for XM audio module loading
*       [raudio] jar_mod (Joshua Reisenauer) for MOD audio module loading
*
*
*   LICENSE: zlib/libpng
*
*   raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
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

#ifndef ASCEDE_H
#define ASCEDE_H
#include <stdarg.h>     // Required for: va_list - Only used by TraceLogCallback
#include <stddef.h>
#include "ascmem.h"
#define ASCEDE_VERSION  "0.3.2"
// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define ASCAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define ASCAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef ASCAPI
    #define ASCAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif

// NOTE: We set some defines with some data types declared by raylib
// Other modules (raymath, rlgl) also require some of those types, so,
// to be able to use those other modules as standalone (not depending on raylib)
// this defines are very useful for internal check and avoid type (re)definitions
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta

//----------------------------------------------------------------------------------
// Structures Definition
//----------------------------------------------------------------------------------
// Boolean type
#if defined(__STDC__) && __STDC_VERSION__ >= 199901L
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false, true } bool;
    #define RL_BOOL_TYPE
#endif

// Vector2, 2 components
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

// Vector3, 3 components
typedef struct Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} Vector3;

// Vector4, 4 components
typedef struct Vector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} Vector4;

// Quaternion, 4 components (Vector4 alias)
typedef Vector4 Quaternion;

// Matrix, 4x4 components, column major, OpenGL style, right handed
typedef struct Matrix {
    float m0, m4, m8, m12;  // Matrix first row (4 components)
    float m1, m5, m9, m13;  // Matrix second row (4 components)
    float m2, m6, m10, m14; // Matrix third row (4 components)
    float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

// Rectangle, 4 components
typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

// Image, pixel data stored in CPU memory (RAM)
typedef struct Image {
    void *data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Image;

// Texture, tex data stored in GPU memory (VRAM)
typedef struct Texture {
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Texture;

// Texture2D, same as Texture
typedef Texture Texture2D;

// RenderTexture, fbo for texture rendering
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;

// RenderTexture2D, same as RenderTexture
typedef RenderTexture RenderTexture2D;

// NPatchInfo, n-patch layout info
typedef struct NPatchInfo {
    Rectangle source;       // Texture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} NPatchInfo;

// GlyphInfo, font characters glyphs info
typedef struct GlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    Image image;            // Character image data
} GlyphInfo;

// Font, font texture and GlyphInfo array data
typedef struct Font {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    Texture2D texture;      // Texture atlas containing the glyphs
    Rectangle *recs;        // Rectangles in texture for the glyphs
    GlyphInfo *glyphs;      // Glyphs info data
} Font;

// Shader
typedef struct Shader {
    unsigned int id;        // Shader program id
    int *locs;              // Shader locations array (RL_MAX_SHADER_LOCATIONS)
} Shader;

// MaterialMap
typedef struct MaterialMap {
    Texture2D texture;      // Material map texture
    Color color;            // Material map color
    float value;            // Material map value
} MaterialMap;

// Material, includes shader and maps
typedef struct Material {
    Shader shader;          // Material shader
    MaterialMap *maps;      // Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // Material generic parameters (if required)
} Material;

// Transform, vectex transformation data
typedef struct Transform {
    Vector3 translation;    // Translation
    Quaternion rotation;    // Rotation
    Vector3 scale;          // Scale
} Transform;

// Wave, audio wave data
typedef struct Wave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} Wave;

typedef struct rAudioBuffer rAudioBuffer;

// AudioStream, custom audio stream
typedef struct AudioStream {
    rAudioBuffer *buffer;       // Pointer to internal data used by the audio system

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} AudioStream;

// Sound
typedef struct Sound {
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} Sound;

// Music, audio stream, anything longer than ~10 seconds should be streamed
typedef struct Music {
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // Music looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} Music;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} ConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} TraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE      = 39,       // Key: '
    KEY_COMMA           = 44,       // Key: ,
    KEY_MINUS           = 45,       // Key: -
    KEY_PERIOD          = 46,       // Key: .
    KEY_SLASH           = 47,       // Key: /
    KEY_ZERO            = 48,       // Key: 0
    KEY_ONE             = 49,       // Key: 1
    KEY_TWO             = 50,       // Key: 2
    KEY_THREE           = 51,       // Key: 3
    KEY_FOUR            = 52,       // Key: 4
    KEY_FIVE            = 53,       // Key: 5
    KEY_SIX             = 54,       // Key: 6
    KEY_SEVEN           = 55,       // Key: 7
    KEY_EIGHT           = 56,       // Key: 8
    KEY_NINE            = 57,       // Key: 9
    KEY_SEMICOLON       = 59,       // Key: ;
    KEY_EQUAL           = 61,       // Key: =
    KEY_A               = 65,       // Key: A | a
    KEY_B               = 66,       // Key: B | b
    KEY_C               = 67,       // Key: C | c
    KEY_D               = 68,       // Key: D | d
    KEY_E               = 69,       // Key: E | e
    KEY_F               = 70,       // Key: F | f
    KEY_G               = 71,       // Key: G | g
    KEY_H               = 72,       // Key: H | h
    KEY_I               = 73,       // Key: I | i
    KEY_J               = 74,       // Key: J | j
    KEY_K               = 75,       // Key: K | k
    KEY_L               = 76,       // Key: L | l
    KEY_M               = 77,       // Key: M | m
    KEY_N               = 78,       // Key: N | n
    KEY_O               = 79,       // Key: O | o
    KEY_P               = 80,       // Key: P | p
    KEY_Q               = 81,       // Key: Q | q
    KEY_R               = 82,       // Key: R | r
    KEY_S               = 83,       // Key: S | s
    KEY_T               = 84,       // Key: T | t
    KEY_U               = 85,       // Key: U | u
    KEY_V               = 86,       // Key: V | v
    KEY_W               = 87,       // Key: W | w
    KEY_X               = 88,       // Key: X | x
    KEY_Y               = 89,       // Key: Y | y
    KEY_Z               = 90,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,       // Key: [
    KEY_BACKSLASH       = 92,       // Key: '\'
    KEY_RIGHT_BRACKET   = 93,       // Key: ]
    KEY_GRAVE           = 96,       // Key: `
    // Function keys
    KEY_SPACE           = 32,       // Key: Space
    KEY_ESCAPE          = 256,      // Key: Esc
    KEY_ENTER           = 257,      // Key: Enter
    KEY_TAB             = 258,      // Key: Tab
    KEY_BACKSPACE       = 259,      // Key: Backspace
    KEY_INSERT          = 260,      // Key: Ins
    KEY_DELETE          = 261,      // Key: Del
    KEY_RIGHT           = 262,      // Key: Cursor right
    KEY_LEFT            = 263,      // Key: Cursor left
    KEY_DOWN            = 264,      // Key: Cursor down
    KEY_UP              = 265,      // Key: Cursor up
    KEY_PAGE_UP         = 266,      // Key: Page up
    KEY_PAGE_DOWN       = 267,      // Key: Page down
    KEY_HOME            = 268,      // Key: Home
    KEY_END             = 269,      // Key: End
    KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    KEY_NUM_LOCK        = 282,      // Key: Num lock
    KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    KEY_PAUSE           = 284,      // Key: Pause
    KEY_F1              = 290,      // Key: F1
    KEY_F2              = 291,      // Key: F2
    KEY_F3              = 292,      // Key: F3
    KEY_F4              = 293,      // Key: F4
    KEY_F5              = 294,      // Key: F5
    KEY_F6              = 295,      // Key: F6
    KEY_F7              = 296,      // Key: F7
    KEY_F8              = 297,      // Key: F8
    KEY_F9              = 298,      // Key: F9
    KEY_F10             = 299,      // Key: F10
    KEY_F11             = 300,      // Key: F11
    KEY_F12             = 301,      // Key: F12
    KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    KEY_LEFT_CONTROL    = 341,      // Key: Control left
    KEY_LEFT_ALT        = 342,      // Key: Alt left
    KEY_LEFT_SUPER      = 343,      // Key: Super left
    KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    KEY_RIGHT_ALT       = 346,      // Key: Alt right
    KEY_RIGHT_SUPER     = 347,      // Key: Super right
    KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = 320,      // Key: Keypad 0
    KEY_KP_1            = 321,      // Key: Keypad 1
    KEY_KP_2            = 322,      // Key: Keypad 2
    KEY_KP_3            = 323,      // Key: Keypad 3
    KEY_KP_4            = 324,      // Key: Keypad 4
    KEY_KP_5            = 325,      // Key: Keypad 5
    KEY_KP_6            = 326,      // Key: Keypad 6
    KEY_KP_7            = 327,      // Key: Keypad 7
    KEY_KP_8            = 328,      // Key: Keypad 8
    KEY_KP_9            = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    KEY_KP_ADD          = 334,      // Key: Keypad +
    KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = 4,        // Key: Android back button
    KEY_MENU            = 82,       // Key: Android menu button
    KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} KeyboardKey;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    MOUSE_BUTTON_LEFT    = 0,       // Mouse button left
    MOUSE_BUTTON_RIGHT   = 1,       // Mouse button right
    MOUSE_BUTTON_MIDDLE  = 2,       // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = 3,       // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = 4,       // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5,       // Mouse button fordward (advanced mouse device)
    MOUSE_BUTTON_BACK    = 6,       // Mouse button back (advanced mouse device)
} MouseButton;

// Mouse cursor
typedef enum {
    MOUSE_CURSOR_DEFAULT       = 0,     // Default pointer shape
    MOUSE_CURSOR_ARROW         = 1,     // Arrow shape
    MOUSE_CURSOR_IBEAM         = 2,     // Text writing cursor shape
    MOUSE_CURSOR_CROSSHAIR     = 3,     // Cross shape
    MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    MOUSE_CURSOR_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL    = 9,     // The omni-directional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} MouseCursor;

// Gamepad buttons
typedef enum {
    GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} GamepadButton;

// Gamepad axis
typedef enum {
    GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} GamepadAxis;

// Material map index
typedef enum {
    MATERIAL_MAP_ALBEDO    = 0,     // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    MATERIAL_MAP_METALNESS,         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    MATERIAL_MAP_NORMAL,            // Normal material
    MATERIAL_MAP_ROUGHNESS,         // Roughness material
    MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    MATERIAL_MAP_EMISSION,          // Emission material
    MATERIAL_MAP_HEIGHT,            // Heightmap material
    MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_BRDF               // Brdf material
} MaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE      MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR     MATERIAL_MAP_METALNESS

// Shader location index
typedef enum {
    SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
    SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
    SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
    SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
    SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
    SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
    SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
    SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view (camera transform)
    SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
    SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
    SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
    SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
    SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
    SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
    SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
    SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
    SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
    SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
    SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
    SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
    SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
    SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
    SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
    SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
    SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
    SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
} ShaderLocationIndex;

#define SHADER_LOC_MAP_DIFFUSE      SHADER_LOC_MAP_ALBEDO
#define SHADER_LOC_MAP_SPECULAR     SHADER_LOC_MAP_METALNESS

// Shader uniform data type
typedef enum {
    SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
    SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // Shader uniform type: int
    SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
} ShaderUniformDataType;

// Shader attribute data types
typedef enum {
    SHADER_ATTRIB_FLOAT = 0,        // Shader attribute type: float
    SHADER_ATTRIB_VEC2,             // Shader attribute type: vec2 (2 float)
    SHADER_ATTRIB_VEC3,             // Shader attribute type: vec3 (3 float)
    SHADER_ATTRIB_VEC4              // Shader attribute type: vec4 (4 float)
} ShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} PixelFormat;

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    TEXTURE_FILTER_POINT = 0,               // No filter, just pixel aproximation
    TEXTURE_FILTER_BILINEAR,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} TextureFilter;

// Texture parameters: wrap mode
typedef enum {
    TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} TextureWrap;

// Cubemap layouts
typedef enum {
    CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by an horizontal line with faces
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     // Layout is defined by a 4x3 cross with cubemap faces
    CUBEMAP_LAYOUT_PANORAMA                 // Layout is defined by a panorama image (equirectangular map)
} CubemapLayout;

// Font type, defines generation method
typedef enum {
    FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    FONT_SDF                        // SDF font generation, requires external shader
} FontType;

// Color blending modes (pre-defined)
typedef enum {
    BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    BLEND_ADDITIVE,                 // Blend textures adding colors
    BLEND_MULTIPLIED,               // Blend textures multiplying colors
    BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    BLEND_CUSTOM                    // Belnd textures using custom src/dst factors (use rlSetBlendMode())
} BlendMode;

// Gesture
// NOTE: It could be used as flags to enable only some gestures
typedef enum {
    GESTURE_NONE        = 0,        // No gesture
    GESTURE_TAP         = 1,        // Tap gesture
    GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    GESTURE_HOLD        = 4,        // Hold gesture
    GESTURE_DRAG        = 8,        // Drag gesture
    GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} Gesture;

// Camera system modes
typedef enum {
    CAMERA_CUSTOM = 0,              // Custom camera
    CAMERA_FREE,                    // Free camera
    CAMERA_ORBITAL,                 // Orbital camera
    CAMERA_FIRST_PERSON,            // First person camera
    CAMERA_THIRD_PERSON             // Third person camera
} CameraMode;

// Camera projection
typedef enum {
    CAMERA_PERSPECTIVE = 0,         // Perspective projection
    CAMERA_ORTHOGRAPHIC             // Orthographic projection
} CameraProjection;

// N-patch layout
typedef enum {
    NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} NPatchLayout;

// Callbacks to hook some internal functions
// WARNING: This callbacks are intended for advance users
typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, unsigned int *bytesRead);      // FileIO: Load binary data
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, unsigned int bytesToWrite);  // FileIO: Save binary data
typedef char *(*LoadFileTextCallback)(const char *fileName);       // FileIO: Load text data
typedef bool (*SaveFileTextCallback)(const char *fileName, char *text);     // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Window-related functions
ASCAPI void Window_Init(size_t width, size_t height, const char *title);  // Initialize window and OpenGL context
ASCAPI bool Window_ShouldClose(void);                               // Check if KEY_ESCAPE pressed or Close icon pressed
ASCAPI void Window_Close(void);                                     // Close window and unload OpenGL context
ASCAPI bool Window_IsReady(void);                                   // Check if window has been initialized successfully
ASCAPI bool Window_IsFullscreen(void);                              // Check if window is currently fullscreen
ASCAPI bool Window_IsHidden(void);                                  // Check if window is currently hidden (only PLATFORM_DESKTOP)
ASCAPI bool Window_IsMinimized(void);                               // Check if window is currently minimized (only PLATFORM_DESKTOP)
ASCAPI bool Window_IsMaximized(void);                               // Check if window is currently maximized (only PLATFORM_DESKTOP)
ASCAPI bool Window_IsFocused(void);                                 // Check if window is currently focused (only PLATFORM_DESKTOP)
ASCAPI bool Window_IsResized(void);                                 // Check if window has been resized last frame
ASCAPI bool Window_GetFlags(unsigned int flag);                      // Check if one specific window flag is enabled
ASCAPI void Window_SetFlags(unsigned int flags);                    // Set window configuration state using flags
ASCAPI void Window_PresetFlags(unsigned int flags);                    // Setup init configuration flags (view FLAGS)
ASCAPI void Window_ResetFlags(unsigned int flags);                  // Clear window configuration state flags
ASCAPI void Window_ToggleFullscreen(void);                                // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
ASCAPI void Window_Maximize(void);                                  // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
ASCAPI void Window_Minimize(void);                                  // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
ASCAPI void Window_Restore(void);                                   // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
ASCAPI void Window_SetIcon(Image image);                            // Set icon for window (only PLATFORM_DESKTOP)
ASCAPI void Window_SetTitle(const char *title);                    // Set title for window (only PLATFORM_DESKTOP)
ASCAPI void Window_SetPos(int x, int y);                           // Set window position on screen (only PLATFORM_DESKTOP)
ASCAPI void Monitor_Set(int monitor);                         // Set monitor for the current window (fullscreen mode)
ASCAPI void Window_SetMinSize(int width, int height);               // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
ASCAPI void Window_SetSize(size_t width,size_t height);            // Set window dimensions
ASCAPI void *Window_GetHandle(void);                                // Get native window handle
ASCAPI size_t Window_GetWidth(void);                                   // Get current screen width
ASCAPI size_t Window_GetHeight(void);                                  // Get current screen height
ASCAPI Vector2 Window_GetPos(void);                                // Get window position XY on monitor
ASCAPI Vector2 Window_GetDPI(void);                            // Get window scale DPI factor
ASCAPI void Clipboard_Set(const char *text);                    // Set clipboard text content
ASCAPI const char *Clipboard_Get(void);                         // Get clipboard text content
ASCAPI int Monitor_Count(void);                                  // Get number of connected monitors
ASCAPI int Monitor_Get(void);                                // Get current connected monitor
ASCAPI Vector2 Monitor_GetPos(int monitor);                    // Get specified monitor position
ASCAPI int Monitor_GetWidth(int monitor);                           // Get specified monitor width (max available by monitor)
ASCAPI int Monitor_GetHeight(int monitor);                          // Get specified monitor height (max available by monitor)
ASCAPI int Monitor_GetPhysicalWidth(int monitor);                   // Get specified monitor physical width in millimetres
ASCAPI int Monitor_GetPhysicalHeight(int monitor);                  // Get specified monitor physical height in millimetres
ASCAPI int Monitor_GetRefreshRate(int monitor);                     // Get specified monitor refresh rate
ASCAPI const char *Monitor_GetName(int monitor);                    // Get the human-readable, UTF-8 encoded name of the primary monitor

ASCAPI void Events_Poll(void);                                 // Register all input events
ASCAPI void Events_Wait(void);                                 // Register all input events
ASCAPI void Events_EndLoop(void);

// Timing-related functions
ASCAPI float Time_GetFPS(void);
ASCAPI float Time_GetFrame(void);
ASCAPI float Time_GetRealFPS(void);                                           // Get current FPS
ASCAPI float Time_GetRealFrame(void);                                   // Get time in seconds for last frame drawn (delta time)
ASCAPI double Time_Get(void);                                      // Get elapsed time in seconds since InitWindow()
ASCAPI void Time_Sleep(float ms);
ASCAPI void Time_Wait(float targetFPS);

// Cursor-related functions
ASCAPI void Cursor_Show(void);                                      // Shows cursor
ASCAPI void Cursor_Hide(void);                                      // Hides cursor
ASCAPI bool Cursor_IsHidden(void);                                  // Check if cursor is not visible
ASCAPI void Cursor_Enable(void);                                    // Enables cursor (unlock cursor)
ASCAPI void Cursor_Disable(void);                                   // Disables cursor (lock cursor)
ASCAPI bool Cursor_IsOnScreen(void);                                // Check if cursor is on the screen

// Drawing-related functions
ASCAPI void Buffer_Begin(void);                                    // Setup canvas (framebuffer) to start drawing
ASCAPI void Buffer_Clear(Color color);                          // Set background color (framebuffer clear color)
ASCAPI void Buffer_Update(void);                                // Swap back buffer with front buffer (screen drawing)
ASCAPI void RenderTexture_Begin(RenderTexture2D target);              // Begin drawing to render texture
ASCAPI void RenderTexture_Update(void);                                  // Ends drawing to render texture
ASCAPI void Shader_Begin(Shader shader);                        // Begin custom shader drawing
ASCAPI void Shader_Update(void);                                   // End custom shader drawing (use default shader)
ASCAPI void Buffer_BeginBlend(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
ASCAPI void Buffer_UpdateBlend(void);                                    // End blending mode (reset to default: alpha blending)
ASCAPI void Buffer_BeginScissor(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
ASCAPI void Buffer_EndScissor(void);                                  // End scissor mode

// Shader management functions
// NOTE: Shader functionality is not available on OpenGL 1.1
ASCAPI Shader Shader_Load(const char *vsFileName, const char *fsFileName);   // Load shader from files and bind default locations
ASCAPI Shader Shader_LoadData(const char *vsCode, const char *fsCode); // Load shader from code strings and bind default locations
ASCAPI int Shader_GetLoc(Shader shader, const char *uniformName);       // Get shader uniform location
ASCAPI int Shader_GetLocAttrib(Shader shader, const char *attribName);  // Get shader attribute location
ASCAPI void Shader_SetValue(Shader shader, int locIndex, const void *value, int uniformType);               // Set shader uniform value
ASCAPI void Shader_SetValueV(Shader shader, int locIndex, const void *value, int uniformType, int count);   // Set shader uniform value vector
ASCAPI void Shader_SetValueMatrix(Shader shader, int locIndex, Matrix mat);         // Set shader uniform value (matrix 4x4)
ASCAPI void Shader_SetValueTexture(Shader shader, int locIndex, Texture2D texture); // Set shader uniform value for texture (sampler2d)
ASCAPI void Shader_Free(Shader shader);                                    // Unload shader from GPU memory (VRAM)

// Misc. functions
ASCAPI double RNG_GenD();
ASCAPI int RNG_Gen(int min, int max);                       // Get a random value between min and max (both included)
ASCAPI void RNG_Init(unsigned int seed);                      // Set the seed for the random number generator defines format)
ASCAPI void RNG_SetState(int state);
ASCAPI int RNG_GetState();

ASCAPI void TraceLog(int logLevel, const char *text, ...);         // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
ASCAPI void TraceLog_SetLevel(int logLevel);                        // Set the current threshold (minimum) log level

// Set custom callbacks
// WARNING: Callbacks setup is intended for advance users
ASCAPI void Callback_SetTraceLog(TraceLogCallback callback);         // Set custom trace log
ASCAPI void Callback_SetLoadFileData(LoadFileDataCallback callback); // Set custom file binary data loader
ASCAPI void Callback_SetSaveFileData(SaveFileDataCallback callback); // Set custom file binary data saver
ASCAPI void Callback_SetLoadFileText(LoadFileTextCallback callback); // Set custom file text data loader
ASCAPI void Callback_SetSaveFileText(SaveFileTextCallback callback); // Set custom file text data saver

// Files management functions
ASCAPI unsigned char *File_Load(const char *fileName, unsigned int *bytesRead);     // Load file data as byte array (read)
ASCAPI void File_Free(unsigned char *data);                   // Unload file data allocated by File_Load()
ASCAPI bool File_Save(const char *fileName, void *data, unsigned int bytesToWrite); // Save data to file from byte array (write), returns true on success
ASCAPI char *File_LoadStr(const char *fileName);                   // Load text data from file (read), returns a '\0' terminated string
ASCAPI void File_FreeStr(char *text);                            // Unload file text data allocated by File_LoadStr()
ASCAPI bool File_SaveStr(const char *fileName, char *text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
ASCAPI bool File_Exists(const char *fileName);                      // Check if file exists
ASCAPI bool File_DirExists(const char *dirPath);                  // Check if a directory path exists
ASCAPI bool File_IsExt(const char *fileName, const char *ext);// Check file extension (including point: .png, .wav)
ASCAPI const char *File_GetExt(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
ASCAPI const char *File_GetName(const char *filePath);              // Get pointer to filename for a path string
ASCAPI const char *File_GetNameNX(const char *filePath);    // Get filename string without extension (uses static string)
ASCAPI const char *File_GetWorkingDir(void);                      // Get current working directory (uses static string)
ASCAPI char **File_LoadDirFileList(const char *dirPath, int *count);  // Get filenames in a directory path (memory should be freed)
ASCAPI void File_FreeDirFileList(void);                             // Clear directory files paths buffers (free memory)
ASCAPI bool File_SetWorkingDir(const char *dir);                      // Change working directory, return true on success
ASCAPI bool File_IsDropped(void);                                   // Check if a file has been dropped into window
ASCAPI char **File_GetDroppedList(int *count);                         // Get dropped files names (memory should be freed)
ASCAPI void File_ClearDroppedList(void);                               // Clear dropped files paths buffer (free memory)
ASCAPI long File_GetModTime(const char *fileName);                  // Get file modification time (last write time)

//------------------------------------------------------------------------------------
// Input Handling Functions (Module: core)
//------------------------------------------------------------------------------------

// Input-related functions: keyboard
ASCAPI bool Key_IsPressed(int key);                             // Check if a key has been pressed once
ASCAPI bool Key_IsDown(int key);                                // Check if a key is being pressed
ASCAPI bool Key_IsReleased(int key);                            // Check if a key has been released once
ASCAPI bool Key_IsUp(int key);                                  // Check if a key is NOT being pressed
ASCAPI void Key_SetExitHotkey(int key);                               // Set a custom key to exit program (default is ESC)
ASCAPI int Key_Get(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
ASCAPI int Key_GetChar(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

// Input-related functions: gamepads
ASCAPI bool Gamepad_IsAvailable(int gamepad);                   // Check if a gamepad is available
ASCAPI const char *Gamepad_GetName(int gamepad);                // Get gamepad internal name id
ASCAPI bool Gamepad_IsPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
ASCAPI bool Gamepad_IsDown(int gamepad, int button);      // Check if a gamepad button is being pressed
ASCAPI bool Gamepad_IsReleased(int gamepad, int button);  // Check if a gamepad button has been released once
ASCAPI bool Gamepad_IsUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
ASCAPI int Gamepad_GetPressed(void);                      // Get the last gamepad button pressed
ASCAPI int Gamepad_CountAxis(int gamepad);                   // Get gamepad axis count for a gamepad
ASCAPI float Gamepad_GetAxisMovment(int gamepad, int axis);    // Get axis movement value for a gamepad axis
ASCAPI int Gamepad_SetMappings(const char *mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

// Input-related functions: mouse
ASCAPI bool Mouse_IsPressed(int button);                  // Check if a mouse button has been pressed once
ASCAPI bool Mouse_IsDown(int button);                     // Check if a mouse button is being pressed
ASCAPI bool Mouse_IsReleased(int button);                 // Check if a mouse button has been released once
ASCAPI bool Mouse_IsUp(int button);                       // Check if a mouse button is NOT being pressed
ASCAPI int Mouse_GetX(void);                                    // Get mouse position X
ASCAPI int Mouse_GetY(void);                                    // Get mouse position Y
ASCAPI Vector2 Mouse_GetPos(void);                         // Get mouse position XY
ASCAPI Vector2 Mouse_GetDelta(void);                            // Get mouse delta between frames
ASCAPI void Mouse_SetPos(int x, int y);                    // Set mouse position XY
ASCAPI void Mouse_SetOffset(int offsetX, int offsetY);          // Set mouse offset
ASCAPI void Mouse_SetScale(float scaleX, float scaleY);         // Set mouse scaling
ASCAPI float Mouse_GetWheelDelta(void);                          // Get mouse wheel movement Y
ASCAPI void Mouse_SetCursor(int cursor);                        // Set mouse cursor

// Input-related functions: touch
ASCAPI int Touch_GetX(void);                                    // Get touch position X for touch point 0 (relative to screen size)
ASCAPI int Touch_GetY(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
ASCAPI Vector2 Touch_GetPos(int index);                    // Get touch position XY for a touch point index (relative to screen size)
ASCAPI int Touch_GetID(int index);                         // Get touch point identifier for given index
ASCAPI int Touch_Count(void);                           // Get number of touch points

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
ASCAPI void Shape_SetTexture(Texture2D texture, Rectangle source);       // Set texture and rectangle to be used on shapes drawing

// Basic shapes drawing functions
ASCAPI void Shape_DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);                // Draw a line
ASCAPI void Shape_DrawLineV(Vector2 startPos, Vector2 endPos, Color color);                                     // Draw a line (Vector version)
ASCAPI void Shape_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);                       // Draw a line defining thickness
ASCAPI void Shape_DrawRec(int posX, int posY, int width, int height, Color color);                        // Draw a color-filled rectangle
ASCAPI void Shape_DrawRecV(Vector2 position, Vector2 size, Color color);                                  // Draw a color-filled rectangle (Vector version)
ASCAPI void Shape_DrawRecRec(Rectangle rec, Color color);                                                 // Draw a color-filled rectangle
ASCAPI void Shape_DrawRecPro(Rectangle rec, Vector2 origin, float rotation, Color color);                 // Draw a color-filled rectangle with pro parameters
ASCAPI void Shape_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
ASCAPI void Shape_DrawTriangleStrip(Vector2 *points, int pointCount, Color color);                              // Draw a triangle strip defined by points
ASCAPI void Shape_DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);               // Draw a regular polygon (Vector version)

//------------------------------------------------------------------------------------
// Texture Loading and Drawing Functions (Module: textures)
//------------------------------------------------------------------------------------

// Image loading functions
// NOTE: This functions do not require GPU access
ASCAPI Image Image_Load(const char *fileName);                                                             // Load image from file into CPU memory (RAM)
ASCAPI Image Image_LoadRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
ASCAPI Image Image_LoadAnim(const char *fileName, int *frames);                                            // Load image sequence from file (frames appended to image.data)
ASCAPI Image Image_LoadMem(const char *fileType, const unsigned char *fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
ASCAPI Image Image_LoadFromTexture(Texture2D texture);                                                     // Load image from GPU texture data
ASCAPI Image Image_Screenshot(void);                                                                   // Load image from screen buffer and (screenshot)
ASCAPI void Image_Free(Image image);                                                                     // Unload image from CPU memory (RAM)
ASCAPI bool Image_Export(Image image, const char *fileName);                                               // Export image data to file, returns true on success
ASCAPI bool Image_ExportCode(Image image, const char *fileName);                                         // Export image as code file defining an array of bytes, returns true on success
ASCAPI Image Image_Gen(int width, int height, Color color);                                           // Generate image: plain color

// Image manipulation functions
ASCAPI Image Image_Copy(Image image);                                                                      // Create an image duplicate (useful for transformations)
ASCAPI Image Image_FromImage(Image image, Rectangle rec);                                                  // Create an image from another image piece
ASCAPI Image Image_FromText(const char *text, int fontSize, Color color);                                      // Create an image from text (default font)
ASCAPI Image Image_FromTextEx(Font font, const char *text, float fontSize, float spacing, Color tint);         // Create an image from text (custom sprite font)
ASCAPI void Image_SetFormat(Image *image, int newFormat);                                                     // Convert image data to desired format
ASCAPI void Image_ToPOT(Image *image, Color fill);                                                         // Convert image to POT (power-of-two)
ASCAPI void Image_Crop(Image *image, Rectangle crop);                                                      // Crop an image to a defined rectangle
ASCAPI void Image_AlphaCrop(Image *image, float threshold);                                                // Crop image depending on alpha value
ASCAPI void Image_AlphaClear(Image *image, Color color, float threshold);                                  // Clear alpha channel to desired color
ASCAPI void Image_AlphaMask(Image *image, Image alphaMask);                                                // Apply alpha mask to image
ASCAPI void Image_AlphaPremultiply(Image *image);                                                          // Premultiply alpha channel
ASCAPI void Image_Resize(Image *image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
ASCAPI void Image_ResizeNN(Image *image, int newWidth,int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
ASCAPI void Image_ResizeCanvas(Image *image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill);  // Resize canvas and fill with color
ASCAPI void Image_GenMipmaps(Image *image);                                                                   // Compute all mipmap levels for a provided image
ASCAPI void Image_Dither(Image *image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
ASCAPI void Image_FlipV(Image *image);                                                              // Flip image vertically
ASCAPI void Image_FlipH(Image *image);                                                            // Flip image horizontally
ASCAPI void Image_RotateCW(Image *image);                                                                  // Rotate image clockwise 90deg
ASCAPI void Image_RotateCCW(Image *image);                                                                 // Rotate image counter-clockwise 90deg
ASCAPI void Image_ColorTint(Image *image, Color color);                                                    // Modify image color: tint
ASCAPI void Image_ColorInvert(Image *image);                                                               // Modify image color: invert
ASCAPI void Image_ColorGrayscale(Image *image);                                                            // Modify image color: grayscale
ASCAPI void Image_ColorContrast(Image *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
ASCAPI void Image_ColorBrightness(Image *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
ASCAPI void Image_ColorReplace(Image *image, Color color, Color replace);                                  // Modify image color: replace color

// Image drawing functions
// NOTE: Image software-rendering functions (CPU)
ASCAPI void Image_Clear(Image *dst, Color color);                                                // Clear image background with given color
ASCAPI void Image_DrawPixel(Image *dst, int posX, int posY, Color color);                                  // Draw pixel within an image
ASCAPI void Image_DrawPixelV(Image *dst, Vector2 position, Color color);                                   // Draw pixel within an image (Vector version)
ASCAPI void Image_DrawLine(Image *dst, int startPosX, int startPosY, int endPosX, int endPosY, Color color); // Draw line within an image
ASCAPI void Image_DrawLineV(Image *dst, Vector2 start, Vector2 end, Color color);                          // Draw line within an image (Vector version)
ASCAPI void Image_DrawCircle(Image *dst, int centerX, int centerY, int radius, Color color);               // Draw circle within an image
ASCAPI void Image_DrawCircleV(Image *dst, Vector2 center, int radius, Color color);                        // Draw circle within an image (Vector version)
ASCAPI void Image_DrawRectangle(Image *dst, int posX, int posY, int width, int height, Color color);       // Draw rectangle within an image
ASCAPI void Image_DrawRectangleV(Image *dst, Vector2 position, Vector2 size, Color color);                 // Draw rectangle within an image (Vector version)
ASCAPI void Image_DrawRectangleRec(Image *dst, Rectangle rec, Color color);                                // Draw rectangle within an image
ASCAPI void Image_Draw(Image *dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);             // Draw a source image within a destination image (tint applied to source)
ASCAPI void Image_DrawText(Image *dst, const char *text, int posX, int posY, int fontSize, Color color);   // Draw text (using default font) within an image (destination)
ASCAPI void Image_DrawTextEx(Image *dst, Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text (custom sprite font) within an image (destination)

// Texture loading functions
// NOTE: These functions require GPU access
ASCAPI Texture2D Texture_Load(const char *fileName);                                                       // Load texture from file into GPU memory (VRAM)
ASCAPI Texture2D Texture_LoadFromImage(Image image);                                                       // Load texture from image data
ASCAPI RenderTexture2D RenderTexture_Load(int width, int height);                                          // Load texture for rendering (framebuffer)
ASCAPI void Texture_Free(Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
ASCAPI void RenderTexture_Free(RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)

// Texture configuration functions
ASCAPI void Texture_GenMipmaps(Texture2D *texture);                                                        // Generate GPU mipmaps for a texture
ASCAPI void Texture_SetFilter(Texture2D texture, int filter);                                              // Set texture scaling filter mode
ASCAPI void Texture_SetWrap(Texture2D texture, int wrap);                                                  // Set texture wrapping mode

// Texture drawing functions
ASCAPI void Texture_Draw(Texture2D texture, int posX, int posY, Color tint);                               // Draw a Texture2D
ASCAPI void Texture_DrawV(Texture2D texture, Vector2 position, Color tint);                                // Draw a Texture2D with position defined as Vector2
ASCAPI void Texture_DrawEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);  // Draw a Texture2D with extended parameters
ASCAPI void Texture_DrawRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);            // Draw a part of a texture defined by a rectangle
ASCAPI void Texture_DrawPro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);           // Draw a part of a texture defined by a rectangle with 'pro' parameters

// Color/pixel related functions
ASCAPI Color Color_Fade(Color color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
ASCAPI Color Color_AlphaBlend(Color dst, Color src, Color tint);              // Get src alpha-blended into dst color with tint
ASCAPI Color Color_GetPixel(void *srcPtr, int format);                        // Get Color from a source pixel pointer of certain format
ASCAPI void Color_SetPixel(void *dstPtr, Color color, int format);            // Set color formatted into destination pixel pointer
ASCAPI int Color_GetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format

//------------------------------------------------------------------------------------
// Font Loading and Text Drawing Functions (Module: text)
//------------------------------------------------------------------------------------

// Font loading/unloading functions
ASCAPI Font Font_GetDefault(void);                                                            // Get the default Font
ASCAPI Font Font_Load(const char *fileName);                                                  // Load font from file into GPU memory (VRAM)
ASCAPI Font Font_LoadEx(const char *fileName, int fontSize, int *fontChars, int glyphCount);  // Load font from file with extended parameters
ASCAPI Font Font_LoadFromImage(Image image, Color key, int firstChar);                        // Load font from Image (XNA style)
ASCAPI Font Font_LoadMem(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
ASCAPI void Font_Free(Font font);                                                           // Unload Font from GPU memory (VRAM)

// Text drawing functions
ASCAPI void Text_Draw(const char *text, int posX, int posY, int fontSize, Color color);       // Draw text (using default font)
ASCAPI void Text_DrawEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);    // Draw text using font and additional parameters
ASCAPI void Text_DrawPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint); // Draw text using Font and pro parameters (rotation)
ASCAPI void Text_DrawRec(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);
ASCAPI void Text_DrawRecEx(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
ASCAPI void Text_DrawCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint);   // Draw one character (codepoint)

// Text font info functions
ASCAPI int Text_GetWidth(const char *text, int fontSize);                                      // Measure string width for default font
ASCAPI Vector2 Text_GetWidthEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font

// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
ASCAPI bool Text_IsEqual(const char *text1, const char *text2);                               // Check if two text string are equal
ASCAPI unsigned int Text_Length(const char *text);                                            // Get text length, checks for '\0' ending
ASCAPI const char *Text_Format(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
ASCAPI const char *Text_ToLower(const char *text);                      // Get lower case version of provided string
ASCAPI int Text_GetCodepoint(const char *text, int *bytesProcessed);// Get codepoint of a UTF-8 character

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------

// Audio device management functions
RLAPI void InitAudioDevice(void);                                     // Initialize audio device and context
RLAPI void CloseAudioDevice(void);                                    // Close the audio device and context
RLAPI bool IsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
RLAPI void SetMasterVolume(float volume);                             // Set master volume (listener)

// Wave/Sound loading/unloading functions
RLAPI Wave LoadWave(const char *fileName);                            // Load wave data from file
RLAPI Wave LoadWaveFromMemory(const char *fileType, const unsigned char *fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
RLAPI Sound LoadSound(const char *fileName);                          // Load sound from file
RLAPI Sound LoadSoundFromWave(Wave wave);                             // Load sound from wave data
RLAPI void UpdateSound(Sound sound, const void *data, int sampleCount); // Update sound buffer with new data
RLAPI void UnloadWave(Wave wave);                                     // Unload wave data
RLAPI void UnloadSound(Sound sound);                                  // Unload sound
RLAPI bool ExportWave(Wave wave, const char *fileName);               // Export wave data to file, returns true on success
RLAPI bool ExportWaveAsCode(Wave wave, const char *fileName);         // Export wave sample data to code (.h), returns true on success

// Wave/Sound management functions
RLAPI void PlaySound(Sound sound);                                    // Play a sound
RLAPI void StopSound(Sound sound);                                    // Stop playing a sound
RLAPI void PauseSound(Sound sound);                                   // Pause a sound
RLAPI void ResumeSound(Sound sound);                                  // Resume a paused sound
RLAPI void PlaySoundMulti(Sound sound);                               // Play a sound (using multichannel buffer pool)
RLAPI void StopSoundMulti(void);                                      // Stop any sound playing (using multichannel buffer pool)
RLAPI int GetSoundsPlaying(void);                                     // Get number of sounds playing in the multichannel
RLAPI bool IsSoundPlaying(Sound sound);                               // Check if a sound is currently playing
RLAPI void SetSoundVolume(Sound sound, float volume);                 // Set volume for a sound (1.0 is max level)
RLAPI void SetSoundPitch(Sound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
RLAPI void WaveFormat(Wave *wave, int sampleRate, int sampleSize, int channels);  // Convert wave data to desired format
RLAPI Wave WaveCopy(Wave wave);                                       // Copy a wave to a new wave
RLAPI void WaveCrop(Wave *wave, int initSample, int finalSample);     // Crop a wave to defined samples range
RLAPI float *LoadWaveSamples(Wave wave);                              // Load samples data from wave as a floats array
RLAPI void UnloadWaveSamples(float *samples);                         // Unload samples data loaded with LoadWaveSamples()

// Music management functions
RLAPI Music LoadMusicStream(const char *fileName);                    // Load music stream from file
RLAPI Music LoadMusicStreamFromMemory(const char *fileType, unsigned char *data, int dataSize); // Load music stream from data
RLAPI void UnloadMusicStream(Music music);                            // Unload music stream
RLAPI void PlayMusicStream(Music music);                              // Start music playing
RLAPI bool IsMusicStreamPlaying(Music music);                         // Check if music is playing
RLAPI void UpdateMusicStream(Music music);                            // Updates buffers for music streaming
RLAPI void StopMusicStream(Music music);                              // Stop music playing
RLAPI void PauseMusicStream(Music music);                             // Pause music playing
RLAPI void ResumeMusicStream(Music music);                            // Resume playing paused music
RLAPI void SeekMusicStream(Music music, float position);              // Seek music to a position (in seconds)
RLAPI void SetMusicVolume(Music music, float volume);                 // Set volume for music (1.0 is max level)
RLAPI void SetMusicPitch(Music music, float pitch);                   // Set pitch for a music (1.0 is base level)
RLAPI float GetMusicTimeLength(Music music);                          // Get music time length (in seconds)
RLAPI float GetMusicTimePlayed(Music music);                          // Get current music time played (in seconds)

// AudioStream management functions
RLAPI AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
RLAPI void UnloadAudioStream(AudioStream stream);                      // Unload audio stream and free memory
RLAPI void UpdateAudioStream(AudioStream stream, const void *data, int frameCount); // Update audio stream buffers with data
RLAPI bool IsAudioStreamProcessed(AudioStream stream);                // Check if any audio stream buffers requires refill
RLAPI void PlayAudioStream(AudioStream stream);                       // Play audio stream
RLAPI void PauseAudioStream(AudioStream stream);                      // Pause audio stream
RLAPI void ResumeAudioStream(AudioStream stream);                     // Resume audio stream
RLAPI bool IsAudioStreamPlaying(AudioStream stream);                  // Check if audio stream is playing
RLAPI void StopAudioStream(AudioStream stream);                       // Stop audio stream
RLAPI void SetAudioStreamVolume(AudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
RLAPI void SetAudioStreamPitch(AudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
RLAPI void SetAudioStreamBufferSizeDefault(int size);                 // Default size for new audio streams

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_H
