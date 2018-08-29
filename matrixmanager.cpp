#include "matrixmanager.h"

#include <cmath>
#include <numeric>

#if __cplusplus < 201402L
namespace std {

double hypot(double x, double y, double z)
{
    return std::cbrt(std::pow(x, 3) + std::pow(y, 3) + std::pow(z, 3));
}

}
#endif

namespace matrix {

constexpr auto coef = std::atan(1) * 45;
constexpr double deg2rad(double deg) {
    return deg * coef;
}

Coordinates translate(Coordinates coordinates_from_point, Coordinates coordinates)
{
    auto x = std::get<0>(coordinates) + std::get<0>(coordinates_from_point);
    auto y = std::get<1>(coordinates) + std::get<1>(coordinates_from_point);
    auto z = std::get<2>(coordinates) + std::get<2>(coordinates_from_point);
    return {x, y, z};
}

Coordinates scale(Coordinates coordinates_from_point, Coordinates center, double scale)
{
    auto x = (std::get<0>(coordinates_from_point) - std::get<0>(center)) * scale + std::get<0>(center);
    auto y = (std::get<1>(coordinates_from_point) - std::get<1>(center)) * scale + std::get<1>(center);
    auto z = (std::get<2>(coordinates_from_point) - std::get<2>(center)) * scale + std::get<2>(center);
    return {x, y, z};
}

Coordinates rotate_ccw(Coordinates coordinates_from_point, Coordinates center, double angle)
{
    auto rads = deg2rad(angle);
    auto x = std::cos(rads) * (std::get<0>(coordinates_from_point) - std::get<0>(center));
    x += std::sin(rads) * (std::get<1>(coordinates_from_point) - std::get<1>(center)) + std::get<0>(center);
    auto y =  -1 * std::sin(rads) * (std::get<0>(coordinates_from_point) - std::get<0>(center));
    y += std::cos(rads) * (std::get<1>(coordinates_from_point) - std::get<1>(center)) + std::get<1>(center);
    auto z = 1;
    return Coordinates{x, y, z};
}

Coordinates calculate_center_of_polygon(const std::vector<Coordinates>& coordinates)
{
    constexpr Coordinates zero = {0.0, 0.0, 0.0};
    auto avg = std::accumulate(coordinates.begin(), coordinates.end(), zero,
            [](const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
                return {
                    std::get<0>(lhs) + std::get<0>(rhs),
                    std::get<1>(lhs) + std::get<1>(rhs),
                    std::get<2>(lhs) + std::get<2>(rhs),
                };
            }
    );
    const auto size = coordinates.size();
    return {std::get<0>(avg) / size, std::get<1>(avg) / size, std::get<2>(avg) / size};
}

double distance(Coordinates a, Coordinates b)
{
    Coordinates diff = a - b;
    return std::hypot(std::get<0>(diff), std::get<1>(diff), std::get<2>(diff));
}

}
