#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<string.h>
#include<stdlib.h>
#include"resource_types.h"
#include<ctype.h>

using namespace std;

class resource_manager
{
 public:

  resource_manager(char * filename);
  ~resource_manager();

  resource * get_resource(char * name);

 protected:

  bool get_file_token(ifstream & infile, char * token);

  int _num_resources;
  resource ** _resources;
};


#endif
