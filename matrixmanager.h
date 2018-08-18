#ifndef MATRIX_MANAGER_H
#define MATRIX_MANAGER_H

#include "shapes.h"

template <size_t... Is>
struct index_sequence;

class MatrixManager
{
public:
    MatrixManager();
    virtual ~MatrixManager();

    Coordinates translate(Point p, Coordinates coordinates);
    // std::vector<std::vector<double>> schedule(Point p);
    // std::vector<std::vector<double>> rotate(Point p);
};

#endif
