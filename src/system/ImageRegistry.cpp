#include "ImageRegistry.hpp"

#include <loadpng.h>

#include "../util/tools.h"

std::map<std::string, BITMAP*> ImageRegistry::images;

void ImageRegistry::loadImage(const std::string& imageKey,
                              const std::string& path) {
  BITMAP* image = load_png(path.c_str(), nullptr);

  if (!image) {
    abort_on_error("Cannot find image " + path +
                   "\nPlease check your files and try again");
  }

  ImageRegistry::images[imageKey] = image;
}

BITMAP* ImageRegistry::getImage(const std::string& imageKey) {
  auto* image = ImageRegistry::images[imageKey];

  if (!image) {
    abort_on_error("Cannot find image " + imageKey);
  }

  return image;
}
