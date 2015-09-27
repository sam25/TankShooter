#include "model.h"


model::model( char * resource_file )
{
  int i;

  sdl_init(resource_file);
  
  _num_players = MAX_PLAYERS;

  _tanks = new tank*[MAX_PLAYERS];
  assert(_tanks);

  for ( i = 0; i < _num_players; i ++ )
  {
    _tanks[i] = new tank( _resources, i );
    assert(_tanks[i]);
  }

  _bullets = new bullet_list( _resources ); 
  assert(_bullets);
  
  _board = new board( _screen->w, _screen->h, _bullets, _tanks, _num_players);
  assert(_board);
  
  _score_board = new score_board(_resources, _tanks, _num_players);
  assert(_score_board);
 
  SDL_WM_SetCaption("Shooter", "Shooter");

  _model_mutex = SDL_CreateMutex();
}

model::~model()
{
  int i;

  SDL_DestroyMutex(_model_mutex);

  delete _resources;

  SDL_Quit();

  TTF_Quit();

  SDLNet_Quit();

  for ( i = 0; i < _num_players; i ++ )
    delete _tanks[i];

  delete [] _tanks;

  delete _board;
  
  delete _bullets;

  delete _score_board;
}

void model::sdl_init(char* resource_file)
{
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) ) 
  {
    cerr<<"SDL_Init Failed: "<<SDL_GetError()<<endl;
    exit(1);
  }

  if ( TTF_Init() )
  {
    cerr<<"Could not init TTF library."<<TTF_GetError()<<endl;
    exit(1);
  }

  
  _resources = new resource_manager(resource_file);
  assert(_resources);
  
  single_image_resource * sir = 
    (single_image_resource*)  _resources->get_resource( (char*) "BACKGROUND"); 
  _back = sir->get_img();
  
  _back_src.x = _back_src.y = 0;
  _back_src.w = _back->w;
  _back_src.h = _back->h;
  
  _back_dest = _back_src; // shallow copy 

  _screen = SDL_SetVideoMode(_back->w, _back->h, 0,0);
  if ( ! _screen )
  {
    cerr<<"Could not init video mode: "<<SDL_GetError()<<endl;
    exit(1);
  }

  if ( SDLNet_Init() )
  {
    cerr<<"Could not init sdl: "<<SDLNet_GetError()<<endl;
    exit(1);
  }


  _done = false;
}

void model::kill()
{
  SDL_mutexP(_model_mutex);
  _done = true;
  SDL_mutexV(_model_mutex);
}

void model::draw_sleep_loop()
{
  while ( ! _done )
  {
    draw();
    SDL_Delay(FRAME_RATE);
  }
}

void model::draw()
{
  int i;
  SDL_mutexP(_model_mutex);
  SDL_BlitSurface(_back, &_back_src, _screen, &_back_dest);
  
  for ( i = 0; i < _num_players; i ++ )
    _tanks[i]->draw(_screen);
  
  _bullets->draw(_screen);
  
  _score_board->draw(_screen);
  SDL_mutexV(_model_mutex);
  SDL_UpdateRect(_screen,0,0,0,0);
}

void model::give_command(char * command)
{
  char token[MAX_LEN];
  int player;
  istringstream com(command);

  // one command string could contain many diff commands.  
  while ( com>>token )
  {

    if ( strcmp(token, MOVE_ALL) == 0 )
      move();
    else
    {
      player = atoi(token);
      com>>token;
      
      if ( strcmp(token, FIRE) == 0 )
	fire(player, com);
      else if ( strcmp(token, ROTATE_LEFT) == 0 )
	rotate_left(player, com);
      else if ( strcmp(token, ROTATE_RIGHT) == 0 )
	rotate_right(player, com);
      else if ( strcmp(token, DRIVE_FORWARD) == 0 )
	drive_forward(player, com);
      else if ( strcmp(token, DRIVE_BACK) == 0 )
	drive_back(player, com);
      else if ( strcmp(token, STOP_ROTATE) == 0 )
	stop_rotate(player, com);
      else if ( strcmp(token, STOP_DRIVE) == 0 )
	stop_drive(player, com);
      else if ( strcmp(token, ROTATE_TURRET_XY) == 0 )
	rotate_turret_xy(player, com);
      else if ( strcmp(token, FIRE_AT_TANK) == 0 )
	fire_at_tank(player, com);
      else if ( strcmp(token, KILLED_BY) == 0 )
	killed_by(player, com);
      else if ( strcmp(token, BULLET_DEAD) == 0 )
	bullet_dead(player, com);
      else if ( strcmp(token, RESPAWN) == 0 )
	respawn(player, com);
      else
	cerr<<"token not found: "<<token<<endl;
    }

  }// end while
}

void model::fire(int player, istringstream & com)
{
  int x,y;
  
  com>>x;
  com>>y;

  SDL_mutexP(_model_mutex);
  _tanks[player]->fire( _bullets->find_unused_bullet(), x,y );
  SDL_mutexV(_model_mutex);
}

