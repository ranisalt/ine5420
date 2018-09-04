#include "objstream.h"
#include "shapes.h"
#include "viewportdraw.h"

#include <algorithm>
#include <cairomm/context.h>

#include <iostream>

void ViewPortDraw::add_shape(std::string name, Shape shape, bool queue_draw_)
{
    df.emplace(std::move(name), std::move(shape));
    if (queue_draw_) {
        queue_draw();
    }
}

void ViewPortDraw::remove_shape(const std::string& name)
{
    df.erase(name);
    queue_draw();
}

void ViewPortDraw::on_in_click()
{
    constexpr auto DELTA = 0.05;

    auto w_width = (x_max - x_min) * DELTA;
    x_min += w_width / 2;
    x_max -= w_width / 2;

    auto w_height = (y_max - y_min) * DELTA;
    y_min += w_height / 2;
    y_max -= w_height / 2;

    pen_width *= 1 + DELTA;

    queue_draw();
}

void ViewPortDraw::on_out_click()
{
    constexpr auto DELTA = 0.05 / 1.05;

    auto w_width = (x_max - x_min) * DELTA;
    x_min -= w_width / 2;
    x_max += w_width / 2;

    auto w_height = (y_max - y_min) * DELTA;
    y_min -= w_height / 2;
    y_max += w_height / 2;

    pen_width *= 1 - DELTA;

    queue_draw();
}

void ViewPortDraw::up_click()
{
    constexpr auto DELTA = 0.05 / 1.05;

    auto w_height = (y_max - y_min) * DELTA;
    y_min += w_height / 2;
    y_max += w_height / 2;

    queue_draw();
}

void ViewPortDraw::left_click()
{
    constexpr auto DELTA = 0.05 / 1.05;

    auto w_width = (x_max - x_min) * DELTA;
    x_min -= w_width / 2;
    x_max -= w_width / 2;

    queue_draw();
}

void ViewPortDraw::down_click()
{
    constexpr auto DELTA = 0.05 / 1.05;

    auto w_height = (y_max - y_min) * DELTA;
    y_min -= w_height / 2;
    y_max -= w_height / 2;

    queue_draw();
}

void ViewPortDraw::right_click()
{
    constexpr auto DELTA = 0.05 / 1.05;

    auto w_width = (x_max - x_min) * DELTA;
    x_min += w_width / 2;
    x_max += w_width / 2;

    queue_draw();
}

void ViewPortDraw::translate(Coordinates coordinates, std::string shape_name)
{
    auto shape = df.at(shape_name);

    if (shape.type() == "point") {
        auto coordinates_from_point = shape.coordinates()[0];
        auto new_coordinates = matrix::translate(coordinates_from_point, coordinates);
        auto x = std::get<X>(new_coordinates);
        auto y = std::get<Y>(new_coordinates);
        auto z = std::get<2>(new_coordinates);
        auto point =  Point{x, y, z};

        remove_shape(shape_name);
        add_shape(shape_name, point);
    } else if (shape.type() == "line") {
        auto coordinates_from_start = shape.coordinates()[0];
        auto coordinates_from_end = shape.coordinates()[1];
        auto coordinates_1 = matrix::translate(coordinates_from_start, coordinates);
        auto coordinates_2 = matrix::translate(coordinates_from_end, coordinates);
        auto line = Line{coordinates_1, coordinates_2};

        remove_shape(shape_name);
        add_shape(shape_name, line);
    } else if (shape.type() == "polygon") {
        std::vector<Coordinates> new_coordinates;

        for(auto coordinate: shape.coordinates()) {
            auto new_coordinate = matrix::translate(coordinates, coordinate);
            new_coordinates.push_back(new_coordinate);
        }
        auto polygon = Polygon{new_coordinates};
        remove_shape(shape_name);
        add_shape(shape_name, polygon);
    }
}

