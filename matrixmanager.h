#ifndef MATRIXMANAGER_H
#define MATRIXMANAGER_H

#include "shapes.h"

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
