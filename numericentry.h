#ifndef GTKMM_NUMERICENTRY_H
#define GTKMM_NUMERICENTRY_H

#include <gtkmm.h>
#include <string>

class NumericEntry : public Gtk::Entry
{
public:
    NumericEntry(bool is_int=true);
    virtual ~NumericEntry();

protected:
    virtual bool on_key_press_event(GdkEventKey* key_event);

    bool is_int;
};

#endif
