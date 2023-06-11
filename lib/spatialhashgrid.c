#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../include/vector.h"
#include "../include/spatialhashgrid.h"
#include "../include/extern.h"
#include "../include/def.h"

/**
 * Sets and allocates everything to make a new hashgrid.
 * This is allocated on the heap exclusively.
 * @param width_div Number of buckets across the x axis
 * @param height_div Number of buckets across the y axis 
 * @param entries Vector of Rectangles. Should already be initialized.
 */
struct SpatialHashgrid* spatialhashgrid_new(size_t width, size_t height, size_t width_div, size_t height_div) {
    struct SpatialHashgrid* shg = malloc(sizeof(struct SpatialHashgrid) + width_div * height_div * sizeof(struct Vector));
    shg->width = width;
    shg->height = height;
    shg->width_div = width_div;
    shg->height_div = height_div;
    
    for (size_t i = 0; i < shg->width_div * shg->height_div; i++) {
        vector_new(&shg->buckets[i], VECTOR_DEFAULT_CAPACITY, sizeof(struct SpatialHashgridEntry*));
    }

    return shg;
}

// From position get index in array
static inline size_t spatialhashgrid_get(struct SpatialHashgrid* shg, real_t x, real_t y);
static inline size_t spatialhashgrid_get(struct SpatialHashgrid* shg, real_t x, real_t y) { 
    return 
        ((size_t) (x / (shg->width / shg->width_div))) +
        ((size_t) (y / (shg->height / shg->height_div)) * shg->width_div);
}

#include <stdio.h>
void spatialhashgrid_insert(struct SpatialHashgrid* shg, struct Rectangle* rect) {
    struct SpatialHashgridEntry* entry = (struct SpatialHashgridEntry*) malloc(sizeof(struct SpatialHashgridEntry));
    entry->rect = rect;
    entry->found = false;

    // Assign buckets
    size_t i1 = spatialhashgrid_get(shg, rect->x, rect->y);
    size_t i2 = spatialhashgrid_get(shg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / (shg->width / shg->width_div));

    // TODO I hate this method of doing things
    size_t i = 0;
    while (i1 <= i2) {
        vector_push_forward(&shg->buckets[i1], &entry);

        // Terrible
        if (i != 0 && i % width == 0) {
            i1 += shg->width_div - width;
            i = 0;
        } else {
            i1++;
            i++;
        }
    }

    return;
}

/**
 * Get all entities in this rectangle
 * @return Vector of SpatialHashgridEntry*.
 */
struct Vector spatialhashgrid_fetch(struct SpatialHashgrid* shg, struct Rectangle* rect) {
    struct Vector ret;
    vector_new(&ret, VECTOR_DEFAULT_CAPACITY, sizeof(struct SpatialHashgridEntry*));

    // Assign buckets
    size_t i1 = spatialhashgrid_get(shg, rect->x, rect->y);
    size_t i2 = spatialhashgrid_get(shg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / (shg->width / shg->width_div));

    // TODO I hate this method of doing things
    size_t i = 0;
    while (i1 <= i2) {
        for (size_t j = 0; j < shg->buckets[i1].length; j++) {
            struct SpatialHashgridEntry* entry = *(struct SpatialHashgridEntry**) vector_get(&shg->buckets[i1], j);
            if (!entry->found) {
                vector_push_forward(&ret, entry);
                entry->found = false;
            }
        }

        // Terrible
        if (i != 0 && i % width == 0) {
            i1 += shg->width_div - width;
            i = 0;
        } else {
            i1++;
            i++;
        }
    }

    // Switch back the found flags
    for (size_t j = 0; j < ret.length; j++) {
        ((struct SpatialHashgridEntry*) vector_get(&ret, j))->found = false;
    }

    return ret;
}
//void spatialhashgrid_delete(struct SpatialHashgrid* shg);