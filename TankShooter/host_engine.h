#ifndef __HOST_ENGINE_H__
#define __HOST_ENGINE_H__

#include "regular_engine.h"
#include<assert.h>
#include<SDL_net.h>
#include "message_handler.h"


#define MAX_CLIENTS (MAX_PLAYERS-1)

class host_engine : public regular_engine
{
 public:

  host_engine(char * num_clients, char * port);
  ~host_engine();

  void decision_loop();
  void check_clients();

 protected:



  void wait_for_clients(int port);
  void send_to_model(char * command);

  void handle_ai_tanks();

  SDLNet_SocketSet _client_set, _listen_set;
  TCPsocket _clients[MAX_CLIENTS];
  message_handler ** _to_clients;
  int _num_clients;
  TCPsocket _listener;

};


int run_clients(void *cl);

#endif
