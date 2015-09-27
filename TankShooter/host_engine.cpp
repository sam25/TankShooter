#include "host_engine.h"

host_engine::host_engine(char * num_clients, char * port)
  : regular_engine()
{
  _num_clients = atoi(num_clients);
  wait_for_clients( atoi(port));
  
}

host_engine::~host_engine()
{
  int i;
  for ( i = 0; i < _num_clients; i ++ )
    delete _to_clients[i];
  
  delete [] _to_clients;

  SDLNet_FreeSocketSet(_client_set);
  SDLNet_FreeSocketSet(_listen_set);
  
  SDLNet_TCP_Close(_listener);

}


// also make something else, can't remember what....override the AI one.

void host_engine::decision_loop()
{
  int r;
  init_game();
  _done = false;
  SDL_Thread * client_thread = SDL_CreateThread( run_clients, (void*)this);
  assert(client_thread);
  while ( !_done )
  {
    make_decisions();
     SDL_Delay(DECISION_DELAY);
   }

  SDL_WaitThread(client_thread, &r);
  
}

int run_clients(void *cl)
{
  host_engine* he = (host_engine*) cl;

  he->check_clients();

  return 0;
}


void host_engine::handle_ai_tanks()
{ 
  // int i;
  // int percent;
  // int action;


  // for ( i = 1 + _num_clients; i < _num_players; i ++ )
  // {
  //   percent = rand() % 100 + 1;
    
  //   if ( percent > PERCENT_ACTIVE )
  //     continue;
    
  //   action = rand() % 3;
    
  //   if ( action == 0 )
  //     handle_rotate(i);
  //   else if ( action == 1 )
  //     handle_drive(i);
  //   else 
  //     handle_fire(i);
  // }

}



void host_engine::check_clients()
{
  int i;
  int rec;
  char message[MAX_LEN];

  while ( ! _done )
  {

    if ( SDLNet_CheckSockets(_client_set,0) <= 0 )
      continue;

    for ( i = 0; i < _num_clients; i ++ )
    {
      _to_clients[i]->lock_socket();
      if ( SDLNet_SocketReady(_clients[i]) )
      {
	rec = SDLNet_TCP_Recv(_clients[i], message, MAX_LEN-1);
	if ( rec > 1 )
	{
	  message[rec] = '\0';
	  send_to_model(message);
	}
      }
      _to_clients[i]->unlock_socket();
    }

  }

}


void host_engine::wait_for_clients(int port)
{
  IPaddress ip;
 
  if ( SDLNet_ResolveHost(&ip, 0, port) )
  {
    cerr<<"could not resolve host: "<<SDLNet_GetError()<<endl;
    exit(1);
  }

  _listener = SDLNet_TCP_Open(&ip);
  if ( ! _listener )
  {
    cerr<<"Could not open listening socket..."<<SDLNet_GetError()<<endl;
    exit(1);
  }

  _listen_set = SDLNet_AllocSocketSet(_num_clients);
  if ( ! _listen_set )
  {
    cerr<<"Could not allocate socket set "<<SDLNet_GetError()<<endl;
    exit(1);
  }

  SDLNet_TCP_AddSocket(_listen_set, _listener);

  _client_set = SDLNet_AllocSocketSet(_num_clients);

  if ( ! _client_set )
  {
    cerr<<"Could not allocate client socket set "<<SDLNet_GetError()<<endl;
    exit(1);
  }
  
  _to_clients = new message_handler*[_num_clients];
  assert(_to_clients);

  int i;
  for ( i = 0; i < _num_clients; i ++ )
  {
    if ( SDLNet_CheckSockets(_listen_set, -1) )
    {
      _clients[i] = SDLNet_TCP_Accept(_listener);
      if ( ! _clients[i] )
      {
	cerr<<"Could not accept client: "<<SDLNet_GetError()<<endl;
	exit(1);
      }
      SDLNet_TCP_AddSocket(_client_set, _clients[i]);

      _to_clients[i] = new message_handler(_clients[i], 
					   MAX_MESSAGES);
      assert(_to_clients[i]);

      ostringstream id;
      id<<"YOUARENUMBER: "<<(i+1)<<endl;

      SDLNet_TCP_Send(_clients[i], id.str().c_str(), 
		      strlen(id.str().c_str())+1);
    }
  }
  


}

void host_engine::send_to_model(char * command)
{
  int i;

  for ( i = 0; i < _num_clients; i ++ )
    _to_clients[i]->add_message(command);

  regular_engine::send_to_model(command);
}
