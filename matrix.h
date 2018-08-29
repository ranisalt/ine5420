#pragma once

#include "shapes.h"

namespace matrix {

Coordinates translate(Coordinates coordinates_from_point, Coordinates coordinates);
Coordinates scale(Coordinates coordinates_from_point, Coordinates center, double scale);
Coordinates rotate_ccw(Coordinates coordinates_from_point, Coordinates center, double angle);
// Coordinates turn_cw(Coordinates coordinates_from_point, double angle);

Coordinates calculate_center_of_polygon(const std::vector<Coordinates>& coordinates);
double distance(Coordinates a, Coordinates b);

}  // namespace matrix
