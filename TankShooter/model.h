#ifndef __MODEL_H__
#define __MODEL_H__

#include "resource_manager.h"
#include "bullet_list.h"
#include "score_board.h"
#include "tank.h"
#include "board.h"
#include <SDL_net.h>

#include "command.h"

#define FRAME_RATE 100
#define ROTATE_SPEED 10
#define DRIVE_SPEED 10

#define MAX_PLAYERS 5

class model
{
 public:
  
  model( char * resource_filen );
  ~model();

  void give_command(char * command);
  void draw_sleep_loop();
  void kill();

  bool is_tank_ready_to_respawn(int tank);
  void get_respawn_coords(int *x, int * y, int tank);
  bool is_tank_dead(int tank, int * which_player, int * which_bullet);
  bool is_bullet_at_edge(int bullet);
  bool is_tank_rotating(int tank);
  bool is_tank_driving(int tank);
  int find_target_for(int tank);
  
  int get_num_bullets();
  
 protected:

  void sdl_init(char* resource_file);
  void draw();
  void move();

  void fire(int player, istringstream & com);
  void rotate_left(int player, istringstream & com);
  void rotate_right(int player, istringstream & com);
  void drive_forward(int player, istringstream & com);
  void drive_back(int player, istringstream & com);
  void stop_rotate(int player, istringstream & com);
  void stop_drive(int player, istringstream & com);
  void rotate_turret_xy(int player, istringstream & com);
  void fire_at_tank(int player, istringstream & com);
  void killed_by(int player, istringstream & com);
  void bullet_dead(int player, istringstream & com);
  void respawn(int player, istringstream & com);


  // things to draw
  bullet_list * _bullets;
  tank ** _tanks;
  score_board * _score_board;
  board * _board;

  // misc 
  bool _done;
  int _num_players;

  // basic resources
  resource_manager * _resources;

  SDL_Surface * _screen, * _back;
  SDL_Rect _back_src, _back_dest;

  // mutexs
  SDL_mutex * _model_mutex;
};

#endif
