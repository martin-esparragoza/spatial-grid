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
    struct SpatialHashgridEntry* entry = NULL;
    
    vector_new(&shg->entries, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle*));
    vector_new(&shg->found, VECTOR_DEFAULT_CAPACITY, sizeof(bool));
    for (size_t i = 0; i < shg->width_div * shg->height_div; i++) {
        vector_new(&shg->buckets[i], VECTOR_DEFAULT_CAPACITY, sizeof(size_t));
    }

    return shg;
}

// From position get index in array
static inline size_t spatialhashgrid_get(struct SpatialHashgrid* shg, real_t x, real_t y);
static inline size_t spatialhashgrid_get(struct SpatialHashgrid* shg, real_t x, real_t y) { 
    return 
        ((size_t) (x / ((real_t) shg->width / shg->width_div))) +
        ((size_t) (y / ((real_t) shg->height / shg->height_div)) * shg->width_div);
}

/**
 * Insert a rectangle into the SHG.
 */
void spatialhashgrid_insert(struct SpatialHashgrid* shg, struct Rectangle* rect) {
    vector_push_forward(&shg->entries, &rect);
    bool* found = vector_add(&shg->found);
    *found = false;

    // The most evil way to do new_index -- if > 0
    size_t ctz = __builtin_ctz(shg->entries.length);
    size_t new_index = shg->entries.length - ((shg->entries.length & (1 << ctz)) >> ctz);

    size_t i1 = spatialhashgrid_get(shg, rect->x, rect->y);
    size_t i2 = spatialhashgrid_get(shg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / ((real_t) shg->width / shg->width_div));

    while (i1 <= i2) {
        // Assign bucket index values
        vector_push_forward(&shg->buckets[i1], &new_index);

        // TODO: THIS CAN BE BRANCHLESS :))))))))))))))))))) (maybe)
        if (i2 - i1 != 0 && (i2 - i1) % width == 0) {
            i1 += shg->width_div - width + 1;
        } else {
            i1++;
        }
    }
}

/**
 * Get a list of all rectangle* that collide with the given rect.
 * @param vector Assumes that the vector is already initialized and of length 0 <b>Vector is of type struct Rectangle*</b>
 */
void spatialhashgrid_fetch(struct SpatialHashgrid* shg, struct Rectangle* rect, struct Vector* vector) {
    size_t i1 = spatialhashgrid_get(shg, rect->x, rect->y);
    size_t i2 = spatialhashgrid_get(shg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / (shg->width / shg->width_div));
    // I generally don't like this way of doing this but it works
    bool* found_ptrs[shg->entries.length];
    size_t found_ptrs_length = 0;

    while (i1 <= i2) {
        for (size_t i = 0; i < shg->buckets[i1].length; i++) {
            size_t* index = vector_get(&shg->buckets[i1], i);
            bool* found = vector_get(&shg->found, *index); // This makes sure that there are 0 duplicates
            if (!(*found)) {
                // vector_get gives a pointer to the pointer to a rectangle (a bit confusing)
                vector_push_forward(vector, vector_get(&shg->entries, *index));
                *found = true;
                found_ptrs[found_ptrs_length++] = found;
            }
        }

        // TODO: THIS CAN BE BRANCHLESS :))))))))))))))))))) (maybe)
        if (i2 - i1 != 0 && (i2 - i1) % width == 0) {
            i1 += shg->width_div - width + 1;
        } else {
            i1++;
        }
    }

    // Clean up found flags FIXME this is pretty scuffed :(
    for (size_t i = 0; i < found_ptrs_length; i++) {
        *found_ptrs[i] = false;
    }
}

/**
 * Deallocs everything
 */
void spatialhashgrid_delete(struct SpatialHashgrid* shg) {
    vector_delete(&shg->entries);
    vector_delete(&shg->found);
    for (size_t i = 0; i < shg->width_div * shg->height_div; i++) {
        vector_delete(&shg->buckets[i]);
    }
}
