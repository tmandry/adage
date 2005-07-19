/** \file image.h
 ** \brief Contains the Image class definition
 **/

#ifndef IMAGE_H
#define IMAGE_H

#include "smartptr.h"
#include "SDL.h"

/// A lazy way to use the SmartPtr class for a SDL_Surface
class Image : public SmartPtr<SDL_Surface> {
public:
	Image(SDL_Surface* prep = 0, delete_function del = SDL_FreeSurface) 
		: SmartPtr<SDL_Surface>(prep, del)
	{}
   
	~Image() {}
};

#endif // IMAGE_H
