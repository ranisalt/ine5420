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
    auto x = std::get<0>(coordinate) + std::get<0>(p.coordinates);
    auto y = std::get<1>(coordinate) + std::get<1>(p.coordinates);
    auto z = std::get<2>(coordinate) + std::get<2>(p.coordinates);
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
