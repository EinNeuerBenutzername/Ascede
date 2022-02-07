#ifndef ASCEDE_H
#define ASCEDE_H

#include <inttypes.h>

typedef int_fast32_t num;
typedef uint_fast32_t unum;
typedef float real;

#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define ASCAPI __declspec(dllexport)
    #elif defined(USE_LIBTYPE_SHARED)
        #define ASCAPI __declspec(dllimport)
    #endif
#endif
#ifndef ASCAPI
    #define ASCAPI
#endif
#if defined(__STDC__)&&__STDC_VERSION__>=199901L
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    #error __STDC_VERSION__ < 199901L
    typedef enum bool{false,true}bool;
#endif
#if defined(__cplusplus)
extern "C" {
#endif

/****************************
***********STRUCTS***********
****************************/
typedef struct Sample{} Sample;
typedef struct Font{} Font;
typedef struct Texture{} Texture;
/****************************
************AUDIO************
****************************/
ASCAPI void     Audio_Init();
ASCAPI void     Audio_Close();

ASCAPI Sample  *Sample_Load(const char *path);
ASCAPI Sample  *Sample_LoadFromMemory(void *data);
ASCAPI void     Sample_Free(Sample *sample);
/****************************
*************CORE************
****************************/
ASCAPI void     Buffer_Init();
ASCAPI void     Buffer_Clear();
ASCAPI void     Buffer_Draw();

ASCAPI void     Event_Poll();
ASCAPI void     Event_Wait();
ASCAPI void     Event_EndLoop();

ASCAPI bool     Key_IsUp(enum e_Key key);
ASCAPI bool     Key_IsDown(enum e_Key key);
ASCAPI bool     Key_IsPressed(enum e_Key key);
ASCAPI bool     Key_IsReleased(enum e_Key key);

ASCAPI void     Mouse_GetPosition(num *x,num *y);
ASCAPI bool     Mouse_IsUp(enum e_Mouse_Buttton mb);
ASCAPI bool     Mouse_IsDown(enum e_Mouse_Buttton mb);
ASCAPI bool     Mouse_IsPressed(enum e_Mouse_Buttton mb);
ASCAPI bool     Mouse_IsReleased(enum e_Mouse_Buttton mb);

ASCAPI Texture *Image_Load(const char *path);
ASCAPI Texture *Image_LoadFromMemory(void *data);
ASCAPI void     Image_Free(Texture *texture);
ASCAPI void     Image_Draw(num x,num y,num scale,real rad);

ASCAPI void     Time_Init();
ASCAPI void     Time_Sleep(unum ms);
ASCAPI real     Time_Get();
ASCAPI void     Time_SetTargetFPS(real fps);
ASCAPI real     Time_GetTargetFPS();
ASCAPI real     Time_Wait();

ASCAPI void     Window_Init(size_t width,size_t height,const char *title);
ASCAPI bool     Window_ShouldClose();
ASCAPI void     Window_Update();
ASCAPI void     Window_Close();
ASCAPI void     Window_SetSize(size_t width,size_t height);
ASCAPI void     Window_GetSize(size_t *width,size_t *height);
ASCAPI void     Window_SetPos(size_t x,size_t y);
ASCAPI void     Window_GetPos(size_t *x,size_t *y);
/****************************
*************MATH************
****************************/
ASCAPI void     RNG_Init(unum seed);
ASCAPI unum     RNG_Gen();
ASCAPI void     RNG_SetState(num state);
ASCAPI num      RNG_GetState();

ASCAPI num      Ceil(real a);
ASCAPI num      Floor(real a);
ASCAPI num      Round(real a);
/****************************
*************TEXT************
****************************/
ASCAPI Font    *Font_Load(const char *path);
ASCAPI Font    *Font_LoadFromMemory(void *data,size_t len);
ASCAPI Font     Font_Free(Font *font);
ASCAPI          Text_Print(Font *font,const char *content,num x,num y,num scale);

#if defined(__cplusplus)
}
#endif
#endif
