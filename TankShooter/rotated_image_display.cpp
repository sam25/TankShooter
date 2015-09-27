
#include "rotated_image_display.h"



rotated_image_display::rotated_image_display(SDL_Surface * img, int angle)
  : image_display(img)
{
  _src_img = img;
  _img = 0;
  set_angle(angle);
}

rotated_image_display::~rotated_image_display()
{
  SDL_FreeSurface(_img);
}

void rotated_image_display::set_angle(int a)
{
  if ( _img )
    SDL_FreeSurface(_img);

  _img = rotozoomSurface(_src_img, -a, 1, 1);
}
