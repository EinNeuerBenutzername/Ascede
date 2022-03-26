#include <stdlib.h>
#include "ascmem.h"
void *pointerpool[128]={0};
unsigned int pointerinuse=0;
void ASC_ERROR(){
    exit(-1);
}
void *ASC_MALLOC(size_t bytes){
    if(pointerinuse>128){
        ASC_ERROR();
        return NULL;
    }
    void *pointer=NULL;
    pointer=malloc(bytes);
    if(pointer==NULL){
        ASC_ERROR();
        return NULL;
    }
    pointerinuse++;
    for(int i=0;i<128;i++){
        if(pointerpool[i]==NULL){
            pointerpool[i]=pointer;
            break;
        }
    }
    return pointer;
}
void *ASC_CALLOC(size_t num, size_t bytes){
    if(pointerinuse>128){
        ASC_ERROR();
        return NULL;
    }
    void *pointer=NULL;
    pointer=calloc(num,bytes);
    if(pointer==NULL){
        ASC_ERROR();
        return NULL;
    }
    pointerinuse++;
    for(int i=0;i<128;i++){
        if(pointerpool[i]==NULL){
            pointerpool[i]=pointer;
            break;
        }
    }
    return pointer;
}
void *ASC_REALLOC(void *pointer,size_t bytes){
    for(int i=0,found=0;i<128;i++){
        if(pointerpool[i]==pointer){
            found=1;
            break;
        }
        if(i==127&&found==0){
            ASC_ERROR();
            return NULL;
        }
    }
    pointer=realloc(pointer,bytes);
    if(pointer==NULL)ASC_ERROR();
    return pointer;
}
void ASC_FREE(void *pointer){
    if(pointer==NULL)return;
    for(int i=0;i<128;i++){
        if(pointerpool[i]==pointer){
            pointerpool[i]=NULL;
            break;
        }
        if(i==127){
            ASC_ERROR();
            return;
        }
    }
    free(pointer);
    pointerinuse--;
    pointer=NULL;
}
void ASC_FREEALL(){
    for(int i=0;i<128&&pointerinuse>0;i++){
        if(pointerpool[i]!=NULL){
            free(pointerpool[i]);
            pointerinuse--;
        }
    }
}
