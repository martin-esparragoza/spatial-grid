#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "rectangle.h"
#include "vector.h"

struct SpatialHashgrid {
    size_t width, height, width_div, height_div;
    struct Vector* entries;
    struct Vector found; // Duplicate check vector 
    struct Vector buckets[]; // This holds a list of indexes that you can lookup later
};

struct SpatialHashgridEntry {
    struct Rectangle* rect;
    bool found;
};

struct SpatialHashgrid* spatialhashgrid_new(size_t width, size_t height, size_t width_div, size_t height_div);
void spatialhashgrid_insert(struct SpatialHashgrid* shg, struct Rectangle* rect);
struct Vector spatialhashgrid_fetch(struct SpatialHashgrid* shg, struct Rectangle* rect);
void spatialhashgrid_delete(struct SpatialHashgrid* shg);