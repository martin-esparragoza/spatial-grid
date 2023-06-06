#include <stddef.h>
#include <stdbool.h>
#include "../include/vector.h"
#include "../include/extern.h"

/**
 * Create a new vector object.
 * @param initial_capacity Starting length of your vector (elements)
 * @param element_size Size of each element (bytes).
 * @return true if created false if not.
 */
bool vector_new(struct Vector* v, size_t initial_capacity, size_t element_size) {
    v->data = malloc(initial_capacity * element_size);
    if (!v->data) {
        return false;
    }

    v->allocated = initial_capacity * element_size;
    v->element_size = element_size;
    v->length = 0;
    return true;
}

/**
 * Get element from vector. For those who want to squeeze out as much performance as possible.
 * @return Location of element. Not checked if the returned address is in the range or not.
 */
void* vector_get(struct Vector* v, size_t index) {
    return v->data + (index * v->element_size);
}

/**
 * Get element from vector. Checks if in range.
 * @return Location of element. NULL if unfound.
 */
void* vector_get_sf(struct Vector* v, size_t index) {
    if (index >= v->length) {
        return NULL;
    }
    return v->data + (index * v->element_size);
}

/**
 * Resize a vector to a desired size.
 * @param size Number of bytes to make the vector length.
 * @return true if successful and false if not.
 */
bool vector_resize(struct Vector* v, size_t size) {
    v->data = realloc(v->data, size * v->element_size);
    v->allocated = size * v->element_size;
    if (!v->data) {
        return false;
    }
    return true;
}

/**
 * Adds an element to the vector. Resizes when needed.
 * @param element Element to add to the vector.
 */
bool vector_push_forward(struct Vector* v, void* element) {
    if (v->length >= v->allocated / v->element_size) {
        if (!vector_resize(v, v->allocated * 2)) {
            return false;
        }
    }

    if (!memcpy(v->data + v->length++ * v->element_size, element, v->element_size)) {
        return false;
    }
    return true;
}

/**
 * vector_push_forward and vector_add are distictly different.
 * Gives pointer to allocated element in vector. Effectiveley
 * gives last element (allocs kind of).
 */
void* vector_add(struct Vector* v) {
    if (v->length >= v->allocated / v->element_size) {
        if (!vector_resize(v, v->allocated * 2)) {
            return NULL;
        }
    }

    return v->data + v->length++ * v->element_size;
}