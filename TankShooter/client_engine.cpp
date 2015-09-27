#include "client_engine.h"
#include "model.h"

client_engine::client_engine(char * ip, char * port)
  : base_engine()
{
  join(ip,port);
}

client_engine::~client_engine()
{
  //  SDLNet_TCP_Close(_host);
  delete _to_host;

  SDLNet_FreeSocketSet(_host_set);
}


// also make something else, can't remember what....override the AI one.

void client_engine::decision_loop()
{
  int rec;
  char message[MAX_LEN];

   _done = false;
   while ( !_done )
     if ( SDLNet_CheckSockets(_host_set,0) > 0 )
     {
       _to_host->lock_socket();
       if ( SDLNet_SocketReady(_host) )
       {
	 rec = SDLNet_TCP_Recv(_host, message, MAX_LEN-1);
	 if ( rec > 1 )
	 {
	   cout<<"client says it got: "<<message<<" rec is:"<<rec<<endl;
	   message[rec] = '\0';
	   _model->give_command(message);
	 }
       }
       _to_host->unlock_socket();
     }
   
}


void client_engine::join(char * ipaddr, char * port)
{
  IPaddress ip;
 
  if ( SDLNet_ResolveHost(&ip, ipaddr, atoi(port) ) )
  {
    cerr<<"could not resolve host: "<<SDLNet_GetError()<<endl;
    exit(1);
  }

  _host = SDLNet_TCP_Open(&ip);
  if ( ! _host )
  {
    cerr<<"Could not connect to host..."<<SDLNet_GetError()<<endl;
    exit(1);
  }

  _host_set = SDLNet_AllocSocketSet(1);
  if ( ! _host_set )
  {
    cerr<<"Could not allocate socket set "<<SDLNet_GetError()<<endl;
    exit(1);
  }

  SDLNet_TCP_AddSocket(_host_set, _host);

  _to_host = new message_handler(_host, MAX_MESSAGES);
  assert(_to_host);

  // get the number of the local player
  int rec;
  char line[MAX_LEN];
  rec = SDLNet_TCP_Recv(_host, line, MAX_LEN-1);

  if ( rec > 0 )
  {
    line[rec] = '\0';
    istringstream id(line);

    id>>line; // should say YOUARENUMBER
    id>>_local_player;
    cout<<"got "<<line<<_local_player<<endl;
  }
}

void client_engine::send_to_model(char * command)
{
  _to_host->add_message(command);
}

