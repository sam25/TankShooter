#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include"command.h"
#include<SDL.h>
#include<iostream>
#include<sstream>

using namespace std;

class base_engine;

class user_interface
{
 public:

  user_interface(base_engine * e, int pnum);
  virtual ~user_interface();

  virtual void event_loop();
  virtual void kill();

protected:

  void handle_quit();
  void handle_mousebuttondown();
  void handle_keydown();
  void handle_keyup();
  void handle_mousemotion();

  base_engine * _the_engine;
  SDL_Event _event;
  bool _forward_down, _rotate_left_down, _rotate_right_down,
    _back_down;
 
  int _player_num;
  ostringstream _command;
};


#endif
