#ifndef VIEWPORTDRAW_H
#define VIEWPORTDRAW_H

#include "displayfile.h"
#include "matrix.h"

#include <gtkmm.h>

class ViewPortDraw final : public Gtk::DrawingArea
{
public:
    void add_shape(std::string, Shape, bool queue_draw = true);
    void remove_shape(const std::string&);

    void on_in_click();
    void on_out_click();

    void up_click();
    void left_click();
    void down_click();
    void right_click();

    void translate(Coordinates coordinates, std::string shape_name);
    void scale_up(Shape s, std::string shape_name);
    void scale_down(Shape s, std::string shape_name);
    void rotate_acw(Shape s, std::string shape_name, Coordinates point, double angle);
    void calculate_normalized_coordinates(Shape& s);
    void rotate_window(double angle);

    Shape get_shape_by_name(std::string shape_name);
    const DisplayFile& display_file() const { return df; }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void draw_new_shape(std::string shape_name, std::string type_of_shape, std::vector<Coordinates> new_coordinates);
    void on_realize() override;
    void clipping(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape s);
    void clip_point(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape p);
    void clip_liang_barsky(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape l);
    void clip_nicholl_lee_nicholl(const Cairo::RefPtr<Cairo::Context>& ctx, const WindowMapping& window, Shape l);

    DisplayFile df;

    double total_angle_window = 0;
    double pen_width = 2.0;
    const double pen_width_cp = 2.0;
    double x_max, y_max, x_min, y_min;
    double x_max_cp, y_max_cp, x_min_cp, y_min_cp;
};

#endif
