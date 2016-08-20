#ifndef COORDINATES_H
#define COORDINATES_H

typedef struct {
    float latitude;
    float longitude;
} Coordinates;

float distance(Coordinates, Coordinates);

#endif
