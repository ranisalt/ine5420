#ifndef GTKMM_INCLUDEOBJECTWINDOW_H
#define GTKMM_INCLUDEOBJECTWINDOW_H

class MainWindow;

#include <gtkmm.h>

#include "numericentry.h"

class IncludeObjectWindow : public Gtk::Window
{
public:

    IncludeObjectWindow(MainWindow &mainwindow);
    virtual ~IncludeObjectWindow();

protected:
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);
    void create_box_point_tab();
    void create_box_line_tab();
    void create_box_wireframes_tab();
    void create_box_curves_tab();
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);

    Gtk::Notebook notebook;

    Gtk::Box notebook_box;
    Gtk::Box point_box;
    Gtk::Box line_box;
    Gtk::Box wireframes_box;
    Gtk::Box curves_box;
    Gtk::Box initial_coordinates_box;
    Gtk::Box final_coordinates_box;

    Gtk::Frame initial_coordinates_frame;
    Gtk::Frame final_coordinates_frame;

    Gtk::Label name_label;
    Gtk::Label x1_label;
    Gtk::Label y1_label;
    Gtk::Label z1_label;
    Gtk::Label x2_label;
    Gtk::Label y2_label;
    Gtk::Label z2_label;

    Gtk::Entry name_entry;
    NumericEntry x1_entry;
    NumericEntry y1_entry;
    NumericEntry z1_entry;
    NumericEntry x2_entry;
    NumericEntry y2_entry;
    NumericEntry z2_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    MainWindow &mainwindow;
    static const int width = 200;
    static const int height = 300;
};

#endif
