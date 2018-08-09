#include "../shapes.h"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("shapes#point", "[shape]") {
    Shape point = Point{1.0, 3.0};

    SECTION("Point == Point") {
        Shape point2 = Point{1.0, 3.0};
        REQUIRE(point == point2);

        Shape point3 = Point{2.0, 4.0};
        REQUIRE(point != point3);
    }

    SECTION("type() == 'point'") {
        REQUIRE(point.type() == "point");
    }
}

TEST_CASE("shapes#line", "[shape]") {
    Shape line = Line{{1.0, 3.0}, {2.0, 5.0}};

    SECTION("Line == Line") {
        Shape line2 = Line{{1.0, 3.0}, {2.0, 5.0}};
        REQUIRE(line == line2);

        Shape line3 = Line{{2.0, 4.0}, {3.0, 6.0}};
        REQUIRE(line != line3);
    }

    SECTION("type() == 'line'") {
        REQUIRE(line.type() == "line");
    }
}

TEST_CASE("shapes#polygon", "[shape]") {
    Shape polygon = Polygon{{1.0, 3.0}, {2.0, 5.0}, {0.0, 4.0}};

    SECTION("Polygon == Polygon") {
        Shape polygon2 = Polygon{{1.0, 3.0}, {2.0, 5.0}, {0.0, 4.0}};
        REQUIRE(polygon == polygon2);

        Shape polygon3 = Polygon{{2.0, 2.0}, {3.0, 4.0}, {1.0, 3.0}};
        REQUIRE(polygon != polygon3);
    }

    SECTION("type() == 'polygon'") {
        REQUIRE(polygon.type() == "polygon");
    }
}
