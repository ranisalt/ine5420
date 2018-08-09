#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow()
: main_box(Gtk::ORIENTATION_HORIZONTAL)
, function_box(Gtk::ORIENTATION_VERTICAL)
, window_box(Gtk::ORIENTATION_VERTICAL)
, step_box(Gtk::ORIENTATION_VERTICAL)
, step_row1_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row2_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row3_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row4_box(Gtk::ORIENTATION_HORIZONTAL)
, rotation_main_box(Gtk::ORIENTATION_VERTICAL)
, rotation1_box(Gtk::ORIENTATION_HORIZONTAL)
, rotation2_box(Gtk::ORIENTATION_HORIZONTAL)
, zoom_box(Gtk::ORIENTATION_HORIZONTAL)
, projection_box(Gtk::ORIENTATION_VERTICAL)
, viewport_box(Gtk::ORIENTATION_VERTICAL)
, function_frame("Functions menu")
, window_frame("Window")
, rotation_frame("Rotation")
, projection_frame("Projection")
, viewport_frame("Viewport")
, add_button("Add"), remove_button("Remove")
, up_button("Up"), left_button("Left")
, down_button("Down"), right_button("Right")
, in_button("In"), out_button("Out")
, x_button("X"), y_button("Y")
, z_button("Z")
, zoom_in_button("+"), zoom_out_button("-")
, set_window_button("Set Window")
, step_label("Step:"), degree_label("Degree:")
, zoom_label("Zoom:")
, parallel_radio_button("Parallel")
, perspective_radio_button("Perspective")
, popup{*this}
{
    // Basic configuration for window
    set_title("INE5420 - SGI - Gustavo & Ranieri");
    set_border_width(5);
    set_default_size(MainWindow::window_width, MainWindow::window_height);
    text_buffer = Gtk::TextBuffer::create();
    add_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::add_button_clicked));
    remove_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::remove_button_clicked));
    degree_entry.set_input_purpose(Gtk::INPUT_PURPOSE_PASSWORD);
    console_text_view.set_buffer(text_buffer);
    set_resizable(false);

    main_box.set_homogeneous(false);
    perspective_radio_button.join_group(parallel_radio_button);
    objects_scrolled_window.add(objects_tree_view);
    objects_scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    console_scrolled_window.add(console_text_view);
    console_scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    console_text_view.set_editable(false);

    add(main_box);
    // Add boxes
    main_box.pack_start(function_box, Gtk::PACK_SHRINK, true, false);
    main_box.pack_start(viewport_box, Gtk::PACK_EXPAND_WIDGET, true, true);
    function_box.pack_start(objects_scrolled_window);
    function_box.set_border_width(5);
    function_frame.add(function_box);
    function_box.pack_start(add_button, Gtk::PACK_SHRINK);
    function_box.pack_start(remove_button, Gtk::PACK_SHRINK);
    function_box.pack_start(window_frame, Gtk::PACK_EXPAND_WIDGET);

    // Create list of objects
    objects_refptr = Gtk::TreeStore::create(objects_records);
    objects_tree_view.set_model(objects_refptr);
    objects_tree_view.append_column("Objects", objects_records.object);
    objects_tree_view.signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_tree_view_row_activated));

    window_box.set_border_width(5);
    window_frame.add(window_box);
    // Add buttons
    window_box.pack_start(step_box, Gtk::PACK_EXPAND_WIDGET);
    step_box.pack_start(step_row1_box, Gtk::PACK_EXPAND_WIDGET);
    step_row1_box.pack_start(step_label, Gtk::PACK_EXPAND_WIDGET, 10);
    step_row1_box.pack_start(step_entry, Gtk::PACK_EXPAND_WIDGET);

    step_box.pack_start(step_row2_box, Gtk::PACK_EXPAND_WIDGET);
    step_row2_box.pack_start(up_button, Gtk::PACK_EXPAND_WIDGET, 20);
    step_row2_box.pack_start(in_button, Gtk::PACK_EXPAND_WIDGET, 20);

    step_box.pack_start(step_row3_box, Gtk::PACK_EXPAND_WIDGET);
    step_row3_box.pack_start(left_button, Gtk::PACK_EXPAND_WIDGET);
    step_row3_box.pack_start(right_button, Gtk::PACK_EXPAND_WIDGET);
    step_row3_box.pack_start(step_row3_box_separator, Gtk::PACK_EXPAND_WIDGET);
    step_row3_box_separator.set_size_request(95, 0);

    step_box.pack_start(step_row4_box, Gtk::PACK_EXPAND_WIDGET);
    step_row4_box.pack_start(down_button, Gtk::PACK_EXPAND_WIDGET, 20);
    step_row4_box.pack_start(out_button, Gtk::PACK_EXPAND_WIDGET, 20);

    window_box.pack_start(rotation_frame, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(zoom_box, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(set_window_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(projection_frame, Gtk::PACK_EXPAND_WIDGET);

    rotation_main_box.set_border_width(5);
    rotation_frame.add(rotation_main_box);
    // Add buttons
    rotation_main_box.pack_start(rotation1_box, Gtk::PACK_EXPAND_WIDGET);
    rotation_main_box.pack_start(rotation2_box, Gtk::PACK_EXPAND_WIDGET);
    rotation1_box.pack_start(degree_label, Gtk::PACK_EXPAND_WIDGET);
    rotation1_box.pack_start(degree_entry, Gtk::PACK_EXPAND_WIDGET);
    rotation2_box.pack_start(x_button, Gtk::PACK_EXPAND_WIDGET);
    rotation2_box.pack_start(y_button, Gtk::PACK_EXPAND_WIDGET);
    rotation2_box.pack_start(z_button, Gtk::PACK_EXPAND_WIDGET);

    zoom_box.set_border_width(5);
    zoom_box.pack_start(zoom_label, Gtk::PACK_EXPAND_WIDGET);
    zoom_box.pack_start(zoom_in_button, Gtk::PACK_EXPAND_WIDGET);
    zoom_box.pack_start(zoom_out_button, Gtk::PACK_EXPAND_WIDGET);

    zoom_in_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::on_zoom_in_click));
    zoom_out_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::on_zoom_out_click));

    projection_box.set_border_width(5);
    projection_frame.add(projection_box);
    projection_box.pack_start(parallel_radio_button, Gtk::PACK_EXPAND_WIDGET);
    projection_box.pack_start(perspective_radio_button, Gtk::PACK_EXPAND_WIDGET);

    viewport_box.set_border_width(5);
    viewport_box.set_homogeneous(false);
    viewport_frame.add(viewport_box);
    // viewport_box.pack_start(drawing_area, Gtk::PACK_EXPAND_WIDGET, true, true);
    viewport_box.pack_start(viewport_frame, Gtk::PACK_EXPAND_WIDGET, true, true);
    viewport_frame.add(drawing_area);
    viewport_box.pack_start(console_scrolled_window, false, true);
    console_scrolled_window.set_size_request(MainWindow::console_width, MainWindow::console_height);

    show_all_children();
    // add_events(Gdk::KEY_PRESS_MASK);
    // up_button.signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::key_pressed));

}

MainWindow::~MainWindow()
{

}

void MainWindow::on_tree_view_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
}

void MainWindow::log(std::string l)
{
    auto old_text = text_buffer->get_text() + '\n';
    text_buffer->set_text(old_text + l);
}

bool MainWindow::add_button_clicked(GdkEventButton* button_event)
{
    popup.show();
    return true;
}

bool MainWindow::remove_button_clicked(GdkEventButton* button_event)
{
    return true;
}

void MainWindow::add_shape(Shape s, std::string object_name)
{
    // Lines to add a new object in view
    Gtk::TreeModel::Row row = *(objects_refptr->append());
    row[objects_records.object] = std::move(object_name);

    drawing_area.add_shape(std::move(s));
}
