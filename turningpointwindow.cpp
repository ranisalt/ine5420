#include "turningpointwindow.h"
#include "mainwindow.h"

TurningPointWindow::TurningPointWindow(MainWindow &mainwindow)
: main_box(Gtk::ORIENTATION_VERTICAL)
, entry_box(Gtk::ORIENTATION_HORIZONTAL)
, x_label("X:"), y_label("Y:"), z_label("Z:")
, ok_button("OK")
, cancel_button("Cancel")
, mainwindow{mainwindow}
{
    add(main_box);
    set_title("Turning point");
    set_border_width(5);
    set_default_size(TurningPointWindow::width, TurningPointWindow::height);
    set_resizable(false);
    main_box.pack_start(entry_box, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(ok_button, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(cancel_button, Gtk::PACK_EXPAND_WIDGET);

    entry_box.pack_start(x_label, Gtk::PACK_EXPAND_WIDGET);
    entry_box.pack_start(x_entry, Gtk::PACK_EXPAND_WIDGET);
    entry_box.pack_start(y_label, Gtk::PACK_EXPAND_WIDGET);
    entry_box.pack_start(y_entry, Gtk::PACK_EXPAND_WIDGET);
    entry_box.pack_start(z_label, Gtk::PACK_EXPAND_WIDGET);
    entry_box.pack_start(z_entry, Gtk::PACK_EXPAND_WIDGET);
    ok_button.signal_button_release_event().connect(sigc::mem_fun(*this, &TurningPointWindow::ok_button_clicked));
    cancel_button.signal_button_release_event().connect(sigc::mem_fun(*this, &TurningPointWindow::cancel_button_clicked));

    show_all_children();
}

TurningPointWindow::~TurningPointWindow()
{

}

bool TurningPointWindow::ok_button_clicked(GdkEventButton* button_event)
{
    auto x_ = x_entry.get_text();
    auto y_ = y_entry.get_text();
    auto z_ = z_entry.get_text();
    if (!x_.empty() and !y_.empty() and !z_.empty()) {
        auto x = std::stod(x_entry.get_text());
        auto y = std::stod(y_entry.get_text());
        auto z = std::stod(z_entry.get_text());
        mainwindow.rotate(Coordinates{x, y, z});
        close();
    } else {
        Gtk::MessageDialog dialog(*this, "A new value must be set!",
            false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.run();
    }
    return true;
}

bool TurningPointWindow::cancel_button_clicked(GdkEventButton* button_event)
{
    x_entry.set_text("");
    y_entry.set_text("");
    z_entry.set_text("");
    close();
    return true;
}
