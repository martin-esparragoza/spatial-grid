#pragma once

extern void* malloc(size_t size);
extern void* realloc(void* ptr, size_t size);
extern void* memcpy(void* to, const void* from, size_t numBytes);