void model::rotate_left(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->start_rotate(-ROTATE_SPEED);
  SDL_mutexV(_model_mutex);
}

void model::rotate_right(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->start_rotate(ROTATE_SPEED);
  SDL_mutexV(_model_mutex);
}

void model::drive_forward(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->drive(DRIVE_SPEED);
  SDL_mutexV(_model_mutex);
}

void model::drive_back(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->drive(-DRIVE_SPEED);
  SDL_mutexV(_model_mutex);
}

void model::stop_rotate(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->start_rotate(0);
  SDL_mutexV(_model_mutex);
}

void model::stop_drive(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _tanks[player]->drive(0);
  SDL_mutexV(_model_mutex);
}

void model::rotate_turret_xy(int player, istringstream & com)
{
  int x, y;

  com>>x;
  com>>y;

  int dx = x - _tanks[player]->get_centerx();
  int dy = y - _tanks[player]->get_centery();
  double rad = atan((double) dy/(double) dx);
  int angle = (int) (180 * rad / 3.14);

  if ( dx < 0 )
    angle -= 180;

  SDL_mutexP(_model_mutex);
  _tanks[player]->set_turret_angle(angle);
  SDL_mutexV(_model_mutex);
}

void model::fire_at_tank(int player, istringstream & com)
{
  int target;

  com>>target;

  int x = _tanks[target]->get_centerx();
  int y = _tanks[target]->get_centery();
  int dx = x - _tanks[player]->get_centerx();
  int dy = y - _tanks[player]->get_centery();
  double rad = atan((double) dy/(double) dx);
  int angle = (int) (180 * rad / 3.14);
  
  if ( dx < 0 )
    angle -= 180;

  SDL_mutexP(_model_mutex);
  _tanks[player]->set_turret_angle(angle);

  _tanks[player]->fire( _bullets->find_unused_bullet(), x,y );
  SDL_mutexV(_model_mutex);
}

void model::killed_by(int player, istringstream & com)
{
  int killer, bullet;

  com>>killer;
  com>>bullet;

  SDL_mutexP(_model_mutex);
  _tanks[player]->set_alive(false);
  _tanks[killer]->inc_kills();
  _bullets->kill_bullet(bullet);
  SDL_mutexV(_model_mutex);
  
}

void model::bullet_dead(int player, istringstream & com)
{
  SDL_mutexP(_model_mutex);
  _bullets->kill_bullet(player);
  SDL_mutexV(_model_mutex);
}

void model::respawn(int player, istringstream & com)
{
  int x,y;

  com>>x;
  com>>y;

  SDL_mutexP(_model_mutex);
  _tanks[player]->set_alive(true);
  _tanks[player]->set_xy(x,y);
  SDL_mutexV(_model_mutex);
}

void model::move()
{
  int i;
  
  SDL_mutexP(_model_mutex);
  for ( i = 0; i < _num_players; i ++ )
    _tanks[i]->move(_board);
  
  _bullets->move(_board);

  SDL_mutexV(_model_mutex);
}



bool model::is_tank_ready_to_respawn(int tank)
{
  bool b;
  SDL_mutexP(_model_mutex);
  b = _tanks[tank]->ready_to_respawn();
  SDL_mutexV(_model_mutex);

  return b;
}

void model::get_respawn_coords(int *x, int * y, int tank)
{
  SDL_mutexP(_model_mutex);
  _board->find_starting_position(x,y, _tanks[tank]);
  SDL_mutexV(_model_mutex);
}

bool model::is_tank_dead(int tank, int * which_player, int * which_bullet)
{
  int bullet, player;

  SDL_mutexP(_model_mutex);
  bullet = _bullets->collide(_tanks[tank]);
  SDL_mutexV(_model_mutex);

  if ( bullet == -1 )
    return false;

  SDL_mutexP(_model_mutex);
  player = _bullets->get_owner(bullet);
  SDL_mutexV(_model_mutex);

  *which_player = player;
  *which_bullet = bullet;

  return true;
}

bool model::is_bullet_at_edge(int bullet)
{
  bool b;
  SDL_mutexP(_model_mutex);
  b = _bullets->is_at_edge(bullet);
  SDL_mutexV(_model_mutex);

  return b;
}

bool model::is_tank_rotating(int tank)
{
  bool b;

  SDL_mutexP(_model_mutex);
  b = _tanks[tank]->is_rotating();
  SDL_mutexV(_model_mutex);

  return b;
}

bool model::is_tank_driving(int tank)
{
  bool b;

  SDL_mutexP(_model_mutex);
  b = _tanks[tank]->is_driving();
  SDL_mutexV(_model_mutex);

  return b;
}

int model::find_target_for(int tank)
{
  int target = rand()%_num_players;

  while ( target == tank )
    target = rand()%_num_players;
  
  return target;
}


int model::get_num_bullets()
{
  int b;
  SDL_mutexP(_model_mutex);
  b = _bullets->get_num_bullets();
  SDL_mutexV(_model_mutex);

  return b;
}
