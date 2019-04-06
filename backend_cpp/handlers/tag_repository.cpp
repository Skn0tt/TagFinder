#include "../TagInfo.h"
#include <map>
#include <vector>
#include "tag_repository.h"
#include <iostream>

std::map<std::string, TagInfo> tags;

void addTag(TagInfo newTag) {
  tags[newTag.macAddress] = newTag;
}

TagInfo getTag(std::string macAddress) {
  return tags[macAddress];
}

void getAllTags(std::vector<TagInfo> *result) {
  for (std::map<std::string, TagInfo>::iterator it = tags.begin(); it != tags.end(); ++it) {
    result->push_back(it->second);
  }
}