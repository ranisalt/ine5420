#include "newanglewindow.h"
#include "mainwindow.h"

NewAngleWindow::NewAngleWindow(MainWindow &mainwindow)
: angle_box(Gtk::ORIENTATION_VERTICAL)
, angle_label("Please, select a new value for the angle:")
, ok_button("OK")
, cancel_button("Cancel")
, mainwindow{mainwindow}
{
    add(angle_box);
    set_title("New window angle value (in degrees)");
    set_border_width(5);
    set_default_size(NewAngleWindow::width, NewAngleWindow::height);
    set_resizable(false);
    angle_box.pack_start(angle_label, Gtk::PACK_EXPAND_WIDGET);
    angle_box.pack_start(angle_entry, Gtk::PACK_EXPAND_WIDGET);
    angle_box.pack_start(ok_button, Gtk::PACK_EXPAND_WIDGET);
    angle_box.pack_start(cancel_button, Gtk::PACK_EXPAND_WIDGET);

    ok_button.signal_button_release_event().connect(sigc::mem_fun(*this, &NewAngleWindow::ok_button_clicked));
    cancel_button.signal_button_release_event().connect(sigc::mem_fun(*this, &NewAngleWindow::cancel_button_clicked));

    show_all_children();
}

NewAngleWindow::~NewAngleWindow()
{

}

bool NewAngleWindow::ok_button_clicked(GdkEventButton* button_event)
{
    auto value = angle_entry.get_text();
    if (!value.empty()) {
        auto angle = std::stod(value);
            mainwindow.rotate_window(angle);
        close();
    } else {
        Gtk::MessageDialog dialog(*this, "A new value must be set!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool NewAngleWindow::cancel_button_clicked(GdkEventButton* button_event)
{
    angle_entry.set_text("");
    close();
    return true;
}
