#pragma once
#include <stddef.h>

extern void* malloc(size_t size);
extern void* realloc(void* ptr, size_t size);
extern void* memcpy(void* to, const void* from, size_t num_bytes);
extern void free(void* ptr);
