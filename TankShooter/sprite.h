#ifndef __SPRITE_H__
#define __SPRITE_H__

#include<SDL.h>
#include<SDL_image.h>
#include<stdlib.h>
#include<assert.h>
#include<iostream>
#include"image_display.h"

class board;


using namespace std;

class sprite
{
public:
  //   constructor
  sprite(image_display * img);
  sprite();

  //   destructor
  virtual ~sprite();

  //   draw
  virtual void draw(SDL_Surface * screen);

  //   move
  virtual bool move(board * b);

  //   set_xy,
  virtual void set_xy(int x, int y, bool center=false);

  //   set_dxdy
  virtual void set_dxdy(int dx, int dy);

  //   collide
  virtual bool collide(sprite * my_friend);

  //   set/get alive
  virtual bool get_alive();
  virtual void set_alive(bool alive);

  virtual bool point_in_image(int x, int y);

  virtual int get_width();
  virtual int get_height();
  virtual int get_x();
  virtual int get_y();
  virtual int get_centerx();
  virtual int get_centery();
  
  virtual void report();
  virtual bool is_tank();

  virtual void set_display(image_display *d);
  virtual image_display* get_display();

protected:
  
  bool collide_helper(sprite * my_friend);
  

  image_display * _img;
  SDL_Rect _src, _dest;
  int _dx,_dy;
  bool _alive;
};

#endif
