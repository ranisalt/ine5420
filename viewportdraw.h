#ifndef VIEWPORTDRAW_H
#define VIEWPORTDRAW_H

#include <gtkmm.h>

class ViewPortDraw : public Gtk::DrawingArea
{
public:
    ViewPortDraw();
    virtual ~ViewPortDraw();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    double x_max, y_max, x_min, y_min;
};

#endif
