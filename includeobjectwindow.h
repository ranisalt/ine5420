#ifndef GTKMM_INCLUDEOBJECTWINDOW_H
#define GTKMM_INCLUDEOBJECTWINDOW_H

class MainWindow;

#include <gtkmm.h>

#include "coordinates.h"
#include "numericentry.h"

class IncludeObjectWindow final: public Gtk::Window
{
public:
    explicit IncludeObjectWindow(MainWindow &mainwindow);

private:
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);
    void create_box_point_tab();
    void create_box_line_tab();
    void create_box_wireframes_tab();
    void clear_fields();
    bool ok_button_clicked(GdkEventButton* button_event);
    bool cancel_button_clicked(GdkEventButton* button_event);
    bool validate_point();
    bool validate_line();
    bool validate_wireframe();
    bool validate_curve();

    Gtk::Notebook notebook;

    Gtk::Box name_box;
    Gtk::Box notebook_box;
    Gtk::Box point_box;
    Gtk::Box line_box;
    Gtk::Box wireframes_box;
    Gtk::Box curve_box;
    Gtk::Box initial_coordinates_box;
    Gtk::Box final_coordinates_box;
    Gtk::Box buttons_box;

    Gtk::Frame point_frame;
    Gtk::Frame initial_coordinates_frame;
    Gtk::Frame final_coordinates_frame;
    Gtk::Frame wireframes_frame;

    Gtk::Label name_label;
    Gtk::Label x1_point_label;
    Gtk::Label y1_point_label;
    Gtk::Label z1_point_label;
    Gtk::Label x1_line_label;
    Gtk::Label y1_line_label;
    Gtk::Label z1_line_label;
    Gtk::Label x2_line_label;
    Gtk::Label y2_line_label;
    Gtk::Label z2_line_label;
    Gtk::Label wireframes_message_label;
    Gtk::Label x1_wireframes_label;
    Gtk::Label y1_wireframes_label;
    Gtk::Label z1_wireframes_label;

    Gtk::Entry name_entry;
    NumericEntry x1_point_entry;
    NumericEntry y1_point_entry;
    NumericEntry z1_point_entry;
    NumericEntry x1_line_entry;
    NumericEntry y1_line_entry;
    NumericEntry z1_line_entry;
    NumericEntry x2_line_entry;
    NumericEntry y2_line_entry;
    NumericEntry z2_line_entry;
    NumericEntry x1_wireframes_entry;
    NumericEntry y1_wireframes_entry;
    NumericEntry z1_wireframes_entry;

    std::vector<std::array<NumericEntry, 3>> curve_entries;


    Gtk::Button ok_button;
    Gtk::Button cancel_button;

    Gtk::Separator button_separator;

    MainWindow& mainwindow;
    std::vector<Coordinates> wireframes_points;
    static constexpr int width = 300;
    static constexpr int height = 340;
};

#endif
