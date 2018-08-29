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

using CoordinateMap = std::unordered_map<unsigned, Coordinates>;

}

DisplayFile parse_stream(std::istream& is)
{
    DisplayFile df;

    std::string name;
    CoordinateMap vertices;

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

void dump_stream(const DisplayFile& df, std::ostream& os)
{
    CoordinateMap vertices;

    auto lineno = 0u;
    for (const auto& entry: df) {
        ++lineno;
        os << "o " << entry.first << std::endl;
        auto shape = entry.second;

        std::vector<unsigned> v;
        const auto& coordinates = shape.coordinates();
        for (const auto& coord: coordinates) {
            auto it = std::find_if(vertices.begin(), vertices.end(), [&](const CoordinateMap::value_type& kv) {
                return kv.second == coord;
            });
            if (it != vertices.end()) {
                v.push_back(it->first);
                continue;
            }

            ++lineno;
            std::tie(it, std::ignore) = vertices.emplace(lineno, coord);
            os << "v " << std::get<0>(coord)
               << ' '  << std::get<1>(coord)
               << ' '  << std::get<2>(coord) << std::endl;
            v.push_back(it->first);
        }

        ++lineno;
        auto it = v.begin();
        os << "l " << *it;
        for (++it; it != v.end(); ++it) {
            os << ' ' << *it;
        }
        os << std::endl;
    }
}
