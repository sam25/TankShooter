#include"resource_types.h"

resource::resource(ifstream & infile)
{
  char line[MAX_LEN];

  infile.getline(line, MAX_LEN);

  _id = new char[strlen(line) + 1];
  assert(_id);

  strcpy(_id, line);
  
}

resource::~resource()
{
  delete [] _id;
}

bool resource::matches(char * id)
{
  return strcmp(id, _id) == 0; 
}


single_image_resource::single_image_resource(ifstream & infile)
  : resource(infile)
{
  char line[MAX_LEN];

  infile.getline(line, MAX_LEN);

  _img = IMG_Load(line);

  if ( ! _img )
  {
    cerr<<"Could not open file: "<<line<<endl;
    exit(1);
  }
}

single_image_resource::~single_image_resource()
{
  SDL_FreeSurface(_img);
}

SDL_Surface * single_image_resource::get_img()
{
  return _img;
}



animation_frames_resource::animation_frames_resource(ifstream & infile)
  : resource(infile)
{
  char fileroot[MAX_LEN], ext[MAX_LEN], num_images[MAX_LEN];

  int i;
  ostringstream filename;

  infile.getline(fileroot, MAX_LEN);
  infile.getline(ext, MAX_LEN);
  infile.getline(num_images, MAX_LEN);


  _num_imgs = atoi(num_images);
  
  _imgs = new SDL_Surface*[_num_imgs];
  assert(_imgs);

  for ( i = 0; i < _num_imgs; i ++ )
  {
    filename.str("");
    filename<<fileroot<<i<<ext;

    _imgs[i] = IMG_Load( filename.str().c_str() );
    if ( ! _imgs[i] )
    {
      cerr<<"could not open: "<<filename.str()<<endl;
      exit(1);
    }
      
  }

  
}

animation_frames_resource::~animation_frames_resource()
{
  int i;

  for ( i = 0; i < _num_imgs; i ++ )
    SDL_FreeSurface(_imgs[i]);

  delete [] _imgs;
}

SDL_Surface ** animation_frames_resource::get_imgs()
{
  return _imgs;
}

int animation_frames_resource::get_num_imgs()
{
  return _num_imgs;
}


font_resource::font_resource(ifstream & infile)
  : resource(infile)
{
  char filename[MAX_LEN], pt_size[MAX_LEN];
  char r[MAX_LEN], g[MAX_LEN], b[MAX_LEN];

  infile.getline(filename, MAX_LEN);
  infile.getline(pt_size, MAX_LEN);
  infile.getline(r, MAX_LEN);
  infile.getline(g, MAX_LEN);
  infile.getline(b, MAX_LEN);

  _color.r = atoi(r);
  _color.g = atoi(g);
  _color.b = atoi(b);


  _pt_size = atoi(pt_size);

  _font = TTF_OpenFont(filename, _pt_size);
  if ( ! _font )
  {
    cout<<_font<<endl;
    cerr<<"could not open: "<<filename<<" with font: "<<_pt_size<<endl;
    cerr<<TTF_GetError()<<endl;
    exit(1);
  }
}

font_resource::~font_resource()
{
  TTF_CloseFont(_font);
}
  
TTF_Font * font_resource::get_font()
{
  return _font;
}

int font_resource::get_pt_size()
{
  return _pt_size;
}

SDL_Color font_resource::get_color()
{
  return _color;
}