void ViewPortDraw::scale_up(Shape s, std::string shape_name)
{
    constexpr auto DELTA = 1.05;
    auto type_of_shape = s.type();
    std::vector<Coordinates> new_coordinates;
    auto center = matrix::calculate_center_of_polygon(s.coordinates());

    for(auto coordinate: s.coordinates()) {
        auto coordinate_ = matrix::scale(coordinate, center, DELTA);
        new_coordinates.push_back(coordinate_);
    }

    draw_new_shape(shape_name, type_of_shape, new_coordinates);
}

void ViewPortDraw::scale_down(Shape s, std::string shape_name)
{
    constexpr auto DELTA = 0.95;
    auto type_of_shape = s.type();
    std::vector<Coordinates> new_coordinates;
    auto center = matrix::calculate_center_of_polygon(s.coordinates());

    for(auto coordinate: s.coordinates()) {
        auto coordinate_ = matrix::scale(coordinate, center, DELTA);
        new_coordinates.push_back(coordinate_);
    }

    draw_new_shape(shape_name, type_of_shape, new_coordinates);
}

void ViewPortDraw::rotate_acw(Shape s, std::string shape_name, Coordinates point, double angle)
{
    std::vector<Coordinates> new_coordinates;
    auto type_of_shape = s.type();

    for(auto coordinate: s.coordinates()) {
        auto coordinate_ = matrix::rotate_ccw(coordinate, point, angle);
        new_coordinates.push_back(coordinate_);
    }

    draw_new_shape(shape_name, type_of_shape, new_coordinates);
}

void ViewPortDraw::calculate_normalized_coordinates(Shape& s)
{
    auto point = matrix::calculate_center_of_polygon(s.normalized_coordinates());
    std::vector<Coordinates> new_coordinates;
    for (auto coordinate: s.normalized_coordinates()) {
        auto coordinate_ = matrix::rotate_ccw(coordinate, point, total_angle_window);
        new_coordinates.push_back(coordinate_);
    }
    s.set_coordinates_normalized(new_coordinates);
}

void ViewPortDraw::rotate_window(double angle)
{
    std::vector<Coordinates> new_coordinates;
    std::vector<std::string> names;
    std::vector<Shape> shapes;
    total_angle_window += angle;

    for (auto s: df) {
        auto shape = s.second;
        auto point = matrix::calculate_center_of_polygon(shape.normalized_coordinates());
        for (auto coordinate: shape.normalized_coordinates()) {
            auto coordinate_ = matrix::rotate_ccw(coordinate, point, angle);
            new_coordinates.push_back(coordinate_);
        }
        auto shape_ = Shape(shape);
        shape_.set_coordinates_normalized(new_coordinates);
        shapes.push_back(shape_);
        names.push_back(s.first);
        new_coordinates.clear();
    }
    df.clear();

    for (auto i = 0u; i < shapes.size(); ++i) {
        add_shape(names[i], shapes[i]);
    }
}

Shape ViewPortDraw::get_shape_by_name(std::string shape_name)
{
    return df.at(shape_name);
}

bool ViewPortDraw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    auto allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    auto window = [=](const Coordinates& orig) -> Coordinates {
        double x, y, z;
        std::tie(x, y, z) = orig;

        x = (x - x_min) / (x_max - x_min) * width;
        y = (1 - (y - y_min) / (y_max - y_min)) * height;
        return {x, y, z};
    };

    cr->set_line_width(pen_width_cp);

    // draw viewport limits for test clipping
    x_max_cp = width - 10;
    y_max_cp = height - 10;
    x_min_cp = 10;
    y_min_cp = 10;
    cr->set_source_rgb(0, 0, 0.8);
    cr->move_to(x_min_cp, y_min_cp);
    cr->line_to(x_min_cp, y_max_cp);
    cr->line_to(x_max_cp, y_max_cp);
    cr->line_to(x_max_cp, y_min_cp);
    cr->line_to(x_min_cp, y_min_cp);
    cr->stroke();

    cr->set_line_width(pen_width);

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);

    for (const auto& entry: df) {
        clipping(cr, window, entry.second);
        // entry.second.draw(cr, window);
    }

    return true;
}

