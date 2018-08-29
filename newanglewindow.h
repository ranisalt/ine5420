#ifndef GTKMM_NEWANGLEWINDOW_H
#define GTKMM_NEWANGLEWINDOW_H

class MainWindow;

#include "numericentry.h"
#include "shapes.h"

#include <gtkmm.h>

class NewAngleWindow final: public Gtk::Window
{
public:
    explicit NewAngleWindow(MainWindow& mainwindow);
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);

private:
    Gtk::Box angle_box;

    Gtk::Label angle_label;

    NumericEntry angle_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    MainWindow& mainwindow;

    static constexpr int width = 100;
    static constexpr int height = 150;
    std::string axis;
};

#endif
