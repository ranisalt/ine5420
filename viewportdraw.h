#ifndef VIEWPORTDRAW_H
#define VIEWPORTDRAW_H

#include "displayfile.h"

#include <gtkmm.h>

class ViewPortDraw final : public Gtk::DrawingArea
{
public:
    void add_shape(Shape);

    void on_zoom_in_click();
    void on_zoom_out_click();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_realize() override;

    DisplayFile df;

    double pen_width = 2.0;
    double x_max, y_max, x_min, y_min;
};

#endif
