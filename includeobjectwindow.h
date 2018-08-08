#ifndef GTKMM_INCLUDEOBJECTWINDOW_H
#define GTKMM_INCLUDEOBJECTWINDOW_H

#include <gtkmm.h>

class IncludeObjectWindow : public Gtk::Window
{
public:

    IncludeObjectWindow();
    virtual ~IncludeObjectWindow();

protected:
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);
    void create_box_point_tab();
    void create_box_line_tab();
    void create_box_wireframes_tab();
    void create_box_curves_tab();

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

    Gtk::Entry x1_entry;
    Gtk::Entry y1_entry;
    Gtk::Entry z1_entry;
    Gtk::Entry x2_entry;
    Gtk::Entry y2_entry;
    Gtk::Entry z2_entry;

    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    static const int width = 300;
    static const int height = 200;
};

#endif
