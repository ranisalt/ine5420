#include "matrixmanager.h"

Coordinates MatrixManager::translate(Coordinates coordinates_from_point, Coordinates coordinates)
{
    auto x = std::get<0>(coordinates) + std::get<0>(coordinates_from_point);
    auto y = std::get<1>(coordinates) + std::get<1>(coordinates_from_point);
    auto z = std::get<2>(coordinates) + std::get<2>(coordinates_from_point);
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::scale_up(Coordinates coordinates_from_point, double scale)
{
    auto x = scale * std::get<0>(coordinates_from_point);
    auto y = scale * std::get<1>(coordinates_from_point);
    auto z = scale * std::get<2>(coordinates_from_point);
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::scale_down(Coordinates coordinates_from_point, double scale)
{
    auto x = scale / std::get<0>(coordinates_from_point);
    auto y = scale / std::get<1>(coordinates_from_point);
    auto z = scale / std::get<2>(coordinates_from_point);
    return Coordinates{x, y, z};
}

Coordinates MatrixManager::rotate_acw(Coordinates coordinates_from_point, double angle)
{
    auto x = std::get<0>(coordinates_from_point) * cos( angle * M_PI / 180.0 ) - std::get<1>(coordinates_from_point) * sin( angle * M_PI / 180.0 );
    auto y = std::get<0>(coordinates_from_point) * sin( angle * M_PI / 180.0 ) + std::get<1>(coordinates_from_point) * cos( angle * M_PI / 180.0 );
    auto z = 1;
    return Coordinates{x, y, z};
}
