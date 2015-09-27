#include "bullet_list.h"
#include "tank.h"

bullet_list::bullet_list(resource_manager * resources, int size)
{
  int i;
  single_image_resource * sir = 
    (single_image_resource*) resources->get_resource( (char*) "BULLET");
  
  _bullet_display = new image_display( sir->get_img() );
  assert(_bullet_display);

  _ammo = new bullet*[size];
  assert(_ammo);

  for ( i = 0; i < size; i ++ )
  {
    _ammo[i] = new bullet(_bullet_display);
    assert(_ammo[i]);
  }

  _num_bullets = size;
}

bullet_list::~bullet_list()
{
  int i;

  delete _bullet_display;

  for ( i = 0; i < _num_bullets; i ++ )
    delete _ammo[i];

  delete[] _ammo;
}

bullet * bullet_list::find_unused_bullet()
{
  int i;

  for ( i = 0; i < _num_bullets; i ++ )
    if ( _ammo[i]->get_alive() == false )
      return _ammo[i];

  grow(_num_bullets);

  return _ammo[i];
}

void bullet_list::move(board *b)
{
  int i;

  for ( i = 0; i < _num_bullets; i ++ )
    _ammo[i]->move(b);
}

void bullet_list::draw(SDL_Surface * screen)
{
  int i;

  for ( i = 0; i < _num_bullets; i ++ )
    _ammo[i]->draw(screen);
}

int bullet_list::collide(sprite * tank)
{
  int i;

  for ( i = 0; i < _num_bullets; i ++ )
    if ( _ammo[i]->collide(tank) )
      return i;

  return -1;
}

void bullet_list::grow(int increment)
{
  int i;
  bullet ** bigger = new bullet*[_num_bullets+increment];
  assert(bigger);

  for ( i = 0; i < _num_bullets; i++ )
    bigger[i] = _ammo[i];

  delete[] _ammo;

  _ammo = bigger;
  _num_bullets += increment;

  for (; i < _num_bullets; i ++ )
  {
    _ammo[i] = new bullet(_bullet_display);
    assert(_ammo[i]);
  }
  
}


void bullet_list::kill_bullet(int which)
{
  _ammo[which]->set_alive(false);
}


int bullet_list::get_owner(int bullet_id)
{
  return _ammo[bullet_id]->get_source()->get_id();
}

bool bullet_list::is_at_edge(int bullet_id)
{
  return _ammo[bullet_id]->is_at_edge();
}

int bullet_list::get_num_bullets()
{
  return _num_bullets;
}
