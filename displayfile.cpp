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

Coordinates parse_coordinates(const std::string& coordinates)
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
    DisplayFile df;

    for (std::string line; std::getline(is, line); ) {
        std::istringstream iss{line};

        std::string shape, name;
        iss >> shape >> name;

        if (shape == "point") {
            std::string coordinates;
            iss >> coordinates;

            df.emplace(std::move(name), Point{parse_coordinates(coordinates)});
        } else if (shape == "line") {
            std::string vertices[2];
            iss >> vertices[0] >> vertices[1];

            Coordinates parsed[2] = {parse_coordinates(vertices[0]), parse_coordinates(vertices[1])};

            df.emplace(std::move(name), Line{parsed[0], parsed[1]});
        } else if (shape == "polygon") {
            std::vector<Coordinates> vertices;
            std::string coordinates;
            while (iss >> coordinates) {
                vertices.push_back(parse_coordinates(coordinates));
            }

            df.emplace(std::move(name), Polygon{std::move(vertices)});
        }
    }
    return df;
}
