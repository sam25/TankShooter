#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "command.h"
#include <SDL.h>
#include <SDL_thread.h>
#include <iostream>
#include <sstream>

using namespace std;

class user_interface;
class model;

#define DECISION_DELAY 100
#define PERCENT_ACTIVE 15

#define MAX_PLAYERS 5

class engine
{
 public:

  engine(int local_player);
  ~engine();

  void set_ui(user_interface * ui);
  void set_model(model * m);

  void decision_loop();

  void give_command(char * command);

 protected:

  void make_decisions();
  void check_for_respawns();
  void check_for_deaths();
  void check_for_dead_bullets();
  void handle_ai_tanks();
  void handle_rotate(int tank);
  void handle_drive(int tank);
  void handle_fire(int tank);
  void init_game();

  user_interface * _ui;
  model * _model;
  int _local_player;
  bool _done;
  int _num_players;
  
  ostringstream _outgoing_command;
};

#endif
