#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../include/vector.h"
#include "../include/spatialgrid.h"
#include "../include/extern.h"
#include "../include/def.h"

/**
 * Sets and allocates everything to make a new grid.
 * This is allocated on the heap exclusively.
 * @param width_div Number of buckets across the x axis
 * @param height_div Number of buckets across the y axis 
 * @param entries Vector of Rectangles. Should already be initialized.
 */
struct SpatialGrid* spatialgrid_new(size_t width, size_t height, size_t width_div, size_t height_div) {
    struct SpatialGrid* sg = malloc(sizeof(struct SpatialGrid) + width_div * height_div * sizeof(struct Vector));
    sg->width = width;
    sg->height = height;
    sg->width_div = width_div;
    sg->height_div = height_div;
    
    vector_new(&sg->entries, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle*));
    vector_new(&sg->found, VECTOR_DEFAULT_CAPACITY, sizeof(bool));
    for (size_t i = 0; i < sg->width_div * sg->height_div; i++) {
        vector_new(&sg->buckets[i], VECTOR_DEFAULT_CAPACITY, sizeof(size_t));
    }

    return sg;
}

// From position get index in array
static inline size_t spatialgrid_get(struct SpatialGrid* sg, real_t x, real_t y);
static inline size_t spatialgrid_get(struct SpatialGrid* sg, real_t x, real_t y) { 
    return 
        ((size_t) (x / ((real_t) sg->width / sg->width_div))) +
        ((size_t) (y / ((real_t) sg->height / sg->height_div)) * sg->width_div);
}

/**
 * Insert a rectangle into the sg.
 */
void spatialgrid_insert(struct SpatialGrid* sg, struct Rectangle* rect) {
    vector_push_forward(&sg->entries, &rect);
    bool* found = vector_add(&sg->found);
    *found = false;

    // The most evil way to do new_index -- if > 0
    size_t ctz = __builtin_ctz(sg->entries.length);
    size_t new_index = sg->entries.length - ((sg->entries.length & (1 << ctz)) >> ctz);

    size_t i1 = spatialgrid_get(sg, rect->x, rect->y);
    size_t i2 = spatialgrid_get(sg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / ((real_t) sg->width / sg->width_div));

    while (i1 <= i2) {
        // Assign bucket index values
        vector_push_forward(&sg->buckets[i1], &new_index);

        // TODO: THIS CAN BE BRANCHLESS :))))))))))))))))))) (maybe)
        if (i2 - i1 != 0 && (i2 - i1) % width == 0) {
            i1 += sg->width_div - width + 1;
        } else {
            i1++;
        }
    }
}

/**
 * Get a list of all rectangle* that collide with the given rect.
 * @param vector Assumes that the vector is already initialized and of length 0 <b>Vector is of type struct Rectangle*</b>
 */
void spatialgrid_fetch(struct SpatialGrid* sg, struct Rectangle* rect, struct Vector* vector) {
    size_t i1 = spatialgrid_get(sg, rect->x, rect->y);
    size_t i2 = spatialgrid_get(sg, rect->x + rect->width, rect->y + rect->height);
    size_t width = ceil(rect->width / (sg->width / sg->width_div));
    // I generally don't like this way of doing this but it works
    bool* found_ptrs[sg->entries.length];
    size_t found_ptrs_length = 0;

    while (i1 <= i2) {
        for (size_t i = 0; i < sg->buckets[i1].length; i++) {
            size_t* index = vector_get(&sg->buckets[i1], i);
            bool* found = vector_get(&sg->found, *index); // This makes sure that there are 0 duplicates
            if (!(*found)) {
                // vector_get gives a pointer to the pointer to a rectangle (a bit confusing)
                vector_push_forward(vector, vector_get(&sg->entries, *index));
                *found = true;
                found_ptrs[found_ptrs_length++] = found;
            }
        }

        // TODO: THIS CAN BE BRANCHLESS :))))))))))))))))))) (maybe)
        if (i2 - i1 != 0 && (i2 - i1) % width == 0) {
            i1 += sg->width_div - width + 1;
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
void spatialgrid_delete(struct SpatialGrid* sg) {
    vector_delete(&sg->entries);
    vector_delete(&sg->found);
    for (size_t i = 0; i < sg->width_div * sg->height_div; i++) {
        vector_delete(&sg->buckets[i]);
    }
}
