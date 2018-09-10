#include <iostream>

#include "includeobjectwindow.h"
#include "mainwindow.h"

IncludeObjectWindow::IncludeObjectWindow(MainWindow &mainwindow)
: name_box(Gtk::ORIENTATION_HORIZONTAL)
, notebook_box(Gtk::ORIENTATION_VERTICAL)
, point_box(Gtk::ORIENTATION_HORIZONTAL)
, line_box(Gtk::ORIENTATION_VERTICAL)
, wireframes_box(Gtk::ORIENTATION_HORIZONTAL)
, curve_box(Gtk::ORIENTATION_VERTICAL)
, initial_coordinates_box(Gtk::ORIENTATION_HORIZONTAL)
, final_coordinates_box(Gtk::ORIENTATION_HORIZONTAL)
, curve_box_1(Gtk::ORIENTATION_HORIZONTAL)
, curve_box_2(Gtk::ORIENTATION_HORIZONTAL)
, curve_box_3(Gtk::ORIENTATION_HORIZONTAL)
, curve_box_4(Gtk::ORIENTATION_HORIZONTAL)
, buttons_box(Gtk::ORIENTATION_HORIZONTAL)
, point_frame("Point coordinates")
, initial_coordinates_frame("Initial point coordinates")
, final_coordinates_frame("Final point coordinates")
, wireframes_frame("Wireframes coordinates")
, name_label("Name")
, x1_point_label("x1:"), y1_point_label("y1:")
, z1_point_label("z1:"), x1_line_label("x1:")
, y1_line_label("y1:"), z1_line_label("z1:")
, x2_line_label("x2:"), y2_line_label("y2:")
, z2_line_label("z2:")
, wireframes_message_label("To stop to add points, leave all fields in blank")
, x1_wireframes_label("x1:")
, y1_wireframes_label("y1:"), z1_wireframes_label("z1:")
, x1_curve_label("x1:")
, y1_curve_label("y1:"), z1_curve_label("z1:")
, x2_curve_label("x2:")
, y2_curve_label("y2:"), z2_curve_label("z2:")
, x3_curve_label("x3:")
, y3_curve_label("y3:"), z3_curve_label("z3:")
, x4_curve_label("x4:")
, y4_curve_label("y4:"), z4_curve_label("z4:")
, x1_line_entry(false)
, y1_line_entry(false)
, z1_line_entry(false)
, x2_line_entry(false)
, y2_line_entry(false)
, z2_line_entry(false)
, x1_curve_entry(false)
, y1_curve_entry(false)
, z1_curve_entry(false)
, x2_curve_entry(false)
, y2_curve_entry(false)
, z2_curve_entry(false)
, x3_curve_entry(false)
, y3_curve_entry(false)
, z3_curve_entry(false)
, x4_curve_entry(false)
, y4_curve_entry(false)
, z4_curve_entry(false)
, ok_button("OK")
, cancel_button("Cancel")
, mainwindow{mainwindow}
{
    // Basic configuration for window
    set_title("Include object");
    set_border_width(5);
    set_default_size(IncludeObjectWindow::width, IncludeObjectWindow::height);
    set_resizable(false);

    ok_button.signal_button_release_event().connect(sigc::mem_fun(*this, &IncludeObjectWindow::ok_button_clicked));
    cancel_button.signal_button_release_event().connect(sigc::mem_fun(*this, &IncludeObjectWindow::cancel_button_clicked));

    create_box_point_tab();
    create_box_line_tab();
    create_box_wireframes_tab();
    create_box_curves_tab();

    add(notebook_box);

    notebook.set_border_width(10);
    notebook_box.pack_start(name_box, Gtk::PACK_EXPAND_WIDGET, 10);
    name_box.set_homogeneous(false);
    name_box.pack_start(name_label, Gtk::PACK_EXPAND_WIDGET);
    name_box.pack_start(name_entry, Gtk::PACK_EXPAND_WIDGET);
    // name_line_entry.set_size_request(500,0);
    notebook_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET, 10);
    // notebook.set_size_request(10,10);
    notebook_box.pack_start(buttons_box, Gtk::PACK_EXPAND_WIDGET, 10);
    buttons_box.set_homogeneous(false);
    buttons_box.pack_start(cancel_button, Gtk::PACK_EXPAND_WIDGET, 10);
    buttons_box.pack_start(button_separator, Gtk::PACK_EXPAND_WIDGET, 10);
    buttons_box.pack_start(ok_button, Gtk::PACK_EXPAND_WIDGET, 10);
    // button_separator.set_size_request(70,0);
    notebook.append_page(point_box, "Point");
    notebook.append_page(line_box, "Line");
    notebook.append_page(wireframes_box, "Wireframe");
    notebook.append_page(curve_box, "Curves");
    notebook.signal_switch_page().connect(sigc::mem_fun(
        *this, &IncludeObjectWindow::on_notebook_switch_page));

    show_all_children();
}

