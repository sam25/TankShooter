#include"host_engine.h"
#include"client_engine.h"
#include"model.h"
#include"user_interface.h"
#include<assert.h>
#include<stdlib.h>
#include<time.h>
// main
// 1) check the command arguments
// 2) allocate a life object
// 3) call the run method
// 4) if it was successful, call the save method

int run_engine(void *ev);
int run_ui(void *uiv);
base_engine * make_new_engine(int argc, char ** argv);

int main(int argc, char ** argv)
{
  SDL_Thread * engine_thread, * user_interface_thread;
  int r1,r2;


  base_engine * e = make_new_engine(argc, argv);
  assert(e);

  model * m = new model( (char*) "imgs/resources.txt");
  assert(m);

  user_interface * ui = new user_interface(e, e->get_local_player());
  assert(ui);

  e->set_ui(ui);
  e->set_model(m);

  srand(time(0));

  engine_thread = SDL_CreateThread( run_engine, (void*) e );
  assert(engine_thread);

  user_interface_thread = SDL_CreateThread( run_ui, (void*) ui );
  assert( user_interface_thread );
  
  m->draw_sleep_loop();

  SDL_WaitThread(engine_thread, &r1);
  SDL_WaitThread(user_interface_thread, &r2);
 
  delete e;
  delete m;
  delete ui;

  return 0;
}

base_engine * make_new_engine(int argc, char ** argv)
{
  if ( argc == 1 )
    return new regular_engine();
  if ( argc == 2 )
  {
      cerr<<"Usage: "<<argv[0]<<" host num_clients port"<<endl;
      cerr<<"Usage: "<<argv[0]<<" join ip port "<<endl;
      exit(1);
  }
  if ( argc == 4 ) // shooter host num_clients port
  {
    if ( strcmp("host", argv[1]) == 0 )
      return new host_engine(argv[2], argv[3]);
    else if ( strcmp("join", argv[1]) == 0 )
      return new client_engine(argv[2], argv[3]);      
    else
    {
      cerr<<"Usage: "<<argv[0]<<" host num_clients port"<<endl;
      cerr<<"Usage: "<<argv[0]<<" join ip port "<<endl;
      exit(1);
    }
  }
    
  return 0;
}


int run_engine(void *ev)
{
  base_engine * e = (base_engine*) ev;

  e->decision_loop();

  return 0;
}

int run_ui(void *uiv)
{
  user_interface * ui = (user_interface*) uiv;

  ui->event_loop();

  return 0;
}
