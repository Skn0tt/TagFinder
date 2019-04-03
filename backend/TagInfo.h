#pragma once
#include <string>

struct TagInfo {
  std::string name;
  std::string IP;
  bool isConnected;
  std::string macAddress;
  int lastSeen;
};