#include "world.h"
#include <iostream>

world::world(unsigned int w, unsigned int h):
	width(w),
	height(h)
{
	init();
}

world::~world()
{
	SDL_DestroyRenderer(ren); 
	SDL_DestroyTexture(tex); 
	delete texbuf;
}

void world::init()
{
	atexit(SDL_Quit); 
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow 
   ( 
      "SDL&AGG", 
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      width, 
      height, 
      0 
   );
   
   ren = SDL_CreateRenderer 
   ( 
      win, 
      -1, 
      0 
   ); 
   
   tex = SDL_CreateTexture 
   ( 
      ren, 
      SDL_PIXELFORMAT_RGBA8888, 
      SDL_TEXTUREACCESS_STATIC, 
      width, 
      height 
   );
   
   texbuf = new buffer(width, height);
}

buffer::buffer(unsigned int w, unsigned int h):
	width(w),
	height(h)
{
	pixels = new uint8_t
	[
		width*
		height*
		4 //bytes per pixel
	];
	
	pitch = width*4;
}
	
buffer::~buffer()
{
	delete [] pixels;
}

