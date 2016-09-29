#pragma once

#include "agg_renderer_base.h" 

#include "SDL.h"

#include <cstdint>

#if SDL_BYTEORDER != SDL_LIL_ENDIAN 
#define AGG_RGBA32_BYTE_ORDER agg::pixfmt_rgba32 
#else 
#define AGG_RGBA32_BYTE_ORDER agg::pixfmt_abgr32 
#endif   

class buffer
{
	public:
	unsigned int width, height, pitch;
	uint8_t *pixels;
	
	buffer(unsigned int w, unsigned int h);
	~buffer();
};

class world
{
	public:
	unsigned int width, height;
	SDL_Renderer *ren;
	SDL_Window *win;
	SDL_Texture *tex;
	buffer *texbuf;
	world(unsigned int w, unsigned int h);
	~world();
	void init();
	void update();
};


