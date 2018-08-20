#include "shapes.h"
#include "viewportdraw.h"

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

void ViewPortDraw::on_zoom_in_click()
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

void ViewPortDraw::on_zoom_out_click()
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
        auto point = dynamic_cast<Point&>(shape);
        // auto x = std::get<0>(coordinates) + std::get<0>(point.coordinates);
        // auto y = std::get<1>(coordinates) + std::get<1>(point.coordinates);
        // auto z = std::get<2>(coordinates) + std::get<2>(point.coordinates);
        // auto new_coordinates = Coordinates{x, y, z};
        // shape = Point{x, y, z};
    } else if (shape.type() == "line") {

    } else if (shape.type() == "polygon") {

    }
    queue_draw();
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
