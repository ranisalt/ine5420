#include "shapes.h"
#include "viewportdraw.h"
#include <iostream>
#include <cairomm/context.h>

void ViewPortDraw::add_shape(std::string name, Shape shape)
{
    df.emplace(std::move(name), std::move(shape));
    queue_draw();
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

        auto x = std::get<0>(coordinates) + std::get<0>(coordinates_from_point);
        auto y = std::get<1>(coordinates) + std::get<1>(coordinates_from_point);
        auto z = std::get<2>(coordinates) + std::get<2>(coordinates_from_point);

        auto point = Point{x, y, z};
        remove_shape(shape_name);
        add_shape(shape_name, point);
    } else if (shape.type() == "line") {
        auto coordinates_from_start = shape.coordinates()[0];
        auto coordinates_from_end = shape.coordinates()[1];

        auto x1 = std::get<0>(coordinates) + std::get<0>(coordinates_from_start);
        auto y1 = std::get<1>(coordinates) + std::get<1>(coordinates_from_start);
        auto z1 = std::get<2>(coordinates) + std::get<2>(coordinates_from_start);

        auto x2 = std::get<0>(coordinates) + std::get<0>(coordinates_from_end);
        auto y2 = std::get<1>(coordinates) + std::get<1>(coordinates_from_end);
        auto z2 = std::get<2>(coordinates) + std::get<2>(coordinates_from_end);

        auto line = Line{Coordinates{x1,y1,z1}, Coordinates{x2,y2,z2}};
        remove_shape(shape_name);
        add_shape(shape_name, line);
    } else if (shape.type() == "polygon") {
        std::vector<Coordinates> new_coordinates;

        for(auto coordinate: shape.coordinates()) {
            auto x = std::get<0>(coordinates) + std::get<0>(coordinate);
            auto y = std::get<1>(coordinates) + std::get<1>(coordinate);
            auto z = std::get<2>(coordinates) + std::get<2>(coordinate);
            new_coordinates.push_back(Coordinates{x, y, z});
        }
        auto polygon = Polygon{new_coordinates};
        remove_shape(shape_name);
        add_shape(shape_name, polygon);
    }
}

void ViewPortDraw::scale_up(Shape s, std::string shape_name)
{
    constexpr auto DELTA = 1.05;
    std::vector<Coordinates> new_coordinates;

    for(auto coordinate: s.coordinates()) {
        auto x = DELTA * std::get<0>(coordinate);
        auto y = DELTA * std::get<1>(coordinate);
        auto z = DELTA * std::get<2>(coordinate);
        new_coordinates.push_back(Coordinates{x, y, z});
    }
    auto polygon = Polygon{new_coordinates};
    remove_shape(shape_name);
    add_shape(shape_name, polygon);
}

void ViewPortDraw::scale_down(Shape s, std::string shape_name)
{
    constexpr auto DELTA = 0.95;
    std::vector<Coordinates> new_coordinates;

    for(auto coordinate: s.coordinates()) {
        auto x = DELTA / std::get<0>(coordinate);
        auto y = DELTA / std::get<1>(coordinate);
        auto z = DELTA / std::get<2>(coordinate);
        new_coordinates.push_back(Coordinates{x, y, z});
    }
    auto polygon = Polygon{new_coordinates};
    remove_shape(shape_name);
    add_shape(shape_name, polygon);
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

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(pen_width);

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);

    for (auto& entry: df) {
        entry.second.draw(cr, window);
    }
    cr->stroke();

    return true;
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
