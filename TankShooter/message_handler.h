#ifndef __MESSAGE_HANDLER_H__
#define __MESSAGE_HANDLER_H__

#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

#define DEF_INTERVAL 50
#define MAX_MESSAGE 2048

class message_handler
{
 public:
  
  message_handler(TCPsocket sock, int max_messages_size, int inter=DEF_INTERVAL);
  virtual ~message_handler();

  void add_message(char * message);
  void lock_socket() { SDL_mutexP(_socket_mutex); }
  void unlock_socket() { SDL_mutexV(_socket_mutex); }
  

  void transmission_thread_main();


 protected:



  void make_transmission();

  SDL_mutex * _message_mutex, * _socket_mutex;
  SDL_Thread * _transmission_thread;
  TCPsocket _socket;
  
  char ** _messages;
  int _num_messages;
  int _max_messages;
  int _transmission_interval;

  bool _done;

  
};

int transmission_thread(void * data);

#endif
