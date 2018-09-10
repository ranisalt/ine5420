#include "mainwindow.h"

#include "objstream.h"

#include <fstream>
#include <iostream>

MainWindow::MainWindow()
: main_box(Gtk::ORIENTATION_HORIZONTAL)
, function_box(Gtk::ORIENTATION_VERTICAL)
, window_box(Gtk::ORIENTATION_VERTICAL)
, window_operations_box(Gtk::ORIENTATION_HORIZONTAL)
, step_box(Gtk::ORIENTATION_VERTICAL)
, step_row1_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row2_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row3_box(Gtk::ORIENTATION_HORIZONTAL)
, step_row4_box(Gtk::ORIENTATION_HORIZONTAL)
, rotation_main_box(Gtk::ORIENTATION_VERTICAL)
, rotation1_box(Gtk::ORIENTATION_HORIZONTAL)
, rotation2_box(Gtk::ORIENTATION_HORIZONTAL)
, rotation3_box(Gtk::ORIENTATION_HORIZONTAL)
, translate_box(Gtk::ORIENTATION_HORIZONTAL)
, zoom_box(Gtk::ORIENTATION_HORIZONTAL)
, projection_box(Gtk::ORIENTATION_VERTICAL)
, viewport_box(Gtk::ORIENTATION_VERTICAL)
, function_frame("Functions menu")
, window_frame("Window")
, rotation_frame("Rotation")
, projection_frame("Projection")
, viewport_frame("Viewport")
, add_button("Add"), remove_button("Remove")
, load_button("Load .obj file")
, export_button("Export .obj file")
, turn_window_button("Turn window")
, up_button("Up"), left_button("Left")
, down_button("Down"), right_button("Right")
, in_button("In"), out_button("Out")
, apply_rotation_button("Apply")
, x_button("X"), y_button("Y")
, z_button("Z")
, up_scale_button("Up scale"), down_scale_button("Down scale")
, set_window_button("Set Window")
, step_label("Step:"), degree_label("Degree:")
, scale_label("Scale:"), translate_label("Translate:")
, liang_barsky_button("Liang Barsky")
, cohen_sutherland_button("Cohen Sutherland")
, parallel_radio_button("Parallel")
, perspective_radio_button("Perspective")
, popup{*this}
, axis_window{*this}
, tp_window{*this}
, point_dialog{*this}
, angle_window{*this}
{
    // Basic configuration for window
    set_title("INE5420 - SGI - Gustavo & Ranieri");
    set_border_width(5);
    set_default_size(MainWindow::window_width, MainWindow::window_height);
    text_buffer = Gtk::TextBuffer::create();
    add_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::add_button_clicked));
    remove_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::remove_button_clicked));
    load_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::load_button_clicked));
    export_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::export_button_clicked));
    console_text_view.set_buffer(text_buffer);
    set_resizable(false);

    main_box.set_homogeneous(false);
    perspective_radio_button.join_group(parallel_radio_button);
    liang_barsky_button.join_group(cohen_sutherland_button);
    liang_barsky_button.set_active();
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
    function_box.pack_start(load_button, Gtk::PACK_SHRINK);
    function_box.pack_start(export_button, Gtk::PACK_SHRINK);
    function_box.pack_start(window_frame, Gtk::PACK_EXPAND_WIDGET);

    // Create list of objects
    objects_refptr = Gtk::TreeStore::create(objects_records);
    objects_tree_view.set_model(objects_refptr);
    objects_tree_view.append_column("Objects", objects_records.object);
    objects_tree_view.signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_tree_view_row_activated));

    window_box.set_border_width(5);
    window_frame.add(window_box);
    // Add buttons
    window_box.pack_start(window_operations_box, Gtk::PACK_EXPAND_WIDGET);
    window_operations_box.pack_start(turn_window_button, Gtk::PACK_EXPAND_WIDGET);
    turn_window_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::turn_window_button_clicked));

    window_box.pack_start(step_box, Gtk::PACK_EXPAND_WIDGET);
    step_box.pack_start(step_row1_box, Gtk::PACK_EXPAND_WIDGET);
    step_row1_box.pack_start(liang_barsky_button, Gtk::PACK_EXPAND_WIDGET);
    step_row1_box.pack_start(cohen_sutherland_button, Gtk::PACK_EXPAND_WIDGET);
    liang_barsky_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::algorithm_button_clicked));
    cohen_sutherland_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::algorithm_button_clicked));

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
    window_box.pack_start(translate_box, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(zoom_box, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(set_window_button, Gtk::PACK_EXPAND_WIDGET);
    window_box.pack_start(projection_frame, Gtk::PACK_EXPAND_WIDGET);

    rotation_main_box.set_border_width(5);
    rotation_frame.add(rotation_main_box);
    // Add buttons
    rotation_main_box.pack_start(rotation1_box, Gtk::PACK_EXPAND_WIDGET);
    rotation_main_box.pack_start(rotation3_box, Gtk::PACK_EXPAND_WIDGET);
    rotation_main_box.pack_start(rotation2_box, Gtk::PACK_EXPAND_WIDGET);
    rotation1_box.pack_start(degree_label, Gtk::PACK_EXPAND_WIDGET);
    rotation1_box.pack_start(degree_entry, Gtk::PACK_SHRINK);
    rotation3_box.pack_start(apply_rotation_button, Gtk::PACK_EXPAND_WIDGET);

    translate_box.pack_start(translate_label, Gtk::PACK_EXPAND_WIDGET);
    translate_box.pack_start(x_button, Gtk::PACK_EXPAND_WIDGET);
    translate_box.pack_start(y_button, Gtk::PACK_EXPAND_WIDGET);
    translate_box.pack_start(z_button, Gtk::PACK_EXPAND_WIDGET);

    zoom_box.set_border_width(5);
    zoom_box.pack_start(scale_label, Gtk::PACK_EXPAND_WIDGET);
    zoom_box.pack_start(up_scale_button, Gtk::PACK_EXPAND_WIDGET);
    zoom_box.pack_start(down_scale_button, Gtk::PACK_EXPAND_WIDGET);

    in_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::on_in_click));
    out_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::on_out_click));

    up_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::up_click));
    left_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::left_click));
    down_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::down_click));
    right_button.signal_clicked().connect(sigc::mem_fun(&drawing_area, &ViewPortDraw::right_click));

    x_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::x_button_clicked));
    y_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::y_button_clicked));
    z_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::z_button_clicked));

    apply_rotation_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::apply_rotation_button_clicked));

    up_scale_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::up_scale_button_clicked));
    down_scale_button.signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::down_scale_button_clicked));

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

