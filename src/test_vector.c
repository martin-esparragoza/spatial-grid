#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"
#define ASSERT(a) if (!(a)) {printf("[LN : %d] Failed assertion\n", __LINE__); exit(-1);}
#define NUM 100

struct Vector v;

int main() {
    ASSERT(vector_new(&v, 1, sizeof(unsigned)));

    for (unsigned i = 0; i < NUM; i++) {
        bool val = vector_push_forward(&v, &i);
        ASSERT(val);
        //*val = i;
    }

    ASSERT(v.length == NUM);

    for (unsigned i = 0; i < NUM; i++) {
        unsigned* val = (unsigned*) vector_get(&v, i);
        ASSERT(val);
        printf("%d\n", *val);
    }

    vector_delete(&v);
}