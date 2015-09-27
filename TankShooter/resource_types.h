#ifndef __RESOURCE_TYPES_H__
#define __RESOURCE_TYPES_H__

#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<SDL_ttf.h>

using namespace std;

#define MAX_LEN 2048

class resource
{
 public:

  resource(ifstream & infile);
  virtual ~resource();

  virtual bool matches(char * id);

 protected:

  char * _id;

};

class single_image_resource : public resource
{
public:

  single_image_resource(ifstream & infile);
  ~single_image_resource();

  SDL_Surface * get_img();

protected:

  SDL_Surface * _img;

};

class animation_frames_resource : public resource
{
public:

  animation_frames_resource(ifstream & infile);
  ~animation_frames_resource();

  SDL_Surface ** get_imgs();
  int get_num_imgs();

protected:
  
  SDL_Surface ** _imgs;
  int _num_imgs;

};


class font_resource : public resource
{
public:
  font_resource(ifstream & infile);
  ~font_resource();
  
  TTF_Font * get_font();
  int get_pt_size();
  SDL_Color get_color();

protected:

  TTF_Font * _font;
  SDL_Color _color;
  int _pt_size;
  
  
};


#endif
