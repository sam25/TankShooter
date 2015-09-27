
#include "message_handler.h"

message_handler::message_handler(TCPsocket sock, int max_messages_size, int inter)
{
  _socket = sock;
  _max_messages = max_messages_size;
  _transmission_interval = inter;
  _done = false;
  _num_messages = 0;
  
  _message_mutex = SDL_CreateMutex();
  assert(_message_mutex);

  _socket_mutex = SDL_CreateMutex();
  assert(_socket_mutex);

  _messages = new char*[_max_messages];
  assert(_messages);

  int i;

  for ( i = 0; i < _max_messages; i ++ )
  {
    _messages[i] = new char[MAX_MESSAGE];
    assert(_messages[i]);
  }

  _transmission_thread = SDL_CreateThread( transmission_thread,
					   (void*) this);
  assert(_transmission_thread);
}


message_handler::~message_handler()
{
  int ret, i;

  _done = true;

  SDL_DestroyMutex(_message_mutex);
  SDL_DestroyMutex(_socket_mutex);
  SDL_WaitThread(_transmission_thread, &ret);
  SDLNet_TCP_Close(_socket);

  for ( i = 0; i < _max_messages; i ++ )
    delete [] _messages[i];

  delete [] _messages[i];
}

void message_handler::add_message(char * message)
{
  bool full;
  SDL_mutexP(_message_mutex);
  
  strcpy(_messages[_num_messages++], message);
  
  full = _num_messages == _max_messages;

  SDL_mutexV(_message_mutex);
  
  if ( full )
    make_transmission();  
}

void message_handler::make_transmission()
{
  int len, i, sent, num;

  
  // check if have messages to send
  SDL_mutexP(_message_mutex);
  num = _num_messages;
  SDL_mutexV(_message_mutex);

  if ( num == 0 )
    return;
  
  // lock the messages and copy to ostringstream
  SDL_mutexP(_message_mutex);

  ostringstream trans;
  trans.str("");
  
  for ( i = 0; i < _num_messages; i ++ )
    trans<<_messages[i]<<" ";
  
  _num_messages = 0;
    
  SDL_mutexV(_message_mutex);
  // now send it on its way....
  
  len = strlen( trans.str().c_str() ) + 1;

  lock_socket();
  sent = SDLNet_TCP_Send(_socket, trans.str().c_str(), len);
  unlock_socket();

  if ( sent < len )
  {
    cerr<<"Could not send: "<<trans<<" to host"<<endl;
    cerr<<SDLNet_GetError()<<endl;
  }
  
}

int transmission_thread(void * data)
{
  message_handler * m = (message_handler*) data;

  m->transmission_thread_main();

  return 0;
}

void message_handler::transmission_thread_main()
{
  _done = false;

  while ( ! _done )
  {
    make_transmission();
    SDL_Delay(_transmission_interval);
  }
}
