#include"user_interface.h"
#include"base_engine.h"



user_interface::user_interface(base_engine * e, int pnum)
{
  _the_engine = e;

  _forward_down = _rotate_left_down = 
    _rotate_right_down = _back_down = false;

  _player_num = pnum;
}

user_interface::~user_interface()
{

}

void user_interface::event_loop()
{
  while ( 1 )
    if ( SDL_WaitEvent( &_event) )
    {
      if ( _event.type == SDL_QUIT )
	handle_quit();
      else if ( _event.type == SDL_MOUSEBUTTONDOWN )
	handle_mousebuttondown();
      else if ( _event.type == SDL_KEYDOWN )
	handle_keydown();
      else if ( _event.type == SDL_KEYUP )
	handle_keyup();
      else if ( _event.type == SDL_MOUSEMOTION )
	handle_mousemotion();
      else if ( _event.type == SDL_USEREVENT )
	return;
    }
}


void user_interface::handle_quit()
{
  _command.str("");
  _command<<_player_num<<" "<<QUIT;

  _the_engine->give_command( (char*) _command.str().c_str() );
}

void user_interface::handle_mousebuttondown()
{
  _command.str("");
  _command<<_player_num<<" "<<FIRE<<" "<<_event.button.x
	  <<" "<<_event.button.y;

  _the_engine->give_command( (char*) _command.str().c_str() );
}

void user_interface::handle_keydown()
{
  _command.str("");
  _command<<_player_num<<" ";

  if ( _event.key.keysym.sym == SDLK_ESCAPE )
    handle_quit();
  if ( _event.key.keysym.sym == SDLK_a )
  {
    _command<<ROTATE_LEFT;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_d )
  {
    _command<<ROTATE_RIGHT;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_w )
  {
    _command<<DRIVE_FORWARD;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_s )
  {
    _command<<DRIVE_BACK;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  
}

void user_interface::handle_keyup()
{
  _command.str("");
  _command<<_player_num<<" ";

  if ( _event.key.keysym.sym == SDLK_a )
  {
    _command<<STOP_ROTATE;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_d )
  {
    _command<<STOP_ROTATE;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_w )
  {
    _command<<STOP_DRIVE;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  if ( _event.key.keysym.sym == SDLK_s )
  {
    _command<<STOP_DRIVE;
    _the_engine->give_command( (char*) _command.str().c_str() );
  }
  
}

void user_interface::handle_mousemotion()
{
  _command.str("");
  _command<<_player_num<<" "<<ROTATE_TURRET_XY<<" "<<_event.motion.x
	  <<" "<<_event.motion.y;

  _the_engine->give_command( (char*) _command.str().c_str() );
}


void user_interface::kill()
{
  SDL_Event e;
  e.type = SDL_USEREVENT;
  SDL_PushEvent(&e);
}
