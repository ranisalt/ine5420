#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    void on_tree_view_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

    class Column : public Gtk::TreeModel::ColumnRecord
    {
    public:

        Column()
        { add(object); }

        Gtk::TreeModelColumn<Glib::ustring> object;
    };

    Gtk::Box function_box;
    Gtk::Box window_box;
    Gtk::Box rotation_box;
    Gtk::Box projection_box;

    Gtk::Frame function_frame;
    Gtk::Frame window_frame;
    Gtk::Frame rotation_frame;
    Gtk::Frame projection_frame;

    Column objects_records;
    Gtk::ScrolledWindow objects_scrolled_window;
    Gtk::TreeView objects_tree_view;
    Glib::RefPtr<Gtk::TreeStore> objects_refptr;

    Gtk::Button up_button;
    Gtk::Button left_button;
    Gtk::Button down_button;
    Gtk::Button right_button;
    Gtk::Button in_button;
    Gtk::Button out_button;
    Gtk::Button x_button;
    Gtk::Button y_button;
    Gtk::Button z_button;

    Gtk::Label step_label;
    Gtk::Label degree_label;
    Gtk::Label zoom_label;

    Gtk::Entry step_entry;
    Gtk::Entry degree_entry;

    Gtk::RadioButton parallel_radio_button;
    Gtk::RadioButton perspective_radio_button;

    static const int width = 600;
    static const int height = 450;
 };

#endif
