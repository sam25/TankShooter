#include "board.h"
#include "sprite.h"
#include "tank.h"
#include "bullet_list.h"



board::board(int w, int h, bullet_list * b, 
	     tank ** tanks, int num_players)
{
  _w = w;
  _h = h;
  _tanks = tanks;
  _num_players = num_players;
  _bullets = b;
}

/*
bullet_list* board::get_bullet_list()
{
  return _bullets;
}
*/
/*
tank* board::get_random_tank(tank * dontshootme)
{
  tank * target = _players[ rand() % _num_players ]->get_tank();

  while ( target == dontshootme )
    target = _players[ rand() % _num_players ]->get_tank();
  
  return target;
}
*/
board::~board()
{
  
}

bool board::is_valid_move(int newx, int newy, sprite * s)
{
  int i;
  bool okay = true;
  int oldx = s->get_x();
  int oldy = s->get_y();

  if ( newx < 0 || newy < 0 || 
       newx + s->get_width() > _w || 
       newy + s->get_height() > _h )
     return false;

  if ( !s->is_tank() )
    return true;

  s->set_xy(newx,newy);

  for ( i = 0; i < _num_players; i ++ )
    if ( _tanks[i] != (tank*) s )
      if ( _tanks[i]->collide(s) )
       {
	 okay = false;
	 break;
       }

   s->set_xy(oldx,oldy);

   return okay;

}


void board::find_starting_position(int * x, int * y, sprite * s)
{
  int newx, newy;
  bool done = false;
  //  SDL_Rect temp_dest;

  //temp_dest.w = s->get_width();
  //temp_dest.h = s->get_height();

  while ( ! done )
  {
    newx = rand()%(_w - s->get_width());
    newy = rand()%(_h - s->get_height());

    done = is_valid_move(newx,newy, s);
  }

  *x = newx;
  *y = newy;
}
