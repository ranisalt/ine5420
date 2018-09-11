#include "shapes.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Line& rhs)
{
    const auto& coordinates = rhs.coordinates();
    return os;
}
