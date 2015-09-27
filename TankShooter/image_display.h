#ifndef __IMAGE_DISPLAY_H__
#define __IMAGE_DISPLAY_H__

#include "base_display.h"


class image_display : public base_display
{
 public:

  image_display(SDL_Surface * img);
  void draw(SDL_Surface * screen, int x, int y, int center=0);
  int get_height();
  int get_width();


 protected:

  SDL_Surface * _img;

};

#endif
