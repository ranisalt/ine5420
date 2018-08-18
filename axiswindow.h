#ifndef GTKMM_AXISINDOW_H
#define GTKMM_AXISINDOW_H

#include <gtkmm.h>

#include "numericentry.h"

class AxisWindow : public Gtk::Window
{
public:
    AxisWindow();
    ~AxisWindow();
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);

protected:
    Gtk::Box axis_box;

    Gtk::Label axis_label;

    NumericEntry axis_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    static const int width = 100;
    static const int height = 150;
};

#endif
