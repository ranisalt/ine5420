#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow()
: up_button("Up"), left_button("Left")
, down_button("Down"), right_button("right")
, in_button("In"), out_button("Out")
, function_frame("Functions menu")
, window_frame("Window")
, rotation_frame("Rotation")
, projection_frame("Projection")
, parallel_radio_button("Parallel")
, perspective_radio_button("Perspective")
, function_box(Gtk::ORIENTATION_VERTICAL)
, window_box(Gtk::ORIENTATION_VERTICAL)
, rotation_box(Gtk::ORIENTATION_VERTICAL)
, projection_box(Gtk::ORIENTATION_VERTICAL)
{
    set_title("INE5420 - SGI - Gustavo & Ranieri");
    set_border_width(5);
    set_default_size(MainWindow::width, MainWindow::height);
    set_resizable(false);

    perspective_radio_button.join_group(parallel_radio_button);
    objects_scrolled_window.add(objects_tree_view);
    objects_scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    add(function_frame);
    function_box.pack_start(objects_scrolled_window);
    function_box.set_border_width(10);
    function_frame.add(function_box);
    function_box.pack_start(window_frame, Gtk::PACK_EXPAND_WIDGET, 10);
    function_box.pack_start(projection_frame, Gtk::PACK_EXPAND_WIDGET, 10);

    objects_refptr = Gtk::TreeStore::create(objects_records);
    objects_tree_view.set_model(objects_refptr);
    objects_tree_view.append_column("Objects", objects_records.object);
    objects_tree_view.signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_tree_view_row_activated));

    window_box.set_border_width(10);
    window_frame.add(window_box);
    window_box.pack_start(left_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(down_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(right_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(up_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(in_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(out_button, Gtk::PACK_EXPAND_WIDGET);

    projection_box.set_border_width(10);
    projection_frame.add(projection_box);
    projection_box.pack_start(parallel_radio_button, Gtk::PACK_EXPAND_WIDGET);
    projection_box.pack_start(perspective_radio_button, Gtk::PACK_EXPAND_WIDGET);

    show_all_children();

    // add_events(Gdk::KEY_PRESS_MASK);
    // up_button.signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::key_pressed));

    // Lines to add a new object in view
    // Gtk::TreeModel::Row row = *(objects_refptr->append());
    // row[objects_records.object] = "Teste";
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_tree_view_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
    std::cout << "Hello, there!" << std::endl;
}
