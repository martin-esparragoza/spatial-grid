#pragma once
#include <stddef.h>
#include <stdbool.h>

struct Vector {
    void* data;
    size_t allocated, length, element_size;
};

bool vector_new(struct Vector* v, size_t initial_capacity, size_t element_size);
void* vector_get(struct Vector* v, size_t index);
void* vector_get_sf(struct Vector* v, size_t index);
bool vector_resize(struct Vector* v, size_t size);
bool vector_push_forward(struct Vector* v, void* element);
void* vector_add(struct Vector* v);