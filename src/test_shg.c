#include "../include/spatialhashgrid.h"
#include "../include/rectangle.h"
#include "../include/vector.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define WIDTH 500000
#define HEIGHT 500000
#define CELL_W 12
#define CELL_H 12
#define NUM_ENTS 1000000
#define NUM_FETCHES 1000

struct SpatialHashgrid* shg;
struct Vector entries;
struct timespec before;
struct timespec after;
long avg = 0;

int main() {
    srand(time(NULL));
    vector_new(&entries, VECTOR_DEFAULT_CAPACITY, sizeof(struct Rectangle));
    shg = spatialhashgrid_new(WIDTH, HEIGHT, CELL_W, CELL_H);
    printf("Spatial hashgrid of %dx%d and %dx%d cells\n", WIDTH, HEIGHT, CELL_W, CELL_H);

    printf("Attempting to insert %d entities\n", NUM_ENTS);

    clock_gettime(CLOCK_REALTIME, &before);
    for (int i = 0; i < NUM_ENTS; i++) {
        struct Rectangle* rect = (struct Rectangle*) vector_add(&entries);
        rect->x = rand() % (WIDTH / 4) + 1;
        rect->y = rand() % (HEIGHT / 4) + 1;
        rect->width = rand() % (WIDTH / 4) + 1;
        rect->height = rand() % (HEIGHT / 4) + 1;

        spatialhashgrid_insert(shg, rect);
    }
    clock_gettime(CLOCK_REALTIME, &after);
    printf("That took %ldns\n", after.tv_nsec - before.tv_nsec);

    printf("Attempting to do %d fetches\n", NUM_FETCHES);
    for (int i = 0; i < NUM_FETCHES; i++) {
        struct Rectangle rect = {
            rand() % (WIDTH / 4) + 1,
            rand() % (HEIGHT / 4) + 1,
            rand() % (WIDTH / 4) + 1,
            rand() % (HEIGHT / 4) + 1
        };

        clock_gettime(CLOCK_REALTIME, &before);
        struct Vector entries = spatialhashgrid_fetch(shg, &rect);
        clock_gettime(CLOCK_REALTIME, &after);
        printf("That took %ldns and got %ld entities\n", after.tv_nsec - before.tv_nsec, entries.length);
        avg += after.tv_nsec - before.tv_nsec;
        vector_delete(&entries);
    }

    printf("Average of %ldns\n", avg / NUM_FETCHES);
}