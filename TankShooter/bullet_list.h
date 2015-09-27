#ifndef __BULLET_LIST_H__
#define __BULLET_LIST_H__

#include"bullet.h"
#include"resource_manager.h"

class bullet_list
{
 public:

  bullet_list(resource_manager * resources, int size=50);
  ~bullet_list();
  bullet * find_unused_bullet();
  
  void move(board *b);
  void draw(SDL_Surface * screen);


  void kill_bullet(int which);
  int collide(sprite * tank);
  int get_owner(int bullet_id);
  bool is_at_edge(int bullet_id);

  int get_num_bullets();

 protected:

  void grow(int increment);

  bullet ** _ammo;
  int _num_bullets;

  SDL_Surface * _bullet_img;
  image_display * _bullet_display;
};

#endif
