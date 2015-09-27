#include "tank.h"
#include "bullet.h"
#include <SDL_rotozoom.h>

//   constructor
tank::tank(resource_manager * resource, int id) 
  : sprite()
{
  _id = id;
  single_image_resource * sir = 
    (single_image_resource*)resource->get_resource( (char*) "TANK_BASE");
  
  image_display * display 
    = new rotated_image_display( sir->get_img() );
  assert(display);

  // base
  set_display( display );

  // turret
  ostringstream r_id;
  
  r_id<<get_string(id)<<"_TURRET";
  sir = (single_image_resource*) 
    resource->get_resource( (char*)  r_id.str().c_str() );
  assert(sir);

  _turret_display = new rotated_image_display( sir->get_img() );
  assert(_turret_display);
  
  // animations...
  r_id.str("");

  r_id<<get_string(id)<<"_RESPAWN";
  
  animation_frames_resource * arf 
    = (animation_frames_resource*) 
    resource->get_resource( (char*) r_id.str().c_str() );
  assert(arf);

  _respawn = new animated_display( arf->get_imgs(), arf->get_num_imgs() );
  assert(_respawn);

  r_id.str("DEATH");

  arf = (animation_frames_resource*)
    resource->get_resource( (char*) r_id.str().c_str() );
  assert(arf);

  _death = new animated_display( arf->get_imgs(), arf->get_num_imgs() );
  assert(_death);

  _active_animation = 0;

  set_angle(270);
  set_turret_angle(270);

  _alive = true;
  _kills = _deaths = 0;
  _speed = 0;
  _rotate_speed = 0;
}

char * tank::get_string(int id)
{
  if ( id == BLUE )
    return (char*) "BLUE";
  if ( id == BLACK )
    return (char*) "BLACK";
  if ( id == RED )
    return (char*) "RED";
  if ( id == PINK )
    return (char*) "PINK";
  if ( id == GREEN )
    return (char*) "GREEN";

  return 0;
}

//   destructor
tank::~tank()
{
  delete _img;
  delete _turret_display;
  delete _respawn;
  delete _death;
}

//   set/get angle
int tank::get_angle()
{
  return _angle;
}

void tank::set_angle(int a)
{
  _angle = a;

  ((rotated_image_display*) _img )->set_angle(a + IMAGE_OFFSET);
}

void tank::set_turret_angle(int a)
{
   _turret_angle = a;

   _turret_display->set_angle(a + IMAGE_OFFSET);
}

//   fire
void tank::fire(bullet *b, int mousex, int mousey)
{
  double c, delx, dely;
  int bdx, bdy;

  if ( ! _alive || is_respawning() )
    return;

  b->set_alive(true);

  b->set_xy( get_centerx(),  get_centery(), true );

  delx = mousex - get_centerx();
  dely = mousey - get_centery();

  c = sqrt( delx*delx + dely*dely);

  bdx = (int) ( BULLET_SPEED * delx / c );
  bdy = (int) ( BULLET_SPEED * dely / c);

  b->set_dxdy(bdx, bdy);

  b->set_source(this);
}

//   rotate
void tank::rotate(int degrees)
{
  if ( ! _alive )
    return;

  _angle += degrees;

  while ( _angle < 0 )
    _angle += 360;

  while ( _angle > 360 )
    _angle -= 360;

  set_angle(_angle);

  drive( _speed );
}

//   drive
void tank::drive(int howfast)
{
  if ( ! _alive )
    return;

  _speed = howfast;

  double rad = (double) _angle * 3.14 / (double) 180;
  _dx = (int) ( cos(rad) * _speed);
  _dy = (int) ( sin(rad) * _speed);
}

void tank::set_alive(bool b)
{
  if ( b )
  {
    _active_animation = _respawn;
    _active_animation->start(LOOPED_SPAWN);
  }
  else
  {
    _deaths ++;
    drive(0);
    set_angle(270);
    _active_animation = _death;
    _active_animation->start(LOOPED_DEATH);
  }

  sprite::set_alive(b);
}
  
void tank::inc_kills()
{
  _kills++;
}

void tank::report()
{
  sprite::report();
  cerr<<" speed: "<<_speed<<endl;
}

int tank::get_speed()
{
  return _speed;
}


void tank::draw(SDL_Surface * screen)
{
  if ( _active_animation )
  {
    _active_animation->draw(screen, get_centerx(), get_centery(),1 );

    if ( _active_animation->done() )
    {
      if ( _active_animation == _respawn )
	sprite::set_alive(true);

      _active_animation = 0;
    }

    return;
  }
  
  sprite::draw(screen);
  _turret_display->draw(screen, get_centerx(), get_centery(), 1);
}


bool tank::is_tank()
{
  return true;
}

char* tank::get_score()
{
  _score.str("");

  _score<<"Kills: "<<_kills
	<<" Deaths: "<<_deaths;

  return (char*) _score.str().c_str();
}


bool tank::is_driving()
{
  return _speed != 0;
}
  
bool tank::is_rotating()
{
  return _rotate_speed != 0;
}
 
void tank::start_rotate(int a)
{
  _rotate_speed = a;
}
 
bool tank::ready_to_respawn()
{
  return ! _alive && _active_animation == 0;
}
 
bool tank::move(board * b)
{
  if ( _rotate_speed )
    rotate(_rotate_speed);

  return sprite::move(b);
}

bool tank::is_respawning()
{
  return _active_animation == _respawn;
}

int tank::get_id()
{
  return _id;
}
