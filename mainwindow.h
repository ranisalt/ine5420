#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include "axiswindow.h"
#include "choosepointdialog.h"
#include "includeobjectwindow.h"
#include "newanglewindow.h"
#include "numericentry.h"
#include "shapes.h"
#include "turningpointwindow.h"
#include "viewportdraw.h"

#include <gtkmm.h>

#include <iosfwd>
#include <string>

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

    void add_shape(std::string, Shape, bool queue_draw = true);
    void load_shapes_from_file(std::istream&);
    void translate(Coordinates coordinates);
    void rotate(Coordinates coordinates);
    void show_tp_window();
    void rotate_window(double angle);

protected:
    bool add_button_clicked(GdkEventButton* button_event);
    bool remove_button_clicked(GdkEventButton* button_event);
    bool x_button_clicked(GdkEventButton* button_event);
    bool y_button_clicked(GdkEventButton* button_event);
    bool z_button_clicked(GdkEventButton* button_event);
    bool up_scale_button_clicked(GdkEventButton* button_event);
    bool down_scale_button_clicked(GdkEventButton* button_event);
    bool apply_rotation_button_clicked(GdkEventButton* button_event);
    bool turn_window_button_clicked(GdkEventButton* button_event);
    bool load_button_clicked(GdkEventButton* button_event);
    void on_tree_view_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void log(std::string l);

    class Column : public Gtk::TreeModel::ColumnRecord
    {
    public:

        Column()
        { add(object); }

        Gtk::TreeModelColumn<Glib::ustring> object;
    };

    Gtk::Box main_box;
    Gtk::Box function_box;
    Gtk::Box window_box;
    Gtk::Box window_operations_box;
    Gtk::Box step_box;
    Gtk::Box step_row1_box;
    Gtk::Box step_row2_box;
    Gtk::Box step_row3_box;
    Gtk::Box step_row4_box;
    Gtk::Box rotation_main_box;
    Gtk::Box rotation1_box;
    Gtk::Box rotation2_box;
    Gtk::Box rotation3_box;
    Gtk::Box translate_box;
    Gtk::Box zoom_box;
    Gtk::Box projection_box;
    Gtk::Box viewport_box;
    Gtk::Box console_box;

    Gtk::Frame function_frame;
    Gtk::Frame window_frame;
    Gtk::Frame rotation_frame;
    Gtk::Frame projection_frame;
    Gtk::Frame viewport_frame;

    Column objects_records;
    Gtk::ScrolledWindow objects_scrolled_window;
    Gtk::ScrolledWindow console_scrolled_window;
    Gtk::TreeView objects_tree_view;
    Glib::RefPtr<Gtk::TreeStore> objects_refptr;

    Gtk::Button add_button;
    Gtk::Button remove_button;
    Gtk::Button load_button;
    Gtk::Button turn_window_button;
    Gtk::Button up_button;
    Gtk::Button left_button;
    Gtk::Button down_button;
    Gtk::Button right_button;
    Gtk::Button in_button;
    Gtk::Button out_button;
    Gtk::Button apply_rotation_button;
    Gtk::Button x_button;
    Gtk::Button y_button;
    Gtk::Button z_button;
    Gtk::Button up_scale_button;
    Gtk::Button down_scale_button;
    Gtk::Button set_window_button;

    Gtk::Separator step_row3_box_separator;

    Gtk::Label step_label;
    Gtk::Label degree_label;
    Gtk::Label scale_label;
    Gtk::Label translate_label;

    ViewPortDraw drawing_area;

    Gtk::TextView console_text_view;
    Glib::RefPtr<Gtk::TextBuffer> text_buffer;

    NumericEntry step_entry;
    NumericEntry degree_entry;

    Gtk::RadioButton parallel_radio_button;
    Gtk::RadioButton perspective_radio_button;

    IncludeObjectWindow popup;
    AxisWindow axis_window;
    TurningPointWindow tp_window;
    ChoosePointDialog point_dialog;
    NewAngleWindow angle_window;

    std::map<Gtk::TreeModel::iterator, std::string> shape_objects;

    static const int window_width = 700;
    static const int window_height = 550;

    static const int console_width = 60;
    static const int console_height = 65;
 };

#endif