void ViewPortDraw::draw_new_shape(std::string shape_name, std::string type_of_shape, std::vector<Coordinates> new_coordinates)
{
    remove_shape(shape_name);
    if (type_of_shape == "point") {
        Shape point = Point{new_coordinates[0]};
        calculate_normalized_coordinates(point);
        add_shape(shape_name, point);
    } else if (type_of_shape == "line") {
        Shape line = Line{new_coordinates[0], new_coordinates[1]};
        calculate_normalized_coordinates(line);
        add_shape(shape_name, line);
    } else if (type_of_shape == "polygon") {
        Shape polygon = Polygon{new_coordinates};
        calculate_normalized_coordinates(polygon);
        add_shape(shape_name, polygon);
    }
}

void ViewPortDraw::on_realize()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    auto allocation = get_allocation();
    x_min = 0;
    x_max = allocation.get_width();
    y_min = 0;
    y_max = allocation.get_height();

    Gtk::DrawingArea::on_realize();
}

void ViewPortDraw::clipping(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape s)
{
    if (s.type() == "point") {
        clip_point(ctx, window, s);
    } else if (s.type() == "line") {
        // clip_liang_barsky(ctx, window, s);
        clip_nicholl_lee_nicholl(ctx, window, s);
    }
}

void ViewPortDraw::clip_point(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape p)
{
    auto coordinates = p.normalized_coordinates()[0];
    auto x = std::get<X>(coordinates);
    auto y = std::get<Y>(coordinates);

    if ((x_min + 10 <= x and x <= x_max - 10) and (y_min + 10 <= y and y <= y_max - 10)) {
        p.draw(ctx, window);
    }
}

void ViewPortDraw::clip_liang_barsky(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape l)
{
    auto coordinates = l.normalized_coordinates();
    auto point1 = coordinates[0];
    auto point2 = coordinates[1];
    auto x1 = std::get<X>(point1);
    auto y1 = std::get<Y>(point1);
    auto x2 = std::get<X>(point2);
    auto y2 = std::get<Y>(point2);
    auto delta_x = std::get<X>(point2) - std::get<X>(point1);
    auto delta_y = std::get<Y>(point2) - std::get<Y>(point1);
    auto p1 = -1 * (delta_x);
    auto p2 = delta_x;
    auto p3 = -1 * (delta_y);
    auto p4 = delta_y;
    auto q1 = std::get<X>(point1) - (x_min + 10);
    auto q2 = (x_max - 10) - std::get<X>(point1);
    auto q3 = std::get<Y>(point1) - (y_min + 10);
    auto q4 = (y_max - 10) - std::get<Y>(point1);
    auto r1 = q1 / p1;
    auto r2 = q2 / p2;
    auto r3 = q3 / p3;
    auto r4 = q4 / p4;
    double zeta1, zeta2;

    // parallel
    if (p1 == 0 or p3 == 0) {
        // out of window
        if (q1 < 0 or q3 < 0) return;
    }

    // out -> in
    if (p1 < 0) {
        if (p2 < 0) {
            zeta1 = std::max(0.0, std::max(r1, r2));
            zeta2 = std::min(1.0, std::min(r3, r4));
        } else if (p3 < 0) {
            zeta1 = std::max(0.0, std::max(r1, r3));
            zeta2 = std::min(1.0, std::min(r2, r4));
        } else if (p4 < 0) {
            zeta1 = std::max(0.0, std::max(r1, r4));
            zeta2 = std::min(1.0, std::min(r2, r3));
        }

        if (zeta1 > zeta2) return;
        if (zeta1 > 0) {
            std::cout << "zeta 1; p1 < 0 and p3 < 0" << std::endl;
            x1 = std::get<X>(point1) + zeta1 * delta_x;
            y1 = std::get<Y>(point1) + zeta1 * delta_y;
        }

        if (zeta2 < 1.0) {
            std::cout << "zeta 2; p1 < 0 and p3 < 0" << std::endl;
            x2 = std::get<X>(point1) + zeta2 * delta_x;
            y2 = std::get<Y>(point1) + zeta2 * delta_y;
        }
    // in -> out
    } else if (p1 > 0) {
        if (p2 > 0) {
            zeta1 = std::max(0.0, std::max(r3, r4));
            zeta2 = std::min(1.0, std::min(r1, r2));
        } else if (p3 > 0) {
            zeta1 = std::max(0.0, std::max(r2, r4));
            zeta2 = std::min(1.0, std::min(r1, r3));
        } else if (p4 > 0) {
            zeta1 = std::max(0.0, std::max(r2, r3));
            zeta2 = std::min(1.0, std::min(r1, r4));
        }

        if (zeta1 > zeta2) return;
        if (zeta1 > 0) {
            std::cout << "zeta 1; p1 > 0 and p3 > 0" << std::endl;
            x1 = std::get<X>(point1) + zeta1 * delta_x;
            y1 = std::get<Y>(point1) + zeta1 * delta_y;
        }

        if (zeta2 < 1) {
            std::cout << "zeta 2; p1 > 0 and p3 > 0" << std::endl;
            x2 = std::get<X>(point1) + zeta2 * delta_x;
            y2 = std::get<Y>(point1) + zeta2 * delta_y;
        }
    }

    auto line = Line{{x1, y1, 1}, {x2, y2, 1}};
    line.draw(ctx, window);
}

