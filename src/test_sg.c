#include "../include/spatialgrid.h"
#include "../include/rectangle.h"
#include "../include/vector.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define WIDTH 500000
#define HEIGHT 500000
#define CELL_W 1024
#define CELL_H 1024
#define ENT_WIDTH 6
#define ENT_HEIGHT 6
#define NUM_ENTS 500000

struct SpatialGrid* sg;
struct Vector entries;
struct timespec before;
struct timespec after;
struct Vector entries_fetch;
double avg = 0;
unsigned long total_ents = 0;

int main() {
    srand(1000);
    vector_new(&entries, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle));
    sg = spatialgrid_new(WIDTH, HEIGHT, CELL_W, CELL_H);
    vector_new(&entries_fetch, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle*));

    printf("Spatial grid of %dx%d and %dx%d cells\n", WIDTH, HEIGHT, CELL_W, CELL_H);

    printf("Attempting to insert %d entities\n", NUM_ENTS);

    clock_gettime(CLOCK_MONOTONIC, &before);
    for (int i = 0; i < NUM_ENTS; i++) {
        struct Rectangle* rect = vector_add(&entries);
        rect->x = rand() % (WIDTH - ENT_WIDTH);
        rect->y = rand() % (HEIGHT - ENT_HEIGHT);
        rect->width = ENT_WIDTH;
        rect->height = ENT_WIDTH;

        spatialgrid_insert(sg, rect);
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    printf("That took %fs\n", ((double)after.tv_sec + 1.0e-9*after.tv_nsec) - ((double)before.tv_sec + 1.0e-9*before.tv_nsec));

    printf("Attempting to do fetches\n");
    for (unsigned i = 0; i < entries.length; i++) {
        struct Rectangle* rect = vector_get(&entries, i);

        clock_gettime(CLOCK_MONOTONIC, &before);
        spatialgrid_fetch(sg, rect, &entries_fetch);
        clock_gettime(CLOCK_MONOTONIC, &after);
        // Stack overflow lol
        double time_delta = ((double)after.tv_sec + 1.0e-9*after.tv_nsec) - ((double)before.tv_sec + 1.0e-9*before.tv_nsec);
        total_ents += entries_fetch.length;
        //printf("That took %fs and got %ld entities\n", time_delta, entries_fetch.length);
        vector_clear(&entries_fetch);
        avg += time_delta;
    }

    printf("Average of %fs\n", avg / entries.length);
    printf("Total time: %fs\n", avg);
    printf("%ld total ents\n", total_ents);
    vector_delete(&entries_fetch);
    vector_delete(&entries);
    spatialgrid_delete(sg);
    free(sg);
}
