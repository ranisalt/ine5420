#include "axiswindow.h"

AxisWindow::AxisWindow()
: axis_box(Gtk::ORIENTATION_VERTICAL)
, axis_label("Please, select a new value for the axis:")
, ok_button("OK")
, cancel_button("Cancel")
{
    add(axis_box);
    set_title("New axis valeu");
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
    return true;
}

bool AxisWindow::cancel_button_clicked(GdkEventButton* button_event)
{
    close();
    return true;
}
