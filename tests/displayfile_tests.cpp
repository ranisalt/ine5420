#include "../displayfile.h"
#include "../objstream.h"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <regex>
#include <sstream>

TEST_CASE("displayfile#parse_stream", "[displayfile]") {
    SECTION("parse_stream(...) point") {
        std::stringstream ss;
        ss << "point point 1.0,3.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto point = df.at("point");
        REQUIRE(point == Point{1.0, 3.0, 1.0});
    }

    SECTION("parse_stream(...) line") {
        std::stringstream ss;
        ss << "line line 1.0,3.0,1.0 2.0,3.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto line = df.at("line");
        REQUIRE(line == Line{{1.0, 3.0, 1.0}, {2.0, 3.0, 1.0}});
    }

    SECTION("parse_stream(...) polygon") {
        std::stringstream ss;
        ss << "polygon polygon 1.0,3.0,1.0 2.0,3.0,1.0, 0.0,4.0,1.0" << std::endl;

        auto df = parse_stream(ss);
        auto polygon = df.at("polygon");
        REQUIRE(polygon == Polygon{{1.0, 3.0, 1.0}, {2.0, 3.0, 1.0}, {0.0, 4.0, 1.0}});
    }
}
