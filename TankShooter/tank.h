#ifndef __TANK_H__
#define __TANK_H__


#define BLUE 0
#define BLACK 1
#define GREEN 2
#define PINK 3
#define RED 4

#include "resource_manager.h"
#include "animated_display.h"
#include "rotated_image_display.h"
#include "sprite.h"
#include<math.h>

class bullet;//inside the.h file, only refer to a pointer to the forwarded class

#define BULLETSPEED 30
#define IMAGE_OFFSET (90)
#define LOOPED_DEATH 3
#define LOOPED_SPAWN 6

class tank : public sprite
{
public:

  //   constructor
  tank(resource_manager * resources, int id);
  //   destructor
  ~tank();

  //   set/get angle
  int get_angle();
  void set_angle(int a);

  //   fire
  void fire(bullet *b, int mousex, int mousey);

  //   rotate
  void rotate(int degrees);
  void set_turret_angle(int degrees);

  //   drive
  void drive(int howfast);
  bool is_driving();  //new
  bool is_rotating(); //new
  void start_rotate(int a); //new

  // alive and respawn
  void set_alive(bool b);
  bool ready_to_respawn(); //new

  // drawing
  void draw(SDL_Surface * screen);

  // etc...
  void inc_kills();
  bool is_tank();
  char* get_score();
  int get_speed();
  bool is_respawning();
  // debugging
  void report();

  // move
  bool move(board * b);
  int get_id();
 
protected:
//   members:
//   - angle (0,0 is in the middle of the tank.)
//     (degrees)
  char * get_string(int id);

  rotated_image_display * _turret_display;
  animated_display * _death, * _respawn, * _active_animation;
  int _loop;
  int _turret_angle;
  
  int _id;

  int _angle;
  int _kills;
  int _deaths;
  int _speed;
  int _rotate_speed;
  ostringstream _score;
};


#endif
