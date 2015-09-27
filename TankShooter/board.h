#ifndef __BOARD_H__
#define __BOARD_H__

#include<SDL.h>
#include<SDL_image.h>
#include<assert.h>
#include<stdlib.h>
#include<iostream>


using namespace std;

class sprite;
class tank;
class bullet_list;


class board
{
 public:

  board(int w, int h, bullet_list * b, tank ** tanks, int num_players);
  virtual ~board();
  virtual bool is_valid_move(int newx, int newy, sprite * s);
  virtual void find_starting_position(int * x, int * y, sprite * s);
  //  virtual bullet_list* get_bullet_list();
  //  virtual tank* get_random_tank(tank * dontshootme);

 protected:

  int _w, _h;
  bullet_list * _bullets;
  tank ** _tanks;
  int _num_players;
  
};


#endif
