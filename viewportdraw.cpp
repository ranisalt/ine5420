#include "shapes.h"
#include "viewportdraw.h"

#include <cairomm/context.h>

ViewPortDraw::ViewPortDraw(): x_min{0}, y_min{0}
{
    Gtk::Allocation allocation = get_allocation();
    x_max = allocation.get_width();
    y_max = allocation.get_height();
}

ViewPortDraw::~ViewPortDraw()
{

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

    queue_draw();
}

void ViewPortDraw::on_zoom_out_click()
{
    constexpr auto DELTA = 0.05;

    auto w_width = (x_max - x_min) * DELTA;
    x_min -= w_width / 2;
    x_max += w_width / 2;

    auto w_height = (y_max - y_min) * DELTA;
    y_min -= w_height / 2;
    y_max += w_height / 2;

    queue_draw();
}

bool ViewPortDraw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
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

    cr->set_line_width(10.0);

    Shape point = Point{xc, yc};
    Shape line = Line{{0.0, 0.0}, {xc - 20, yc + 20}};

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);
    point.draw(cr, window);
    line.draw(cr, window);
    cr->stroke();

    return true;
}
