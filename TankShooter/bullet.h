#ifndef __BULLET_H__
#define __BULLET_H__

#include "sprite.h"

// forward declartion
class tank;

#define BULLET_SPEED 30


class bullet : public sprite
{
public:

  //   constructor
  bullet(image_display * img);
  //   destructor
  ~bullet();
  bool collide(sprite * my_friend);

  //   get/set for whose bullet
  void set_source(tank * t);
  tank* get_source();

  bool is_at_edge();
  bool move(board * b);
  void set_alive(bool b);

protected:

  //   - whose bullet
  tank * _source;
  bool _at_edge;

};


#endif
