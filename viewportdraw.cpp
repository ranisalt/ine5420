#include "shapes.h"
#include "viewportdraw.h"

#include <cairomm/context.h>

void ViewPortDraw::add_shape(Shape s)
{
    df.emplace(std::move(s));

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

    for (auto& s: df) {
        s.draw(cr, window);
    }
    cr->stroke();

    return true;
}

void ViewPortDraw::on_realize()
{
    auto allocation = get_allocation();
    x_min = 0;
    x_max = allocation.get_width();
    y_min = 0;
    y_max = allocation.get_height();

    Gtk::DrawingArea::on_realize();
}
