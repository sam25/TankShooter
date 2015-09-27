#include "bullet.h"
#include "tank.h"

//   constructor
bullet::bullet(image_display * img)
  : sprite(img)
{
  _source = 0;
  _at_edge = false;
}
//   destructor
bullet::~bullet()
{
}

void bullet::set_alive(bool b)
{
  sprite::set_alive(b);

  _at_edge = false;
}

bool bullet::collide(sprite * my_friend)
{
  tank * enemy = (tank*) my_friend;

  if ( sprite::collide(my_friend) )
    if ( _source != my_friend && !enemy->is_respawning() )
      return true;
  
  return false;
}


//   get/set for whose bullet
void bullet::set_source(tank * t)
{
  _source = t;
}

tank* bullet::get_source()
{
  return _source;
}


bool bullet::move(board * b)
{
  if ( ! _alive )
    return false;

  bool moved = sprite::move(b);

  if ( ! moved )
    _at_edge = true;
  
  return moved;
}

bool bullet::is_at_edge()
{
  return _at_edge;
}
