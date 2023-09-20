#ifndef SRC_SYSTEM_IMAGE_REGISTRY_H_
#define SRC_SYSTEM_IMAGE_REGISTRY_H_

#include <asw/asw.h>
#include <map>
#include <string>

class ImageRegistry {
 public:
  static void loadImage(const std::string& imageKey, const std::string& path);
  static asw::Texture getImage(const std::string& imageKey);

 private:
  static std::map<std::string, asw::Texture> images;
};

#endif  // SRC_SYSTEM_IMAGE_REGISTRY_H_