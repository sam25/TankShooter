#ifndef __REGULAR_ENGINE_H__
#define __REGULAR_ENGINE_H__

#include "base_engine.h"

class regular_engine : public base_engine
{
 public:

  regular_engine();
  ~regular_engine();

  void decision_loop();

 protected:

  void make_decisions();
  void check_for_respawns();
  void check_for_deaths();
  void check_for_dead_bullets();
  virtual void handle_ai_tanks();
  void handle_rotate(int tank);
  void handle_drive(int tank);
  void handle_fire(int tank);
  void init_game();

};

#endif
