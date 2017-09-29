#ifndef SPWINDOW_H_
#define SPWINDOW_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

typedef struct spwindow_t SPWindow;
struct spwindow_t {
  void* data;
  void (*drawWindow)(SPWindow* );
  void (*handleEventWindow)(SPWindow* , SDL_Event* );
  void (*destroyWindow)(SPWindow* );
  //You can add more common functions Like exit/hide etc...
};

//NULL safe FUNCTION
void destroyWindow(SPWindow* src);

#endif

