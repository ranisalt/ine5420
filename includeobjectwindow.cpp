#include <iostream>
#include "includeobjectwindow.h"

IncludeObjectWindow::IncludeObjectWindow()
: notebook_box(Gtk::ORIENTATION_VERTICAL)
, initial_coordinates_box(Gtk::ORIENTATION_HORIZONTAL)
, final_coordinates_box(Gtk::ORIENTATION_HORIZONTAL)
, point_box(Gtk::ORIENTATION_VERTICAL)
, line_box(Gtk::ORIENTATION_VERTICAL)
, wireframes_box(Gtk::ORIENTATION_VERTICAL)
, curves_box(Gtk::ORIENTATION_VERTICAL)
, initial_coordinates_frame("Initial point coordinates")
, final_coordinates_frame("Final point coordinates")
, name_label("Name"), x1_label("x1:")
, y1_label("y1:"), z1_label("z1:")
, x2_label("x2:"), y2_label("y2:")
, z2_label("z2")
, ok_button("OK")
, cancel_button("Cancel")
{
    // Basic configuration for window
    set_title("Include object");
    set_border_width(5);
    set_default_size(IncludeObjectWindow::width, IncludeObjectWindow::height);
    set_resizable(false);

    create_box_point_tab();
    create_box_line_tab();
    create_box_wireframes_tab();
    create_box_curves_tab();

    add(notebook_box);

    notebook.set_border_width(10);
    notebook_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET, 10);
    notebook_box.pack_start(ok_button, Gtk::PACK_EXPAND_WIDGET, 10);
    notebook_box.pack_start(cancel_button, Gtk::PACK_EXPAND_WIDGET, 10);
    notebook.append_page(point_box, "Point");
    notebook.append_page(line_box, "Line");
    notebook.append_page(wireframes_box, "Wireframe");
    notebook.append_page(curves_box, "Curves");
    notebook.signal_switch_page().connect(sigc::mem_fun(
        *this, &IncludeObjectWindow::on_notebook_switch_page));

    show_all_children();
}

IncludeObjectWindow::~IncludeObjectWindow()
{

}

void IncludeObjectWindow::on_notebook_switch_page(Gtk::Widget* page, guint page_num)
{

}

void IncludeObjectWindow::create_box_point_tab()
{
    // TODO
}

void IncludeObjectWindow::create_box_line_tab()
{
    line_box.pack_start(initial_coordinates_frame, Gtk::PACK_EXPAND_WIDGET, 10);
    line_box.pack_start(final_coordinates_frame, Gtk::PACK_EXPAND_WIDGET, 10);

    initial_coordinates_box.set_border_width(0);
    initial_coordinates_frame.add(initial_coordinates_box);
    initial_coordinates_box.pack_start(x1_label, Gtk::PACK_EXPAND_WIDGET, 10);
    initial_coordinates_box.pack_start(x1_entry, Gtk::PACK_EXPAND_WIDGET, 10);
    initial_coordinates_box.pack_start(y1_label, Gtk::PACK_EXPAND_WIDGET, 10);
    initial_coordinates_box.pack_start(y1_entry, Gtk::PACK_EXPAND_WIDGET, 10);
    initial_coordinates_box.pack_start(z1_label, Gtk::PACK_EXPAND_WIDGET, 10);
    initial_coordinates_box.pack_start(z1_entry, Gtk::PACK_EXPAND_WIDGET, 10);

    final_coordinates_box.set_border_width(0);
    final_coordinates_frame.add(final_coordinates_box);
    final_coordinates_box.pack_start(x2_label, Gtk::PACK_EXPAND_WIDGET, 10);
    final_coordinates_box.pack_start(x2_entry, Gtk::PACK_EXPAND_WIDGET, 10);
    final_coordinates_box.pack_start(y2_label, Gtk::PACK_EXPAND_WIDGET, 10);
    final_coordinates_box.pack_start(y2_entry, Gtk::PACK_EXPAND_WIDGET, 10);
    final_coordinates_box.pack_start(z2_label, Gtk::PACK_EXPAND_WIDGET, 10);
    final_coordinates_box.pack_start(z2_entry, Gtk::PACK_EXPAND_WIDGET, 10);
}

void IncludeObjectWindow::create_box_wireframes_tab()
{
  // TODO
}

void IncludeObjectWindow::create_box_curves_tab()
{
  // TODO
}
