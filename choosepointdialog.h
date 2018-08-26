#ifndef GTKMM_CHOOSEPOINTDIALOG_H
#define GTKMM_CHOOSEPOINTDIALOG_H

class MainWindow;

#include "viewportdraw.h"
#include "shapes.h"

#include <gtkmm.h>

class ChoosePointDialog : public Gtk::Window
{
public:
    ChoosePointDialog(MainWindow &mainwindow);
    virtual ~ChoosePointDialog();
    bool center_world_button_clicked(GdkEventButton* button_event);
    bool center_shape_button_clicked(GdkEventButton* button_event);
    bool arbitrary_button_clicked(GdkEventButton* button_event);
    void set_shape(Shape s);

protected:
    Gtk::Box main_box;

    Gtk::Label point_label;

    Gtk::Button center_world_button;
    Gtk::Button center_shape_button;
    Gtk::Button arbitrary_button;

    MainWindow &mainwindow;
    Shape &shape;
    ViewPortDraw &viewportdraw;

    static const int width = 100;
    static const int height = 150;
};

#endif
