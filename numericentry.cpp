#include <iostream>
#include "numericentry.h"

NumericEntry::NumericEntry()
{

}

NumericEntry::~NumericEntry()
{

}

bool NumericEntry::on_key_press_event(GdkEventKey* key_event)
{
    auto old_num = get_text();
    std::cout << key_event->keyval << std::endl;
    switch (key_event->keyval)
    {
      case GDK_KEY_0:
      case GDK_KEY_1:
      case GDK_KEY_2:
      case GDK_KEY_3:
      case GDK_KEY_4:
      case GDK_KEY_5:
      case GDK_KEY_6:
      case GDK_KEY_7:
      case GDK_KEY_8:
      case GDK_KEY_9:
          set_text(old_num + std::to_string(key_event->keyval - GDK_KEY_0));
          break;
      case 65288:
          set_text(old_num.substr(0, old_num.size()-1));
          break;
    }
    set_position(get_text().size());
    return false;
}
