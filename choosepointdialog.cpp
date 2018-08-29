#include <iostream>
#include "choosepointdialog.h"
#include "mainwindow.h"

ChoosePointDialog::ChoosePointDialog(MainWindow &mainwindow)
: main_box(Gtk::ORIENTATION_VERTICAL)
, point_label("Please, select a type of point")
, center_world_button("Center world point")
, center_shape_button("Center shape point")
, arbitrary_button("Arbitrary point")
, mainwindow{mainwindow}
, shape{shape}
, viewportdraw{viewportdraw}

{
    add(main_box);
    set_title("Set type point");
    set_border_width(5);
    set_default_size(ChoosePointDialog::width, ChoosePointDialog::height);
    set_resizable(false);
    main_box.pack_start(point_label, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(center_world_button, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(center_shape_button, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(arbitrary_button, Gtk::PACK_EXPAND_WIDGET);

    center_world_button.signal_button_release_event().connect(sigc::mem_fun(*this, &ChoosePointDialog::center_world_button_clicked));
    center_shape_button.signal_button_release_event().connect(sigc::mem_fun(*this, &ChoosePointDialog::center_shape_button_clicked));
    arbitrary_button.signal_button_release_event().connect(sigc::mem_fun(*this, &ChoosePointDialog::arbitrary_button_clicked));

    show_all_children();
}

ChoosePointDialog::~ChoosePointDialog()
{

}

bool ChoosePointDialog::center_world_button_clicked(GdkEventButton* button_event)
{
    auto coordinates = Coordinates{0, 0, 0};
    mainwindow.rotate(coordinates);
    close();
    return true;
}

bool ChoosePointDialog::center_shape_button_clicked(GdkEventButton* button_event)
{
    auto center = matrix::calculate_center_of_polygon(shape.coordinates());
    mainwindow.rotate(center);
    close();
    return true;
}

bool ChoosePointDialog::arbitrary_button_clicked(GdkEventButton* button_event)
{
    close();
    mainwindow.show_tp_window();
    return true;
}

void ChoosePointDialog::set_shape(Shape s)
{
    shape = s;
}
