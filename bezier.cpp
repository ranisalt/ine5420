#include "bezier.h"
#include "shapes.h"


Coordinates  Bezier::calculate_point(double t, std::vector<Coordinates> coordinates)
{
    std::vector<double> result;
    auto sum = 0.0;
    auto x = 0.0;
    auto y = 0.0;
    auto z = 0.0;
    auto t_square = t * t;
    auto t_cube = t * t_square;
    const std::vector<double> t_matrix = {t_cube,
                                          t_square,
                                          t,
                                          1};

    for (auto i = 0; i < 4; ++i) {
        for (auto j = 0; j < 4; ++j) {
            sum += t_matrix[j] * matrix[j][i];
        }
        result.push_back(sum);
        sum = 0;
    }

    for (auto i = 0; i < result.size(); ++i) {
        x += result[i] * std::get<0>(coordinates[i]);
        y += result[i] * std::get<1>(coordinates[i]);
        z += result[i] * std::get<2>(coordinates[i]);
    }

    return Coordinates{x,y,z};
}
