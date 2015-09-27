#ifndef __CLIENT_ENGINE_H__
#define __CLIENT_ENGINE_H__

#include "base_engine.h"
#include "message_handler.h"
#include<SDL_net.h>

class client_engine : public base_engine
{
 public:

  client_engine(char * ip, char * port);
  ~client_engine();

  void decision_loop();

 protected:

  void join(char * ip, char * port);

  void send_to_model(char * command);

  SDLNet_SocketSet _host_set;
  TCPsocket _host;
  message_handler * _to_host;
};

#endif
