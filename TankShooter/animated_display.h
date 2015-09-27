#ifndef __ANIMATED_DISPLAY_H__
#define __ANIMATED_DISPLAY_H__

#include "image_display.h"


class animated_display : image_display
{
 public:

  animated_display(SDL_Surface ** imgs, int howmany);
  void draw(SDL_Surface * screen, int x, int y, int center=0);

  void start(int num_loops);
  bool done();
  

 protected:

  SDL_Surface ** _imgs;
  int _num_images;
  int _which_image;
  bool _animate;

  int _which_loop;
  int _max_loops;
};

#endif
