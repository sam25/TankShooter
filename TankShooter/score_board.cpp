#include "score_board.h"
 

score_board::score_board(resource_manager * resources, tank ** tanks, 
			 int num_players)
{
  _tanks = tanks;
  _num_players = num_players;

  _fonts = new font_resource*[num_players];
  
  assert(_fonts);

  _fonts[BLUE] = 
    (font_resource*) resources->get_resource( (char*) "BLUE_FONT");
  _fonts[BLACK] = 
    (font_resource*) resources->get_resource( (char*) "BLACK_FONT");
  _fonts[RED] = 
    (font_resource*) resources->get_resource( (char*) "RED_FONT");
  _fonts[PINK] = 
    (font_resource*) resources->get_resource( (char*) "PINK_FONT");
  _fonts[GREEN] = 
    (font_resource*) resources->get_resource( (char*) "GREEN_FONT");
}

score_board::~score_board()
{
  delete [] _fonts;
}

void score_board::draw(SDL_Surface * screen)
{
  int top = 0;
  int i;

  for ( i = 0; i < _num_players; i ++ )
    top = draw_line(screen, i, top);
  
}

int score_board::draw_line(SDL_Surface * screen, int i, int top)
{
  SDL_Rect src, dest;
  SDL_Surface * txtimg;

  txtimg = TTF_RenderText_Blended(_fonts[i]->get_font(), 
				  _tanks[i]->get_score(),
				  _fonts[i]->get_color());

  assert(txtimg);

  src.x = src.y = 0;
  src.h = txtimg->h;
  src.w = txtimg->w;

  dest = src;
  dest.y = top;

  SDL_BlitSurface( txtimg, &src, screen, &dest);

  SDL_FreeSurface( txtimg );

  return dest.y + dest.h;
}
