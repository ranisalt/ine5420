#include "matrixmanager.h"

Coordinates MatrixManager::translate(Coordinates coordinates_from_point, Coordinates coordinates)
{
    auto x = std::get<0>(coordinates) + std::get<0>(coordinates_from_point);
    auto y = std::get<1>(coordinates) + std::get<1>(coordinates_from_point);
    auto z = std::get<2>(coordinates) + std::get<2>(coordinates_from_point);
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::scale(Coordinates coordinates_from_point, Coordinates center, double scale)
{
    auto x = (std::get<0>(coordinates_from_point) - std::get<0>(center)) * scale + std::get<0>(center);
    auto y = (std::get<1>(coordinates_from_point) - std::get<1>(center)) * scale + std::get<1>(center);
    auto z = (std::get<2>(coordinates_from_point) - std::get<2>(center)) * scale + std::get<2>(center);
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::rotate_acw(Coordinates coordinates_from_point, Coordinates center, double angle)
{
    auto x_ = (cos(angle * M_PI / 180.0) * (std::get<0>(coordinates_from_point) - std::get<0>(center)));
    auto x = x_ + (sin(angle * M_PI / 180.0) * (std::get<1>(coordinates_from_point) - std::get<1>(center))) + std::get<0>(center);
    auto y_ =  -1 * sin( angle * M_PI / 180.0 ) * (std::get<0>(coordinates_from_point) - std::get<0>(center));
    auto y = y_ + cos(angle * M_PI / 180.0) * (std::get<1>(coordinates_from_point) - std::get<1>(center)) + std::get<1>(center);
    auto z = 1;
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::calculate_center_of_polygon(std::vector<Coordinates> coordinates)
{
    Coordinates center;
    int i = 0;
    double average_x = 0;
    double average_y = 0;
    double average_z = 0;
    for (auto coordinate: coordinates) {
        ++i;
        average_x += std::get<0>(coordinate);
        average_y += std::get<1>(coordinate);
        average_z += std::get<2>(coordinate);
    }

    average_x = average_x / i;
    average_y = average_y  / i;
    average_z = average_z  / i;
    return Coordinates{average_x, average_y, average_z};
}

double MatrixManager::distance_between_two_points(Coordinates coordinates, Coordinates coordinates_)
{   auto x_diff = std::get<0>(coordinates) - std::get<0>(coordinates_);
    auto y_diff = std::get<1>(coordinates) - std::get<1>(coordinates_);
    auto expr = x_diff * x_diff + y_diff * y_diff;
    return sqrt(expr);
}
