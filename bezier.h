#pragma once

#include "shapes.h"

class Bezier
{
public:
    Bezier() = default;
    ~Bezier() = default;

    Coordinates calculate_point(double t, std::vector<Coordinates> coordinates);
private:
    const std::vector<std::vector<int>> matrix = {{-1, 3, -3, 1},
                                                     {3, -6, 3, 0},
                                                     {-3, 3, 0, 0},
                                                     {1, 0, 0, 0}};
};
