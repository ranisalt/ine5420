#include "displayfile.h"

#include <regex>
#include <sstream>

namespace {

std::vector<std::string> split(const std::string& s, const std::string& sep)
{
    auto re = std::regex{sep, std::regex_constants::basic};
    return {
        std::sregex_token_iterator{s.begin(), s.end(), re, -1},
        std::sregex_token_iterator{},
    };
}

Point parse_point(const std::string& coordinates)
{
    auto parsed = split(coordinates, ",");
    double x, y, z;
    x = std::stod(parsed.at(0));
    y = std::stod(parsed.at(1));
    z = std::stod(parsed.at(2));
    return {x, y, z};
}

}

DisplayFile parse_stream(std::istream& is)
{
    std::unordered_set<Shape> shapes;

    for (std::string line; std::getline(is, line); ) {
        std::istringstream iss{line};

        std::string shape;
        iss >> shape;

        if (shape == "point") {
            std::string coordinates;
            iss >> coordinates;

            shapes.emplace(parse_point(coordinates));
        } else if (shape == "line") {
            std::string vertices[2];
            iss >> vertices[0] >> vertices[1];

            Point parsed[2] = {parse_point(vertices[0]), parse_point(vertices[1])};

            shapes.emplace(Line{parsed[0], parsed[1]});
        } else if (shape == "polygon") {
            std::vector<Point> vertices;
            std::string coordinates;
            while (iss >> coordinates) {
                vertices.push_back(parse_point(coordinates));
            }

            shapes.emplace(Polygon{std::move(vertices)});
        }
    }
    return {shapes};
}
