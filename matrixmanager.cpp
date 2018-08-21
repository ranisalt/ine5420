#include "matrixmanager.h"

MatrixManager::MatrixManager()
{

}

MatrixManager::~MatrixManager()
{

}

Coordinates MatrixManager::translate(Point p, Coordinates coordinate)
{
    // auto len = std::tuple_size<decltype(coordinate)>::value;
    auto coordinates = p.coordinates();
    auto x = std::get<0>(coordinate) + std::get<0>(coordinates[0]);
    auto y = std::get<1>(coordinate) + std::get<1>(coordinates[0]);
    auto z = std::get<2>(coordinate) + std::get<2>(coordinates[0]);
    return Coordinates{x, y, z};
}

// std::vector<double> MatrixManager::schedule(Point p)
// {
//     std::vector<std::vector<double>> t;
//     return t;
// }

// std::vector<double> MatrixManager::rotate(Point p)
// {
//     std::vector<std::vector<double>> t;
//     return t;
// }
