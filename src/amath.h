#ifndef ASCEDE_MATH
#define ASCEDE_MATH
#include <limits.h>
#include "ascede.h"
#include "external/mtwister.h"
struct{
    MTRand mtr;
} amath={0};
void     RNG_Init(unum seed){
    amath.mtr=seedRand(seed);
}
unum      RNG_Gen(){
    return genRand(&amath.mtr);
}
void     RNG_SetState(num state){
    amath.mtr.index=state;
}
num      RNG_GetState(){
    return amath.mtr.index;
}

num      Ceil(real a){
    if(a>=INT_MAX)return 0;
    if(a<=INT_MIN)return 0;
    num n=a;
    if(n<a)n++;
}
num      Floor(real a){
    if(a>=INT_MAX)return 0;
    if(a<=INT_MIN)return 0;
    num n=a;
    if(n>a)n--;
}
num      Round(real a){
    if(a>=INT_MAX)return 0;
    if(a<=INT_MIN)return 0;
    num c=a,f=a;
    if(c<a)c++;
    if(f>a)f--;
    return a-f<c-a?f:c;
}

#endif
