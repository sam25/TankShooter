#ifndef __BASE_ENGINE_H__
#define __BASE_ENGINE_H__

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
#define MAX_LEN 2048
#define MAX_MESSAGES 30

class base_engine
{
 public:

  base_engine();
  virtual ~base_engine();
  

  void set_ui(user_interface * ui);
  void set_model(model * m);

  virtual void decision_loop()=0;
  
  virtual void give_command(char * command);

  int get_local_player() { return _local_player; }

 protected:

  virtual void send_to_model(char * command);

  user_interface * _ui;
  model * _model;
  int _local_player;
  bool _done;
  int _num_players;
  
  ostringstream _outgoing_command;
};

#endif
