#include "viewportdraw.h"
#include <cairomm/context.h>

ViewPortDraw::ViewPortDraw()
{

}

ViewPortDraw::~ViewPortDraw()
{

}

bool ViewPortDraw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(10.0);

    Shape point = Point{xc, yc};
    Shape line = Line{{0.0, 0.0}, {xc - 20, yc + 20}};

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);
    point.draw(cr);
    line.draw(cr);
    cr->stroke();

    return true;
}
