#ifndef GTKMM_AXISWINDOW_H
#define GTKMM_AXISWINDOW_H

class MainWindow;

#include "numericentry.h"
#include "shapes.h"

#include <gtkmm.h>

class AxisWindow : public Gtk::Window
{
public:
    explicit AxisWindow(MainWindow& mainwindow);
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);
    void set_axis(std::string axis);

protected:
    Gtk::Box axis_box;

    Gtk::Label axis_label;

    NumericEntry axis_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    MainWindow& mainwindow;

    static constexpr int width = 100;
    static constexpr int height = 150;
    std::string axis;
};

#endif
