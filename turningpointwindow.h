#ifndef GTKMM_TURNING_POINT_WINDOW_H
#define GTKMM_TURNING_POINT_WINDOW_H

class MainWindow;

#include "numericentry.h"
#include "shapes.h"

#include <gtkmm.h>

class TurningPointWindow final: public Gtk::Window
{
public:
    explicit TurningPointWindow(MainWindow& mainwindow);
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);
    // void set_axis(std::string axis);

protected:
    void clear_fields();

    Gtk::Box main_box;
    Gtk::Box entry_box;

    Gtk::Label x_label;
    Gtk::Label y_label;
    Gtk::Label z_label;

    NumericEntry x_entry;
    NumericEntry y_entry;
    NumericEntry z_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    MainWindow& mainwindow;

    static constexpr int width = 100;
    static constexpr int height = 150;
};

#endif
