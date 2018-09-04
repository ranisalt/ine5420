#pragma once

#include <iosfwd>
#include <tuple>

enum {
    X, Y, Z
};

using Coordinates = std::tuple<double, double, double>;

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs);
Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs);
std::ostream& operator<<(std::ostream& os, const Coordinates& rhs);

/* Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs) */
/* { */
/*     return { */
/*         std::get<0>(lhs) + std::get<0>(rhs), */
/*         std::get<1>(lhs) + std::get<1>(rhs), */
/*         std::get<2>(lhs) + std::get<2>(rhs), */
/*     }; */
/* } */

/* Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs) */
/* { */
/*     return { */
/*         std::get<0>(lhs) - std::get<0>(rhs), */
/*         std::get<1>(lhs) - std::get<1>(rhs), */
/*         std::get<2>(lhs) - std::get<2>(rhs), */
/*     }; */
/* } */
