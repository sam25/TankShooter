#ifndef __ROTATED_IMAGE_DISPLAY_H__
#define __ROTATED_IMAGE_DISPLAY_H__

#include "image_display.h"
#include <SDL_rotozoom.h>

class rotated_image_display : public image_display
{
 public:

  rotated_image_display(SDL_Surface * img, int angle=0);
  ~rotated_image_display();

  virtual void set_angle(int a);

 protected:

  SDL_Surface * _src_img;
};


#endif