void MainWindow::show_tp_window()
{
    tp_window.show();
}

void MainWindow::rotate_window(double angle)
{
    drawing_area.rotate_window(angle);
}

void MainWindow::calculate_bezier(std::string name, std::vector<Coordinates> coordinates, double k)
{
    drawing_area.draw_curve_bezier(k, coordinates);
    auto row = *objects_refptr->append();
    row[objects_records.object] = "Bezier " + name;
    ++bezier_count;
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

bool MainWindow::x_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    if (row) {
        axis_window.set_axis("x");
        axis_window.show();
    } else {
        Gtk::MessageDialog dialog(*this, "A shape must be selected!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool MainWindow::y_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    if (row) {
        axis_window.set_axis("y");
        axis_window.show();
    } else {
        Gtk::MessageDialog dialog(*this, "A shape must be selected!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool MainWindow::z_button_clicked(GdkEventButton* button_event)
{
    // auto row = objects_tree_view.get_selection()->get_selected();
    // if (row) {
    //     axis_window.set_axis("z");
    //     axis_window.show();
    // } else {
    //     Gtk::MessageDialog dialog(*this, "A shape must be selected!",
    //         false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    //     dialog.run();
    // }
    return true;
}

bool MainWindow::up_scale_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    if (row) {
        const auto &name = objects_tree_view.get_selection()->get_selected()->get_value(objects_records.object);
        auto shape = drawing_area.get_shape_by_name(name);
        drawing_area.scale_up(shape, name);
    } else {
        Gtk::MessageDialog dialog(*this, "A shape must be selected!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool MainWindow::down_scale_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    if (row) {
        const auto &name = objects_tree_view.get_selection()->get_selected()->get_value(objects_records.object);
        auto shape = drawing_area.get_shape_by_name(name);
        drawing_area.scale_down(shape, name);
    } else {
        Gtk::MessageDialog dialog(*this, "A shape must be selected!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool MainWindow::apply_rotation_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    if (row) {
        const auto &name = objects_tree_view.get_selection()->get_selected()->get_value(objects_records.object);
        auto shape = drawing_area.get_shape_by_name(name);
        point_dialog.set_shape(shape);
        point_dialog.show();
    } else {
        Gtk::MessageDialog dialog(*this, "A shape must be selected!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool MainWindow::turn_window_button_clicked(GdkEventButton* button_event)
{
    angle_window.show();
    return true;
}

bool MainWindow::load_button_clicked(GdkEventButton* button_event)
{
    Gtk::FileChooserDialog dialog("Please choose a .obj file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Obj files");
    filter_text->add_pattern("*.obj");
    dialog.add_filter(filter_text);
    auto result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        auto filename = dialog.get_filename();
        std::ifstream object_file{filename};
        load_shapes_from_file(object_file);
    }
    return true;
}

bool MainWindow::export_button_clicked(GdkEventButton* button_event)
{
    Gtk::FileChooserDialog dialog("Please choose a .obj file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Export obj file");
    filter_text->add_pattern("*.obj");
    dialog.add_filter(filter_text);
    auto result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        auto filename = dialog.get_filename() + ".obj";
        std::ofstream object_file{filename};
        export_shapes_to_file(object_file);
    }
    return true;
}

bool MainWindow::algorithm_button_clicked(GdkEventButton* button_event)
{
    if (liang_barsky_button.get_active()) {
        drawing_area.set_algorithm(0);
    } else if (cohen_sutherland_button.get_active()) {
        drawing_area.set_algorithm(1);
    }

    return true;
}

bool MainWindow::remove_button_clicked(GdkEventButton* button_event)
{
    auto row = objects_tree_view.get_selection()->get_selected();
    const auto &name = row->get_value(objects_records.object);
    objects_refptr->erase(row);
    if (name.find("Bezier") != std::string::npos) {
        drawing_area.remove_bezier(name);
    } else {
        drawing_area.remove_shape(name);
    }
    return true;
}

void MainWindow::add_shape(std::string object_name, Shape s, bool queue_draw)
{
    auto row = *objects_refptr->append();
    row[objects_records.object] = object_name;
    drawing_area.calculate_normalized_coordinates(s);
    drawing_area.add_shape(std::move(object_name), std::move(s), queue_draw);
}

void MainWindow::load_shapes_from_file(std::istream& is)
{
    for (auto&& entry: parse_stream(is)) {
        add_shape(std::move(entry.first), std::move(entry.second), false);
    }
    drawing_area.queue_draw();
}

void MainWindow::export_shapes_to_file(std::ostream& os)
{
    dump_stream(drawing_area.display_file(), os);
}

void MainWindow::translate(Coordinates coordinates)
{
    const auto &name = objects_tree_view.get_selection()->get_selected()->get_value(objects_records.object);
    drawing_area.translate(coordinates, name);
}

void MainWindow::rotate(Coordinates coordinates)
{
    auto angle = std::stod(degree_entry.get_text());
    const auto &name = objects_tree_view.get_selection()->get_selected()->get_value(objects_records.object);
    auto shape = drawing_area.get_shape_by_name(name);
    drawing_area.rotate_acw(shape, name, coordinates, angle);
}
