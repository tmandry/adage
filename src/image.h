#ifndef IMAGE_H
#define IMAGE_H

#include "handle.h"
#include "SDL.h"

struct Image : basic_handle<SDL_Surface>
{
     Image (SDL_Surface* prep = 0, delete_function del = SDL_FreeSurface) :
            basic_handle<SDL_Surface> (prep, del)
            {}
     
     ~Image () {}
};

#endif // IMAGE_H
