#ifndef __BASE_DISPLAY_H__
#define __BASE_DISPLAY_H__

#include<SDL.h>
#include<SDL_image.h>
#include<stdlib.h>
#include<assert.h>
#include<iostream>


using namespace std;


class base_display
{
 public:

  virtual ~base_display() {}
  virtual void draw(SDL_Surface * screen, int x, int y, int center=0)=0;
  

};

#endif
