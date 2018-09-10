#include "bezier.h"
#include "shapes.h"


Coordinates  Bezier::calculate_point(double t, std::vector<Coordinates> coordinates)
{
    std::vector<double> result;
    auto sum = 0;
    auto x = 0;
    auto y = 0;
    auto z = 0;
    auto t_square =  t * t;
    auto t_cube = t * t_square;
    const std::vector<double> t_matrix = {2 * t_cube - 3 * t_square + 1,
                     -2 * t_cube + 3 * t_square,
                     t_cube - 2 * t_square + t,
                     t_cube - t_square};

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
