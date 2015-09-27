#include "image_display.h"


image_display::image_display(SDL_Surface * img)
{
  _img = img;
}

void image_display::draw(SDL_Surface * screen, int x, int y, int center)
{
  SDL_Rect src, dest;

  src.x = src.y = 0;
  src.h = _img->h;
  src.w = _img->w;

  dest = src;

  dest.x = x;
  dest.y = y;
  
  if ( center )
  {
    dest.x -= dest.w/2;
    dest.y -= dest.h/2;
  }

  SDL_BlitSurface(_img, &src, screen, &dest);
}


int image_display::get_height()
{
  return _img->h;
}

int image_display::get_width()
{
  return _img->w;
}
