#ifndef MATRIXMANAGER_H
#define MATRIXMANAGER_H

#include "shapes.h"
#include <math.h>

class MatrixManager
{
public:

    Coordinates translate(Coordinates coordinates_from_point, Coordinates coordinates);
    Coordinates scale(Coordinates coordinates_from_point, Coordinates center, double scale);
    Coordinates rotate_acw(Coordinates coordinates_from_point,double angle);
    // Coordinates turn_cw(Coordinates coordinates_from_point, double angle);

    Coordinates calculate_center_of_polygon(std::vector<Coordinates> coordinates);
    double distance_between_two_points(Coordinates coordinates, Coordinates coordinates_);
};

#endif
