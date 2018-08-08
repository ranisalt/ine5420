#include "mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "INE5420");
    MainWindow window_;

    return app->run(window_);
}
