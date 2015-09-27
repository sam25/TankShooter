#include "animated_display.h"

animated_display::animated_display(SDL_Surface ** imgs, int howmany)
  : image_display(0)
{
  //  cout<<"imgs: "<<imgs<<endl;
  //int i;

  //  for ( i = 0; i < howmany; i ++ )
  //    cout<<"imgs["<<i<<"] is: "<<imgs[i]<<endl;

  _imgs = imgs;
  _num_images = howmany;
  _which_image = 0;
  _animate = false;
  _which_loop = 0;
  _max_loops = 0;
  
}

void animated_display::draw(SDL_Surface * screen, int x, int y, int center)
{
  if ( ! _animate )
    return;

  _img = _imgs[_which_image++];

  image_display::draw(screen,x,y,center);

  if ( _which_image == _num_images )
  {
    _which_loop ++;
    if ( _which_loop == _max_loops )
      _animate = false;
    else
      _which_image = 0;
  }
}

void animated_display::start(int num_loops)
{
  _max_loops = num_loops;
  _which_loop = 0;
  _which_image = 0;
  _animate = true;
}

bool animated_display::done()
{
  return !_animate;
} 
