#include "ImageRegistry.h"

#include <loadpng.h>

#include "../util/tools.h"

std::map<std::string, BITMAP*> ImageRegistry::images;

void ImageRegistry::loadImage(const std::string& key, const std::string& path) {
  BITMAP* image = load_png(path.c_str(), nullptr);

  if (!image) {
    abort_on_error("Cannot find image " + path +
                   "\nPlease check your files and try again");
  }

  ImageRegistry::images[key] = image;
}

BITMAP* ImageRegistry::getImage(const std::string& key) {
  auto image = ImageRegistry::images[key];

  if (!image) {
    abort_on_error(("Cannot find image " + key).c_str());
  }

  return image;
}