void IncludeObjectWindow::on_notebook_switch_page(Gtk::Widget* page, guint page_num)
{
    clear_fields();

}

void IncludeObjectWindow::create_box_point_tab()
{
    // point_box.pack_start(point_frame, Gtk::PACK_EXPAND_WIDGET, 5);

    point_box.set_border_width(10);
    // point_frame.add(point_box);
    point_box.pack_start(x1_point_label, Gtk::PACK_EXPAND_WIDGET, 5);
    point_box.pack_start(x1_point_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    point_box.pack_start(y1_point_label, Gtk::PACK_EXPAND_WIDGET, 5);
    point_box.pack_start(y1_point_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    point_box.pack_start(z1_point_label, Gtk::PACK_EXPAND_WIDGET, 5);
    point_box.pack_start(z1_point_entry, Gtk::PACK_EXPAND_WIDGET, 5);
}

void IncludeObjectWindow::create_box_line_tab()
{
    line_box.pack_start(initial_coordinates_frame, Gtk::PACK_EXPAND_WIDGET, 15);
    line_box.pack_start(final_coordinates_frame, Gtk::PACK_EXPAND_WIDGET, 15);

    initial_coordinates_box.set_border_width(10);
    initial_coordinates_frame.add(initial_coordinates_box);
    initial_coordinates_box.set_homogeneous(false);
    initial_coordinates_box.pack_start(x1_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    initial_coordinates_box.pack_start(x1_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    initial_coordinates_box.pack_start(y1_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    initial_coordinates_box.pack_start(y1_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    initial_coordinates_box.pack_start(z1_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    initial_coordinates_box.pack_start(z1_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);

    final_coordinates_box.set_border_width(5);
    final_coordinates_frame.add(final_coordinates_box);
    final_coordinates_box.pack_start(x2_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    final_coordinates_box.pack_start(x2_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    final_coordinates_box.pack_start(y2_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    final_coordinates_box.pack_start(y2_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    final_coordinates_box.pack_start(z2_line_label, Gtk::PACK_EXPAND_WIDGET, 5);
    final_coordinates_box.pack_start(z2_line_entry, Gtk::PACK_EXPAND_WIDGET, 5);
}

void IncludeObjectWindow::create_box_wireframes_tab()
{
    // wireframes_box.pack_start(wireframes_frame, Gtk::PACK_EXPAND_WIDGET, 5);

    // wireframes_box.set_border_width(10);
    // wireframes_frame.add(wireframes_box);

    wireframes_box.pack_start(x1_wireframes_label, Gtk::PACK_EXPAND_WIDGET, 5);
    wireframes_box.pack_start(x1_wireframes_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    wireframes_box.pack_start(y1_wireframes_label, Gtk::PACK_EXPAND_WIDGET, 5);
    wireframes_box.pack_start(y1_wireframes_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    wireframes_box.pack_start(z1_wireframes_label, Gtk::PACK_EXPAND_WIDGET, 5);
    wireframes_box.pack_start(z1_wireframes_entry, Gtk::PACK_EXPAND_WIDGET, 5);
}

void IncludeObjectWindow::create_box_curves_tab()
{
    curve_box.pack_start(curve_box_1, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box.pack_start(curve_box_2, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box.pack_start(curve_box_3, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box.pack_start(curve_box_4, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(x1_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(x1_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(y1_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(y1_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(z1_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_1.pack_start(z1_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);

    curve_box_2.pack_start(x2_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_2.pack_start(x2_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_2.pack_start(y2_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_2.pack_start(y2_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_2.pack_start(z2_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_2.pack_start(z2_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);

    curve_box_3.pack_start(x3_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_3.pack_start(x3_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_3.pack_start(y3_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_3.pack_start(y3_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_3.pack_start(z3_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_3.pack_start(z3_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);

    curve_box_4.pack_start(x4_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_4.pack_start(x4_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_4.pack_start(y4_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_4.pack_start(y4_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_4.pack_start(z4_curve_label, Gtk::PACK_EXPAND_WIDGET, 5);
    curve_box_4.pack_start(z4_curve_entry, Gtk::PACK_EXPAND_WIDGET, 5);
}

void IncludeObjectWindow::clear_fields()
{
    name_entry.set_text("");

  x1_point_entry.set_text("");
  y1_point_entry.set_text("");
  z1_point_entry.set_text("");

  x1_line_entry.set_text("");
  y1_line_entry.set_text("");
  z1_line_entry.set_text("");
  x2_line_entry.set_text("");
  y2_line_entry.set_text("");
  z2_line_entry.set_text("");

  x1_wireframes_entry.set_text("");
  y1_wireframes_entry.set_text("");
  z1_wireframes_entry.set_text("");

  wireframes_points.clear();
}

bool IncludeObjectWindow::ok_button_clicked(GdkEventButton* button_event)
{
    double x1, y1, z1, x2, y2, z2;
    auto name = name_entry.get_text();
    bool is_valid;
    switch(notebook.get_current_page())
    {
        case 0:
            is_valid = validate_point();
            if (is_valid) {
                x1 = std::stod(x1_point_entry.get_text());
                y1 = std::stod(y1_point_entry.get_text());
                z1 = std::stod(z1_point_entry.get_text());
                mainwindow.add_shape(std::move(name), Point{x1, y1, z1});
                clear_fields();
                close();
            } else {
                Gtk::MessageDialog dialog(*this, "Every field must be filled!",
                    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
                dialog.run();
            }
            break;
        case 1:
            is_valid = validate_line();
            if (is_valid) {
                x1 = std::stod(x1_line_entry.get_text());
                y1 = std::stod(y1_line_entry.get_text());
                z1 = std::stod(z1_line_entry.get_text());

                x2 = std::stod(x2_line_entry.get_text());
                y2 = std::stod(y2_line_entry.get_text());
                z2 = std::stod(z2_line_entry.get_text());
                mainwindow.add_shape(std::move(name), Line{{x1, y1, z1}, {x2, y2, z2}});
                clear_fields();
                close();
            } else {
                Gtk::MessageDialog dialog(*this, "Every field must be filled!",
                    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
                dialog.run();
            }
            break;
        case 2:
            is_valid = validate_wireframe();
            if (is_valid) {
                auto t_x1 = x1_wireframes_entry.get_text();
                auto t_y1 = y1_wireframes_entry.get_text();
                auto t_z1 = z1_wireframes_entry.get_text();

                if (t_x1.empty() && t_y1.empty() && t_z1.empty()) {
                    mainwindow.add_shape(std::move(name), Polygon{wireframes_points});
                    clear_fields();
                    close();
                } else {
                    x1 = std::stod(x1_wireframes_entry.get_text());
                    y1 = std::stod(y1_wireframes_entry.get_text());
                    z1 = std::stod(z1_wireframes_entry.get_text());

                    wireframes_points.push_back({x1, y1, z1});

                    x1_wireframes_entry.set_text("");
                    y1_wireframes_entry.set_text("");
                    z1_wireframes_entry.set_text("");

                    Gtk::MessageDialog dialog(*this, "Point added!");
                    dialog.run();
                }
            } else {
                Gtk::MessageDialog dialog(*this, "Every field must be filled!",
                    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
                dialog.run();
            }
            break;
        case 3:
            is_valid = validate_curve();
            if (is_valid) {
                std::vector<Coordinates> coordinates;
                auto x1 = std::stod(x1_curve_entry.get_text());
                auto y1 = std::stod(y1_curve_entry.get_text());
                auto z1 = std::stod(z1_curve_entry.get_text());
                auto x2 = std::stod(x2_curve_entry.get_text());
                auto y2 = std::stod(y2_curve_entry.get_text());
                auto z2 = std::stod(z2_curve_entry.get_text());
                auto x3 = std::stod(x3_curve_entry.get_text());
                auto y3 = std::stod(y3_curve_entry.get_text());
                auto z3 = std::stod(z3_curve_entry.get_text());
                auto x4 = std::stod(x4_curve_entry.get_text());
                auto y4 = std::stod(y4_curve_entry.get_text());
                auto z4 = std::stod(z4_curve_entry.get_text());

                coordinates.push_back(Coordinates{x1, y1, z1});
                coordinates.push_back(Coordinates{x2, y2, z2});
                coordinates.push_back(Coordinates{x3, y3, z3});
                coordinates.push_back(Coordinates{x4, y4, z4});
                mainwindow.calculate_bezier(coordinates);
                clear_fields();
                close();
            } else {
                Gtk::MessageDialog dialog(*this, "Every field must be filled!",
                    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
                dialog.run();
            }
            break;
    }
    return true;
}

bool IncludeObjectWindow::cancel_button_clicked(GdkEventButton* button_event)
{
    clear_fields();
    close();
    return true;
}

bool IncludeObjectWindow::validate_point()
{
    auto name = name_entry.get_text();
    auto t_x1 = x1_point_entry.get_text();
    auto t_y1 = y1_point_entry.get_text();
    auto t_z1 = z1_point_entry.get_text();
    if (t_x1.empty() || t_y1.empty()
        || t_z1.empty() || name.empty()) {
        return false;
    } else {
        return true;
    }
}

bool IncludeObjectWindow::validate_line()
{
    auto name = name_entry.get_text();
    auto t_x1 = x1_line_entry.get_text();
    auto t_y1 = y1_line_entry.get_text();
    auto t_z1 = z1_line_entry.get_text();
    auto t_x2 = x2_line_entry.get_text();
    auto t_y2 = y2_line_entry.get_text();
    auto t_z2 = z2_line_entry.get_text();
    if (t_x1.empty() || t_y1.empty() || t_z1.empty()
        || t_x2.empty() || t_y2.empty() || t_z2.empty()
        || name.empty()) {
        return false;
    } else {
        return true;
    }
}

bool IncludeObjectWindow::validate_wireframe()
{
    auto name = name_entry.get_text();
    auto t_x1 = x1_wireframes_entry.get_text();
    auto t_y1 = y1_wireframes_entry.get_text();
    auto t_z1 = z1_wireframes_entry.get_text();

    if ((t_x1.empty() && t_y1.empty() && t_z1.empty()
        && !name.empty()) || (!t_x1.empty()
        && !t_y1.empty()  && !t_z1.empty()
        && !name.empty())) {
        return true;
    } else {
        return false;
    }
}

bool IncludeObjectWindow::validate_curve()
{
    auto name = name_entry.get_text();
    auto t_x1 = x1_curve_entry.get_text();
    auto t_y1 = y1_curve_entry.get_text();
    auto t_z1 = z1_curve_entry.get_text();
    auto t_x2 = x2_curve_entry.get_text();
    auto t_y2 = y2_curve_entry.get_text();
    auto t_z2 = z2_curve_entry.get_text();
    auto t_x3 = x3_curve_entry.get_text();
    auto t_y3 = y3_curve_entry.get_text();
    auto t_z3 = z3_curve_entry.get_text();
    auto t_x4 = x4_curve_entry.get_text();
    auto t_y4 = y4_curve_entry.get_text();
    auto t_z4 = z4_curve_entry.get_text();
    if (t_x1.empty() || t_y1.empty() || t_z1.empty()
        || t_x2.empty() || t_y2.empty() || t_z2.empty()
        || t_x3.empty() || t_y3.empty() || t_z3.empty()
        || t_x4.empty() || t_y4.empty() || t_z4.empty()
        || name.empty()) {
        return false;
    } else {
        return true;
    }
}
