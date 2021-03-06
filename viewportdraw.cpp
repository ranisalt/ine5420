#include "objstream.h"
#include "shapes.h"
#include "viewportdraw.h"

#include <algorithm>
#include <cairomm/context.h>

#include <iostream>

std::array<Coordinates, 4> vector_to_array_coordinates(std::vector<Coordinates> coordinates)
{
    std::array<Coordinates, 4> new_coordinates;
    for (auto i = 0; i < 4; ++i) {
        new_coordinates[i] = coordinates[i];
    }
    return new_coordinates;
}

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
        auto z = std::get<Z>(new_coordinates);
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
    } else if (shape.type() == "polygon" or shape.type() == "bspline") {
        std::vector<Coordinates> new_coordinates;

        for(auto coordinate: shape.coordinates()) {
            auto new_coordinate = matrix::translate(coordinates, coordinate);
            new_coordinates.push_back(new_coordinate);
        }

        remove_shape(shape_name);
        if (shape.type() == "polygon") {
            auto polygon = Polygon{new_coordinates};
            add_shape(shape_name, polygon);
        } else {
            auto spline = BSpline{new_coordinates};
            add_shape(shape_name, spline);
        }
    } else if (shape.type() == "bezier") {
        std::array<Coordinates, 4> new_coordinates;
        for(unsigned int i = 0; i < shape.coordinates().size(); ++i) {
            auto new_coordinate = matrix::translate(coordinates, shape.coordinates()[i]);
            new_coordinates[i] = new_coordinate;
        }
        auto polygon = Bezier{new_coordinates};
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

std::vector<Coordinates> ViewPortDraw::calculate_normalized(Shape& s)
{
    auto point = matrix::calculate_center_of_polygon(s.normalized());
    std::vector<Coordinates> new_coordinates;
    for (auto coordinate: s.normalized()) {
        auto c = matrix::rotate_ccw(coordinate, point, total_angle_window);
        new_coordinates.push_back(c);
    }
    return new_coordinates;
}

void ViewPortDraw::rotate_window(double angle)
{
    std::vector<Coordinates> new_coordinates;
    std::vector<std::string> names;
    std::vector<Shape> shapes;
    total_angle_window += angle;

    for (auto s: df) {
        auto shape = s.second;
        auto point = matrix::calculate_center_of_polygon(shape.normalized());
        for (auto coordinate: shape.normalized()) {
            auto coordinate_ = matrix::rotate_ccw(coordinate, point, angle);
            new_coordinates.push_back(coordinate_);
        }
        auto shape_ = Shape(shape);
        shape_.normalized(new_coordinates);
        shapes.push_back(shape_);
        names.push_back(s.first);
        new_coordinates.clear();
    }
    df.clear();

    for (auto i = 0u; i < shapes.size(); ++i) {
        add_shape(names[i], shapes[i]);
    }
}

Shape ViewPortDraw::get_shape_by_name(const std::string& shape_name)
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
    cr->set_source_rgb(0, 0, 0.8);
    cr->move_to(10, 10);
    cr->line_to(10, height - 10);
    cr->line_to(width - 10, height - 10);
    cr->line_to(width - 10, 10);
    cr->line_to(10, 10);
    cr->stroke();

    cr->set_line_width(pen_width);

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);

    for (const auto& entry: df) {
        clipping(cr, window, entry.second);
    }

    return true;
}

