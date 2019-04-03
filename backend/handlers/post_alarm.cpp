#include "post_alarm.h"
#include <iostream>
#include <string>

void postAlarm(std::string macAddressOfTag) {
  std::cout << "Alarm to '" << macAddressOfTag << "' was invoked" << std::endl;
}