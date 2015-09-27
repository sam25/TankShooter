#include "resource_manager.h"
#include<assert.h> 

resource_manager::resource_manager(char * filename)
{
  ifstream infile(filename);
  char token[MAX_LEN];
  int i;

  if ( ! infile )
  {
    cerr<<"Could not open resource file: "<<filename<<endl;
    exit(1);
  }

  if ( ! get_file_token(infile, token) ||
       strcmp(token, "NUMBER_RESOURCES") != 0 )
  {
    cerr<<"could not find NUMBER_RESOURCES"<<endl;
    cerr<<"found "<<token<<endl;
    exit(1);
  }
    
  if ( ! get_file_token(infile, token) )
  {
    cerr<<"Cannot find number of resources"<<endl;
    cerr<<"found: "<<token<<endl;
    exit(1);
  }

  _num_resources = atoi(token);

  _resources = new resource*[_num_resources];
  assert(_resources);

  for ( i = 0; i < _num_resources; i ++ )
  {
    if ( ! get_file_token(infile,token) )
    {
      cerr<<"Could not find token"<<endl;
      cerr<<"found: "<<token<<endl;
    }

    if ( strcmp(token, "SINGLE_IMAGE") == 0 )
      _resources[i] = new single_image_resource(infile);
    else if ( strcmp(token, "ANIMATION_FRAMES") == 0 )
      _resources[i] = new animation_frames_resource(infile);
    else if ( strcmp(token, "FONT") == 0 )
      _resources[i] = new font_resource(infile);
    else
      cerr<<"Token not found: "<<token<<endl;

    assert(_resources[i]);
  }

  infile.close();
}

resource_manager::~resource_manager()
{
  int i;

  for ( i = 0; i < _num_resources; i ++ )
    delete _resources[i];

  delete [] _resources;
}


resource * resource_manager::get_resource(char * name)
{
  int i;

  for ( i = 0; i < _num_resources; i ++ )
    if ( _resources[i]->matches(name) )
      return _resources[i];

  // no resources found
  cout<<"resource not found: "<<name<<endl;
  return 0;
}


bool resource_manager::get_file_token(ifstream & infile, char * token)
{
  while ( 1 )
  {
    if ( ! infile.getline(token, MAX_LEN) )
    {
      cerr<<"end of file."<<endl;
      return false;
    }

    if ( isalnum(token[0]) )
      return true;
  }

  return false;
}
