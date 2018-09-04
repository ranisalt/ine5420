#include "coordinates.h"

#include <iostream>

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs)
{
    return {
        std::get<X>(lhs) + std::get<X>(rhs),
        std::get<Y>(lhs) + std::get<Y>(rhs),
        std::get<Z>(lhs) + std::get<Z>(rhs),
    };
}

Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs)
{
    return {
        std::get<X>(lhs) - std::get<X>(rhs),
        std::get<Y>(lhs) - std::get<Y>(rhs),
        std::get<Z>(lhs) - std::get<Z>(rhs),
    };
}

std::ostream& operator<<(std::ostream& os, const Coordinates& rhs)
{
    os << "Coordinates{"
       << std::get<X>(rhs) << ", "
       << std::get<Y>(rhs) << ", "
       << std::get<Z>(rhs) << "}";
    return os;
}
