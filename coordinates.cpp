#include "coordinates.h"

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs)
{
    return {
        std::get<0>(lhs) + std::get<0>(rhs),
        std::get<1>(lhs) + std::get<1>(rhs),
        std::get<2>(lhs) + std::get<2>(rhs),
    };
}

Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs)
{
    return {
        std::get<0>(lhs) - std::get<0>(rhs),
        std::get<1>(lhs) - std::get<1>(rhs),
        std::get<2>(lhs) - std::get<2>(rhs),
    };
}
