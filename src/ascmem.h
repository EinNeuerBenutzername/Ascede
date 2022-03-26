#ifndef MEMORY_MANAGEMENT_FUNCTIONS_DEFINED
#define MEMORY_MANAGEMENT_FUNCTIONS_DEFINED
void ASC_ERROR();
void *ASC_MALLOC(size_t bytes);
void *ASC_CALLOC(size_t num, size_t bytes);
void *ASC_REALLOC(void *pointer,size_t bytes);
void ASC_FREE(void *pointer);
void ASC_FREEALL();
#endif
