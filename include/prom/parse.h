#ifndef PARSE_H
#define PARSE_H

#include "constants.h"
#include "coordinates.h"

#include <stdio.h>

void parse_csv(
        FILE *,
        char [CSV_LENGTH][MAX_NAME_LENGTH],
        Coordinates [CSV_LENGTH]
        );

#endif
