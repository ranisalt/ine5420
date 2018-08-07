#include "../displayfile.h"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <regex>
#include <sstream>

using namespace std::literals;

TEST_CASE("displayfile#parse_stream", "[displayfile]") {
    SECTION("parse_stream(...) point") {
        std::stringstream ss;
        ss << "point 1.0,3.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto point = *df.shapes.begin();
        REQUIRE(point == Point{1.0, 3.0, 1.0});
    }

    SECTION("parse_stream(...) line") {
        std::stringstream ss;
        ss << "line 1.0,3.0,1.0 2.0,3.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto line = *df.shapes.begin();
        REQUIRE(line == Line{{1.0, 3.0, 1.0}, {2.0, 3.0, 1.0}});
    }

    SECTION("parse_stream(...) polygon") {
        std::stringstream ss;
        ss << "polygon 1.0,3.0,1.0 2.0,3.0,1.0, 0.0,4.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto line = *df.shapes.begin();
        REQUIRE(line == Polygon{{1.0, 3.0, 1.0}, {2.0, 3.0, 1.0}, {0.0, 4.0, 1.0}});
    }
}
