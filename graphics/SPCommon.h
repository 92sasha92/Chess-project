#ifndef SPCOMMON_H_
#define SPCOMMON_H_


#include <SDL.h>
#include <SDL_video.h>

/**
 * creates a copy of a SDL_Rect
 * @param src - a pointer to a SDL_Rect to be copied
 * @return a pointer to the copy the was created
 */
SDL_Rect* spCopyRect(SDL_Rect* src);


#endif
