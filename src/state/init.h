/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#ifndef SRC_STATE_INIT_H_
#define SRC_STATE_INIT_H_

#include <allegro.h>

#include "./state.h"

class init : public state {
 public:
  init();
  ~init(){};

  void update(){};
  void draw() {}

 protected:
 private:
};

#endif  // SRC_STATE_INIT_H_
