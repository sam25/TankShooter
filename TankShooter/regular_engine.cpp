#include "regular_engine.h"
#include "user_interface.h"
#include "model.h"


regular_engine::regular_engine()
  : base_engine()
{
  _local_player = 0;
}

regular_engine::~regular_engine()
{
}

void regular_engine::init_game()
{
  int i, x, y;

  for ( i = 0; i < _num_players; i ++ )
  {
    _model->get_respawn_coords( &x, &y, i );
    _outgoing_command.str("");
    _outgoing_command<<i<<" "<<RESPAWN<<" "<<x<<" "<<y;
    send_to_model( (char*) _outgoing_command.str().c_str() );
  }

}

void regular_engine::decision_loop()
{
  init_game();

  _done = false;
  while ( !_done )
  {
    make_decisions();
    SDL_Delay(DECISION_DELAY);
  }
  
}

void regular_engine::make_decisions()
{
  check_for_respawns();

  check_for_deaths();

  check_for_dead_bullets();

  handle_ai_tanks();

  _outgoing_command.str( (char*) MOVE_ALL );
  send_to_model( (char*) _outgoing_command.str().c_str() );
}

void regular_engine::check_for_respawns()
{
  int i;
  int x,y;

  for ( i = 0; i < _num_players; i ++ )
    if ( _model->is_tank_ready_to_respawn(i) )
    {
      _model->get_respawn_coords( &x, &y, i );
      _outgoing_command.str("");
      _outgoing_command<<i<<" "<<RESPAWN<<" "<<x<<" "<<y;
      send_to_model( (char*) _outgoing_command.str().c_str() );
    }
      
}

void regular_engine::check_for_deaths()
{
  int i, which_bullet, which_player;

  for ( i = 0; i < _num_players; i ++ )
    if ( _model->is_tank_dead(i, &which_player, &which_bullet) )
    {
      _outgoing_command.str("");
      _outgoing_command<<i<<" "<<KILLED_BY<<" "<<which_player<<" "
		       <<which_bullet;
      send_to_model( (char*) _outgoing_command.str().c_str() );
    }
      
}

void regular_engine::check_for_dead_bullets()
{
  int i;
  for ( i = 0; i < _model->get_num_bullets(); i ++ )
    if ( _model->is_bullet_at_edge(i) )
    {
      _outgoing_command.str("");
      _outgoing_command<<i<<" "<<BULLET_DEAD;
      send_to_model( (char*) _outgoing_command.str().c_str() );
    }
}

void regular_engine::handle_ai_tanks()
{ 
  int i;
  int percent;
  int action;


  for ( i = 0; i < _num_players; i ++ )
    if ( i != _local_player )
    {
      percent = rand() % 100 + 1;

      if ( percent > PERCENT_ACTIVE )
	continue;

      action = rand() % 3;

      if ( action == 0 )
	handle_rotate(i);
      else if ( action == 1 )
	handle_drive(i);
      else 
	handle_fire(i);
    }
}


void regular_engine::handle_rotate(int tank)
{
  int coin;
  _outgoing_command.str("");
  _outgoing_command<<tank<<" ";

  if ( _model->is_tank_rotating(tank) )
    _outgoing_command<<STOP_ROTATE;
  else
  {
    coin = rand() % 2;

    if ( coin == 0 )
      _outgoing_command<<ROTATE_LEFT;
    else
      _outgoing_command<<ROTATE_RIGHT;
  }

  send_to_model( (char*) _outgoing_command.str().c_str() );
    
}

void regular_engine::handle_drive(int tank)
{

  int coin;
  _outgoing_command.str("");
  _outgoing_command<<tank<<" ";

  if ( _model->is_tank_driving(tank) )
    _outgoing_command<<STOP_DRIVE;
  else
  {
    coin = rand() % 2;

    if ( coin == 0 )
      _outgoing_command<<DRIVE_FORWARD;
    else
      _outgoing_command<<DRIVE_BACK;
  }

  send_to_model( (char*) _outgoing_command.str().c_str() );
    
  
}

void regular_engine::handle_fire(int tank)
{
  int target;

  target = _model->find_target_for( tank );

  _outgoing_command.str("");
  _outgoing_command<<tank<<" "<<FIRE_AT_TANK<<" "<<target;

  send_to_model( (char*) _outgoing_command.str().c_str() ); 
}
