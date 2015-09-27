#include "base_engine.h"
#include "user_interface.h"
#include "model.h"


base_engine::base_engine()
{
  _num_players = MAX_PLAYERS;
}

base_engine::~base_engine()
{
}

void base_engine::set_ui(user_interface * ui)
{
  _ui = ui;
}

void base_engine::set_model(model * m)
{
  _model = m;
}

void base_engine::give_command(char * command)
{
  int which_player;
  char action[1024];
  istringstream comm(command);

  comm.str(command);
  
  comm>>which_player;
  comm>>action;

  if ( strcmp(action, QUIT) == 0 )
  {
    _done = true;
    _ui->kill();
    _model->kill();
    return;
  }

  send_to_model(command);
  
}

void base_engine::send_to_model(char * command)
{
  _model->give_command(command);
  cout<<"base_engine->sending to model: "<<command<<endl;
}
