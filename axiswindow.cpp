#include "axiswindow.h"
#include "mainwindow.h"

AxisWindow::AxisWindow(MainWindow &mainwindow)
: axis_box(Gtk::ORIENTATION_VERTICAL)
, axis_label("Please, select a new value for the axis:")
, ok_button("OK")
, cancel_button("Cancel")
, mainwindow{mainwindow}
{
    add(axis_box);
    set_title("New axis value");
    set_border_width(5);
    set_default_size(AxisWindow::width, AxisWindow::height);
    set_resizable(false);
    axis_box.pack_start(axis_label, Gtk::PACK_EXPAND_WIDGET);
    axis_box.pack_start(axis_entry, Gtk::PACK_EXPAND_WIDGET);
    axis_box.pack_start(ok_button, Gtk::PACK_EXPAND_WIDGET);
    axis_box.pack_start(cancel_button, Gtk::PACK_EXPAND_WIDGET);

    ok_button.signal_button_release_event().connect(sigc::mem_fun(*this, &AxisWindow::ok_button_clicked));
    cancel_button.signal_button_release_event().connect(sigc::mem_fun(*this, &AxisWindow::cancel_button_clicked));

    show_all_children();
}

AxisWindow::~AxisWindow()
{

}

bool AxisWindow::ok_button_clicked(GdkEventButton* button_event)
{
    auto value = axis_entry.get_text();
    if (!value.empty()) {
        if (axis == "x") {
            auto x = std::stod(value);
            mainwindow.translate(Coordinates{x, 0, 0});
        } else if (axis == "y") {
            auto y = std::stod(value);
            mainwindow.translate(Coordinates{0, y, 0});
        }
        close();
    } else {
        Gtk::MessageDialog dialog(*this, "A new value must be set!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool AxisWindow::cancel_button_clicked(GdkEventButton* button_event)
{
    axis_entry.set_text("");
    close();
    return true;
}

void AxisWindow::set_axis(std::string axis)
{
    this->axis = axis;
}
