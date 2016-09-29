enum 
{ 
   frame_width = 800, 
   frame_height = 600 
}; 

#include "SDL.h" 
#include "agg_renderer_base.h" 
#include "agg_pixfmt_rgba.h" 
#include "agg_scanline_u.h" 
#include "agg_rasterizer_scanline_aa.h" 
#include "agg_renderer_scanline.h" 
#include "agg_path_storage.h" 
#include "agg_conv_stroke.h" 

#include "agg_conv_bspline.h"
#include "agg_conv_smooth_poly1.h"

#include "world.h"

#include <iostream> 
using namespace std; 

int main(int argc, char * argv[]) 
{ 
  world w(frame_width, frame_height);  

   agg::rendering_buffer rbuf 
   ( 
      (unsigned char*)w.texbuf->pixels, 
      frame_width, 
      frame_height, 
      w.texbuf->pitch 
   ); 
    
   AGG_RGBA32_BYTE_ORDER pixf(rbuf); 
   agg::renderer_base<AGG_RGBA32_BYTE_ORDER> rbase(pixf); 
   agg::scanline_u8 scanline; 
   agg::rasterizer_scanline_aa<> ras; 
   agg::renderer_scanline_aa_solid 
   < 
      agg::renderer_base 
      < 
         AGG_RGBA32_BYTE_ORDER 
      > 
   > rscan(rbase);
   
   agg::path_storage path;
   
	//--smoothed lines
	agg::conv_smooth_poly1
	<
		agg::path_storage
	> smooth(path);
	
	agg::conv_curve
	<
		agg::conv_smooth_poly1
		<
			agg::path_storage
		>
	> curve(smooth);
	
	agg::conv_stroke
	<
		agg::conv_curve
		<
			agg::conv_smooth_poly1
			<
				agg::path_storage
			>
		>
	> stroke(curve);
	
	//--straight lines
	agg::conv_stroke
	<
		agg::path_storage
	> lines(path);
	
   rbase.clear(agg::rgba8(75, 75, 75, 255)); 
   rscan.color(agg::rgba8(255, 100, 0, 255)); 

   float brush_width = 1.0; 
   smooth.smooth_value(0.5);

   stroke.width(brush_width); 
   stroke.line_cap(agg::round_cap); 
   stroke.line_join(agg::round_join); 

   //main event loop 
   //--------------- 
   SDL_Event e; 
   SDL_Point a; 
   while(1) //loop until user quits 
   { 
      while(SDL_PollEvent(&e)) 
      { 
         switch(e.type) 
         { 
            case SDL_QUIT: 
               return 0; 
             
            case SDL_KEYDOWN: 
               if(e.key.keysym.sym == SDLK_ESCAPE) 
                  return(0); 
                   
            case SDL_MOUSEBUTTONDOWN: 
               if (e.button.button == SDL_BUTTON_LEFT) 
               { 
                  SDL_GetMouseState(&a.x, &a.y); 
                  path.move_to(a.x, a.y); 
               } 
               break; 
                
            case SDL_MOUSEMOTION: 
               if(e.motion.state & SDL_BUTTON_LMASK) 
               { 
                  SDL_GetMouseState(&a.x, &a.y);
                  path.line_to(a.x, a.y); 
               } 
               break; 
                
            case SDL_MOUSEWHEEL: 
               brush_width += e.wheel.y/1.0; 
               if(brush_width < 0) 
               brush_width = 0; 
               stroke.width(brush_width);
            break; 
         } 
      } 


	  rbase.clear(agg::rgba8(75, 75, 75, 255));

      ras.add_path(stroke);//conv_curve::conv_smooth
      rscan.color(agg::rgba8(255, 100, 0, 255));
      agg::render_scanlines(ras, scanline, rscan);
      
      //ras.add_path(lines);
      rscan.color(agg::rgba8(0, 255, 0, 255));
      agg::render_scanlines(ras, scanline, rscan);
      
      //path.remove_all(); 
       
      SDL_UpdateTexture 
      ( 
         w.tex, 
         NULL, 
         w.texbuf->pixels, 
         w.texbuf->pitch 
      ); 
       
      SDL_RenderCopy 
      ( 
         w.ren, 
         w.tex, 
         NULL, 
         NULL 
      ); 

      SDL_RenderPresent(w.ren); 
      SDL_Delay(100); //speedbump 
   } 


    
   return 0; 
} 
