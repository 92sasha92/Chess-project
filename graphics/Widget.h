#ifndef WIDGET_H_
#define WIDGET_H_


#include <SDL.h>
#include <SDL_video.h>


typedef struct widget_t Widget;


struct widget_t {
	void (*drawWidget)(Widget*);
	void (*handleEvent)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void* data;
};


void destroyWidget(Widget* src);


#endif
