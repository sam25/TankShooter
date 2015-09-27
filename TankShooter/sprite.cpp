#include"sprite.h"
#include"board.h"

//   constructor
sprite::sprite(image_display * img)
{
  _alive = false;
  _dx = _dy = 0;

  set_display(img);
}

sprite::sprite()
{
  _alive = false;
  _dx = _dy = 0;

  _img = 0;
}



void sprite::set_display(image_display *img)
{
  _src.x = _src.y = 0;
  _src.h = img->get_height();
  _src.w = img->get_width();

  _dest = _src;

  _img = img;
}

//   destructor
sprite::~sprite()
{
}

//   draw
void sprite::draw(SDL_Surface * screen)
{
  if ( _alive )
    _img->draw(screen, get_centerx(), get_centery(), 1);
}

//   move
bool sprite::move(board * b)
{
  int newx = _dest.x + _dx;
  int newy = _dest.y + _dy;
  bool res;

  if ( ! _alive )
    return false;

  res = b->is_valid_move(newx, newy, this);

  if ( res )
  {
    _dest.x = newx;
    _dest.y = newy;
    return true;
  }
    
  return false;
}

//   set_xy,
void sprite::set_xy(int x, int y, bool center)
{
  _dest.x = x;
  _dest.y = y;

  if ( center )
  {
    _dest.x -= _dest.w / 2;
    _dest.y -= _dest.h / 2;
  }
}

//   set_dxdy
void sprite::set_dxdy(int dx, int dy)
{
  _dx = dx;
  _dy = dy;
}

//   collide
bool sprite::collide_helper(sprite * my_friend)
{
  if ( point_in_image( my_friend->_dest.x, my_friend->_dest.y) ||
       point_in_image( my_friend->_dest.x + my_friend->_dest.w, my_friend->_dest.y) ||
       point_in_image( my_friend->_dest.x + my_friend->_dest.w,  my_friend->_dest.y +  my_friend->_dest.h) ||
       point_in_image( my_friend->_dest.x, my_friend->_dest.y + my_friend->_dest.h ) )
    return true;

  return false;
}

bool sprite::collide(sprite * my_friend )
{
  if ( !get_alive() || ! my_friend->get_alive() )
    return false;

  if ( collide_helper(my_friend) || my_friend->collide_helper(this) )
    return true;

  return false;
}


//   set/get alive
bool sprite::get_alive()
{
  return _alive;
}

void sprite::set_alive(bool alive)
{
  _alive = alive;
}
  

bool sprite::point_in_image(int x, int y)
{
  if ( x < _dest.x || y < _dest.y ||
       x > _dest.x + _dest.w ||
       y > _dest.y + _dest.h )
    return false;

  return true;
}


int sprite::get_width()
{
  return _dest.w;
}

int sprite::get_height()
{
  return _dest.h;
}

int sprite::get_x()
{
  return _dest.x;
}

int sprite::get_y()
{
  return _dest.y;
}

void sprite::report()
{
  cerr<<"x: "<<_dest.x<<" y: "<<_dest.y<<" dx: "<<_dx<<" dy: "<<_dy<<endl;
}


int sprite::get_centerx()
{
  return _dest.x + _dest.w/2;
}

int sprite::get_centery()
{
  return _dest.y + _dest.h/2;
}

bool sprite::is_tank()
{
  return false;
}



image_display* sprite::get_display()
{
  return _img;
}
