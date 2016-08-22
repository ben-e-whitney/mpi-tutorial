#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include "constants.h"
#include "coordinates.h"

#include <stdlib.h>
#include <stdbool.h>

void get_neighbors(int, int, Coordinates [CSV_LENGTH],
        bool [CSV_LENGTH]);

#endif
