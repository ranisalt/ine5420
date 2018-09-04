#include "shapes.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Line& rhs)
{
    const auto& coordinates = rhs.coordinates();
    std::cout << "Line{" << coordinates[0] << ", " << coordinates[1] << "}";
    return os;
}
