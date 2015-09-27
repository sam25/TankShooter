#ifndef __SCORE_BOARD_H__
#define __SCORE_BOARD_H__


#include "resource_manager.h"
#include "resource_types.h"
#include "tank.h"

class score_board
{
 public:

  score_board(resource_manager * resources, tank ** tanks,
	      int num_players);
  virtual ~score_board();

  virtual void draw(SDL_Surface * screen);

 protected:

  // returns the bottom of the line drawn
  int draw_line(SDL_Surface * screen, int line, int top);

  tank ** _tanks;
  int _num_players;
  font_resource ** _fonts;

};

#endif
