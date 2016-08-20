#include "coordinates.h"

#include <stdlib.h>
#include <math.h>

static const float MEAN_RADIUS = 6371;

void convert_to_radians(Coordinates *c) {
    /*Degrees to radians.*/
    (*c).latitude  *= 3.1415926535 / 180;
    (*c).longitude *= 3.1415926535 / 180;
}

float distance(Coordinates c, Coordinates d) {
    convert_to_radians(&c);
    convert_to_radians(&d);
    return 2 * MEAN_RADIUS * asin(sqrt(
                pow(sin((c.latitude - d.latitude) / 2), 2) +
                cos(c.latitude) * cos(d.latitude) *
                pow(sin((c.longitude - d.longitude) / 2), 2)
                ));
}
