#ifndef VIEWPORTDRAW_H
#define VIEWPORTDRAW_H

#include "displayfile.h"
#include "matrixmanager.h"

#include <gtkmm.h>

class ViewPortDraw final : public Gtk::DrawingArea
{
public:
    void add_shape(std::string, Shape);
    void remove_shape(const std::string&);

    void on_zoom_in_click();
    void on_zoom_out_click();

    void up_click();
    void left_click();
    void down_click();
    void right_click();

    void translate(Coordinates coordinates, std::string shape_name);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_realize() override;

    DisplayFile df;
    // MatrixManager matrix;

    double pen_width = 2.0;
    double x_max, y_max, x_min, y_min;
};

#endif
