#ifndef MATRIXMANAGER_H
#define MATRIXMANAGER_H

#include "shapes.h"
#include <math.h>

class MatrixManager
{
public:

    Coordinates translate(Coordinates coordinates_from_point, Coordinates coordinates);
    Coordinates scale_up(Coordinates coordinates_from_point, double scale);
    Coordinates scale_down(Coordinates coordinates_from_point, double scale);
    Coordinates rotate_acw(Coordinates coordinates_from_point, double angle);
    // Coordinates turn_cw(Coordinates coordinates_from_point, double angle);
};

#endif
