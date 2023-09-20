#ifndef SRC_SYSTEM_IMAGE_REGISTRY_H_
#define SRC_SYSTEM_IMAGE_REGISTRY_H_

#include <allegro.h>
#include <map>
#include <string>

class ImageRegistry {
 public:
  static void loadImage(const std::string& imageKey, const std::string& path);
  static BITMAP* getImage(const std::string& imageKey);

 private:
  static std::map<std::string, BITMAP*> images;
};

#endif  // SRC_SYSTEM_IMAGE_REGISTRY_H_