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
//#include "agg_vcgen_stroke.h"
#include "agg_conv_stroke.h" 

#include <iostream> 
using namespace std; 

int main(int argc, char * argv[]) 
{ 
    
   //initialize SDL 
   //-------------- 
   atexit(SDL_Quit); 
   SDL_Init(SDL_INIT_VIDEO); 
   SDL_Window* win = SDL_CreateWindow 
   ( 
      "SDL&AGG", 
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      frame_width, 
      frame_height, 
      0 
   ); 
   SDL_Renderer* ren = SDL_CreateRenderer 
   ( 
      win, 
      -1, 
      0 
   ); 
                   
   //initialize AGG with memory allocated manually 
   uint8_t *tex_pixels = new uint8_t[frame_width*frame_height*4]; 
   int tex_pitch = frame_width*4; 
    
   #if SDL_BYTEORDER == SDL_BIG_ENDIAN 
   #define AGG_RGBA32_BYTE_ORDER agg::pixfmt_rgba32 
   #else 
   #define AGG_RGBA32_BYTE_ORDER agg::pixfmt_abgr32 
   #endif                   

   agg::rendering_buffer rbuf 
   ( 
      (unsigned char*)tex_pixels, 
      frame_width, 
      frame_height, 
      tex_pitch 
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
   agg::conv_stroke<agg::path_storage> stroke(path); 
       
   SDL_Texture* tex = SDL_CreateTexture 
   ( 
      ren, 
      SDL_PIXELFORMAT_RGBA8888, 
      SDL_TEXTUREACCESS_STATIC, 
      frame_width, 
      frame_height 
   ); 
    
   rbase.clear(agg::rgba8(75, 75, 75, 255)); 
   rscan.color(agg::rgba8(255, 100, 0, 255)); 

   float brush_width = 1.0; 

   stroke.width(brush_width); 
   stroke.line_cap(agg::round_cap); 
   stroke.line_join(agg::round_join); 

   //main event loop 
   //--------------- 
   SDL_Event e; 
   SDL_Point a, b; 
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
                  SDL_GetMouseState(&b.x, &b.y);
                  path.move_to(a.x, a.y); 
                  path.line_to(b.x, b.y); 
                   
                  a=b; 
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
       
      ras.add_path(stroke); 
      rbase.clear(agg::rgba8(75, 75, 75, 255));
      agg::render_scanlines(ras, scanline, rscan); 
      //path.remove_all(); 
       
      SDL_UpdateTexture 
      ( 
         tex, 
         NULL, 
         tex_pixels, 
         tex_pitch 
      ); 
       
      SDL_RenderCopy 
      ( 
         ren, 
         tex, 
         NULL, 
         NULL 
      ); 

      SDL_RenderPresent(ren); 
      SDL_Delay(1); //speedbump 
   } 
    
   delete [] tex_pixels; 
   SDL_DestroyRenderer(ren); 
   SDL_DestroyTexture(tex); 
    
   return 0; 
} 
