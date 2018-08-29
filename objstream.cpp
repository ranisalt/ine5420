#include "objstream.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

namespace {

Coordinates parse_vertex(std::istringstream& iss)
{
    std::string x, y, z;
    iss >> x >> y >> z;
    return {std::stod(x), std::stod(y), std::stod(z)};
}

template<class Vertices>
Shape create_shape(std::istringstream& iss, const Vertices& vertices)
{
    std::vector<Coordinates> v;
    for (unsigned i; iss >> i; ) {
        v.push_back(vertices.at(i));
    }

    switch (v.size()) {
        case 0:
            throw std::runtime_error("Object with no vertices!");
        case 1:
            return Point{v[0]};
        case 2:
            return Line{v[0], v[1]};
        default:
            return Polygon{v};
    }
}

}

DisplayFile parse_stream(std::istream& is)
{
    DisplayFile df;

    std::string name;
    std::unordered_map<unsigned, Coordinates> vertices;

    auto lineno = 0u;
    for (std::string line; std::getline(is, line); ) {
        ++lineno;
        std::istringstream iss{line};

        char type;
        iss >> type;

        switch (type) {
            case 'o':
                iss >> name;
                break;
            case 'v':
                vertices.emplace(lineno, parse_vertex(iss));
                break;
            case 'l':
                df.emplace(name, create_shape(iss, vertices));
                break;
            default:
                std::cout << type << '\n';
        }
    }
    return df;
}

void export_object_file(DisplayFile df, std::string path)
{
    auto i = 1;
    std::vector<int> lines;
    std::fstream object_file(path, std::fstream::in | std::fstream::out | std::fstream::app);
    std::string line = "l ";
    for (auto entry: df) {
        object_file << "o " << entry.first << std::endl;
        auto shape = entry.second;
        for (auto coordinate: shape.coordinates()) {
            auto x = std::to_string(std::get<0>(coordinate));
            auto y = std::to_string(std::get<1>(coordinate));
            auto z = std::to_string(std::get<2>(coordinate));
            object_file << "v " + x + " " + y + " " + z << std::endl;;
            ++i;
            lines.push_back(i);
        }
        for (auto line_: lines) {
            line += std::to_string(line_) + " ";
        }
        object_file << line << std::endl;
        line = "";
        ++i;
    }
    object_file.close();
}
