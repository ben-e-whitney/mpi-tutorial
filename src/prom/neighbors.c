#include "neighbors.h"

#include <time.h>

#include <stdio.h>

float get_p(float distance) {
    float p = 1 - distance / 5000;
    return p > 0 ? p : 0;
}

void get_neighbors(int rank, int size, Coordinates latlongs[CSV_LENGTH],
        bool neighbors[CSV_LENGTH]) {
    srand(time(NULL) + rank * rank);
    for (size_t i = 0; i < CSV_LENGTH; ++i) {
        neighbors[i] = i != rank && ((float) rand()) / RAND_MAX < get_p(
                distance(latlongs[rank], latlongs[i])
                );
    }
}