enum Direction {
    CENTER = 0,
    RIGHT = 1,
    LEFT = 2,
    TOP = 4,
    BOTTOM = 8,
};

unsigned direction(const Coordinates& c, const Coordinates& bottom_left,
        const Coordinates& top_right)
{
    unsigned d = CENTER;
    if (std::get<X>(c) > std::get<X>(top_right)) {
        d |= RIGHT;
    } else if (std::get<X>(c) < std::get<X>(bottom_left)) {
        d |= LEFT;
    }

    if (std::get<Y>(c) > std::get<Y>(top_right)) {
        d |= TOP;
    } else if (std::get<Y>(c) < std::get<Y>(bottom_left)) {
        d |= BOTTOM;
    }
    return d;
}

void ViewPortDraw::clip_nicholl_lee_nicholl(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& l)
{
    auto coordinates = l.normalized_coordinates();
    auto point1 = coordinates.at(0);
    auto point2 = coordinates.at(1);

    double x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = point1;
    std::tie(x2, y2, z2) = point2;

    auto p1_direction = direction(point1, {x_min + 10, y_min + 10, z1}, {x_max - 10, y_max - 10, z1});
    auto p2_direction = direction(point2, {x_min + 10, y_min + 10, z2}, {x_max - 10, y_max - 10, z2});

    if (p1_direction & p2_direction) {
        return;
    }

    // left
    if (p1_direction == LEFT) {
        double mi = (x2 - (x_min + 10)) / (x2 - x1);
        x1 = x_min + 10;
        y1 = y2 - (y2 - y1) * mi;
    // left top
    } else if (p1_direction == (LEFT | TOP)) {

    // top
    } else if (p1_direction == TOP) {

    // top right
    } else if (p1_direction == (TOP | RIGHT)) {

    // right
    } else if (p1_direction == RIGHT) {

    // right bottom
    } else if (p1_direction == (RIGHT | BOTTOM)) {

    //bottom
    } else if (p1_direction == BOTTOM) {

    // left bottom
    } else if (p1_direction == (LEFT | BOTTOM)) {

    }

    auto line = Line{{x1, y1, 1}, {x2, y2, 1}};
    line.draw(ctx, window);
}
