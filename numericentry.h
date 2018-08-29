#ifndef GTKMM_NUMERICENTRY_H
#define GTKMM_NUMERICENTRY_H

#include <gtkmm.h>

class NumericEntry final: public Gtk::Entry
{
public:
    explicit NumericEntry(bool is_int = true): is_int{is_int} {}

private:
    bool on_key_press_event(GdkEventKey* key_event);

    bool is_int;
};

#endif
