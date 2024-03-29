#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "rectangle.h"
#include "vector.h"

struct SpatialGrid {
    size_t width, height, width_div, height_div;
    struct Vector entries; // List of all rects
    struct Vector found; // Duplicate check vector 
    struct Vector buckets[]; // This holds a list of indexes that you can lookup later
};

struct SpatialGrid* spatialgrid_new(size_t width, size_t height, size_t width_div, size_t height_div);
void spatialgrid_insert(struct SpatialGrid* sg, struct Rectangle* rect);
void spatialgrid_fetch(struct SpatialGrid* sg, struct Rectangle* rect, struct Vector* vector);
void spatialgrid_delete(struct SpatialGrid* sg);