void ViewPortDraw::draw_new_shape(std::string shape_name, std::string type_of_shape, std::vector<Coordinates> new_coordinates)
{
    remove_shape(shape_name);
    if (type_of_shape == "point") {
        Shape point = Point{new_coordinates[0]};
        point.normalized(calculate_normalized(point));
        add_shape(shape_name, point);
    } else if (type_of_shape == "line") {
        Shape line = Line{new_coordinates[0], new_coordinates[1]};
        line.normalized(calculate_normalized(line));
        add_shape(shape_name, line);
    } else if (type_of_shape == "polygon") {
        Shape polygon = Polygon{new_coordinates};
        polygon.normalized(calculate_normalized(polygon));
        add_shape(shape_name, polygon);
    } else if (type_of_shape == "bezier") {
        auto coordinates = vector_to_array_coordinates(new_coordinates);
        Shape bezier = Bezier{std::move(coordinates)};
        bezier.normalized(calculate_normalized(bezier));
        add_shape(shape_name, bezier);
    } else if (type_of_shape == "bspline") {
        Shape bspline = BSpline{new_coordinates};
        bspline.normalized(calculate_normalized(bspline));
        add_shape(shape_name, bspline);
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

void ViewPortDraw::clipping(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& s)
{
    if (s.type() == "point") {
        clip_point(ctx, window, s);
    } else if (s.type() == "line") {
        switch (algorithm_) {
            case LIANG_BARSKY:
                clip_liang_barsky(ctx, window, s);
                break;
            case COHEN_SUTHERLAND:
                clip_cohen_sutherland(ctx, window, s);
                break;
        }
    } else if (s.type() == "polygon" or s.type() == "bezier" or s.type() == "bspline") {
        clip_polygon(ctx, window, s);
    }
}

void ViewPortDraw::clip_point(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& p)
{
    auto coordinates = p.vertices().front();
    auto x = std::get<X>(coordinates);
    auto y = std::get<Y>(coordinates);

    if ((x_min_cp() <= x and x <= x_max_cp()) and (y_min_cp() <= y and y <= y_max_cp())) {
        cairo_context_guard guard{ctx};
        std::tie(x, y, std::ignore) = window(p.vertices()[0]);
        constexpr auto tau = std::atan(1) * 8;
        ctx->arc(x, y, 1., 0, tau);
        ctx->stroke();
    }
}

void ViewPortDraw::clip_liang_barsky(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& l)
{
    auto coordinates = l.vertices();
    auto point1 = coordinates[0];
    auto point2 = coordinates[1];
    auto x1 = std::get<X>(point1);
    auto y1 = std::get<Y>(point1);
    auto x2 = std::get<X>(point2);
    auto y2 = std::get<Y>(point2);
    auto delta_x = std::get<X>(point2) - std::get<X>(point1);
    auto delta_y = std::get<Y>(point2) - std::get<Y>(point1);
    std::vector<double> p {-1 * (delta_x), delta_x, -1 * (delta_y), delta_y};
    std::vector<double> q {std::get<X>(point1) - x_min_cp(), x_max_cp() - std::get<X>(point1),
              std::get<Y>(point1) - y_min_cp(), y_max_cp() - std::get<Y>(point1)};
    std::vector<double> r;
    for (unsigned int i = 0; i < p.size(); ++i)
        r.push_back(q[i]/p[i]);
    auto zeta1 = 0.0;
    auto zeta2 = 1.0;

    for (unsigned int i = 0; i < p.size(); ++i) {
        if (p[i] == 0 and q[i] < 0) return;
           if (p[i] < 0) {
            if (r[i] > zeta2) return;
            if (r[i] > zeta1) zeta1 = r[i];
        } else if (p[i] > 0) {
            if (r[i] < zeta1) return;
            if (r[i] < zeta2) zeta2 = r[i];
        }
    }

    cairo_context_guard guard{ctx};
    x1 = std::get<X>(point1) + zeta1 * delta_x;
    y1 = std::get<Y>(point1) + zeta1 * delta_y;
    x2 = std::get<X>(point1) + zeta2 * delta_x;
    y2 = std::get<Y>(point1) + zeta2 * delta_y;
    auto line = Line{{x1, y1, 1}, {x2, y2, 1}};
    std::tie(x1, y1, std::ignore) = window(line.vertices()[0]);
    ctx->move_to(x1, y1);
    std::tie(x2, y2, std::ignore) = window(line.vertices()[1]);
    ctx->line_to(x2, y2);
    ctx->stroke();
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

void ViewPortDraw::clip_cohen_sutherland(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& l)
{
    auto coordinates = l.vertices();
    auto p0 = coordinates.at(0), p1 = coordinates.at(1);

    double x0, y0, z0, x1, y1, z1;
    std::tie(x0, y0, z0) = p0;
    std::tie(x1, y1, z1) = p1;

    Coordinates bl{x_min_cp(), y_min_cp(), 1};
    Coordinates tr{x_max_cp(), y_max_cp(), 1};
    auto p0_direction = direction(p0, bl, tr), p1_direction = direction(p1, bl, tr);

    bool accept = false;
    while (true) {
        if (not (p0_direction | p1_direction)) {
            accept = true;
            break;
        } else if (p0_direction & p1_direction) {
            break;
        }

        double x, y;
        auto outcode = p0_direction ? p0_direction : p1_direction;

        if (outcode & TOP) {
            x = x0 + (x1 - x0) * (y_max_cp() - y0) / (y1 - y0);
            y = y_max_cp();
        } else if (outcode & BOTTOM) {
            x = x0 + (x1 - x0) * (y_min_cp() - y0) / (y1 - y0);
            y = y_min_cp();
        } else if (outcode & RIGHT) {
            x = x_max_cp();
            y = y0 + (y1 - y0) * (x_max_cp() - x0) / (x1 - x0);
        } else if (outcode & LEFT) {
            x = x_min_cp();
            y = y0 + (y1 - y0) * (x_min_cp() - x0) / (x1 - x0);
        }

        if (outcode == p0_direction) {
            x0 = x;
            y0 = y;
            p0_direction = direction({x0, y0, z0}, bl, tr);
        } else {
            x1 = x;
            y1 = y;
            p1_direction = direction({x1, y1, z1}, bl, tr);
        }
    }

    if (accept) {
        cairo_context_guard guard{ctx};
        auto line = Line{{x0, y0, 1}, {x1, y1, 1}};
        std::tie(x0, y0, std::ignore) = window(line.vertices()[0]);
        ctx->move_to(x0, y0);
        std::tie(x1, y1, std::ignore) = window(line.vertices()[1]);
        ctx->line_to(x1, y1);
        ctx->stroke();
    }
}

void ViewPortDraw::clip_polygon(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, const Shape& p)
{
    const auto& coordinates = p.vertices();
    switch (algorithm_) {
        case LIANG_BARSKY:
            for(auto it = coordinates.begin(), it2 = coordinates.begin() + 1;
                it2 != coordinates.end(); ++it, ++it2) {

                auto l = Line{*it, *it2};
                clip_liang_barsky(ctx, window, l);
            }
            break;
        case COHEN_SUTHERLAND:
            for(auto it = coordinates.begin(), it2 = coordinates.begin() + 1;
                it2 != coordinates.end(); ++it, ++it2) {

                auto l = Line{*it, *it2};
                clip_cohen_sutherland(ctx, window, l);
            }
            break;
    }
}
