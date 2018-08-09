#ifndef VIEWPORTDRAW_H
#define VIEWPORTDRAW_H

#include <gtkmm.h>

class ViewPortDraw : public Gtk::DrawingArea
{
public:
    ViewPortDraw();
    virtual ~ViewPortDraw();

    void on_zoom_in_click();
    void on_zoom_out_click();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    double x_max, y_max, x_min, y_min;
};

#endif
