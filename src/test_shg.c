#include "../include/spatialhashgrid.h"
#include "../include/rectangle.h"
#include "../include/vector.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define WIDTH 1000000
#define HEIGHT 1000000
#define CELL_W 12
#define CELL_H 12
#define NUM_ENTS 500000
#define NUM_FETCHES 500

struct SpatialHashgrid* shg;
struct Vector entries;
struct timespec before;
struct timespec after;
struct Vector entries_fetch;
double avg = 0;

int main() {
    srand(time(NULL));
    vector_new(&entries, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle));
    shg = spatialhashgrid_new(WIDTH, HEIGHT, CELL_W, CELL_H);
    vector_new(&entries_fetch, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle*));

    printf("Spatial hashgrid of %dx%d and %dx%d cells\n", WIDTH, HEIGHT, CELL_W, CELL_H);

    printf("Attempting to insert %d entities\n", NUM_ENTS);

    clock_gettime(CLOCK_MONOTONIC, &before);
    for (int i = 0; i < NUM_ENTS; i++) {
        struct Rectangle* rect = (struct Rectangle*) vector_add(&entries);
        rect->x = rand() % (WIDTH / 4) + 1;
        rect->y = rand() % (HEIGHT / 4) + 1;
        rect->width = rand() % (WIDTH / 4) + 1;
        rect->height = rand() % (HEIGHT / 4) + 1;

        spatialhashgrid_insert(shg, rect);
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    printf("That took %ldns\n", after.tv_nsec - before.tv_nsec);

    printf("Attempting to do %d fetches\n", NUM_FETCHES);
    for (int i = 0; i < NUM_FETCHES; i++) {
        struct Rectangle rect = {
            rand() % (WIDTH / 4) + 1,
            rand() % (HEIGHT / 4) + 1,
            rand() % (WIDTH / 4) + 1,
            rand() % (HEIGHT / 4) + 1
        };

        clock_gettime(CLOCK_MONOTONIC, &before);
        spatialhashgrid_fetch(shg, &rect, &entries_fetch);
        clock_gettime(CLOCK_MONOTONIC, &after);
        // Stack overflow yoink
        double time_delta = ((double)after.tv_sec + 1.0e-9*after.tv_nsec) - ((double)before.tv_sec + 1.0e-9*before.tv_nsec);
        printf("That took %fs and got %ld entities\n", time_delta, entries_fetch.length);
        vector_clear(&entries_fetch);
        avg += time_delta;
    }

    vector_delete(&entries_fetch);
    vector_delete(&entries);
    printf("Average of %fs\n", avg / NUM_FETCHES);
}
