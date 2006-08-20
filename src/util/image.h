/** \file image.h
 ** \brief Contains the Image class definition
 **/

#ifndef IMAGE_H
#define IMAGE_H

#include "config.h"

#include "boost/shared_ptr.hpp"
#include "SDL.h"

/// A lazy way to use the SmartPtr class for a SDL_Surface
class Image : public boost::shared_ptr<SDL_Surface> {
public:
	typedef void (*delete_function)(SDL_Surface*);
	Image(SDL_Surface* prep = 0, delete_function del = SDL_FreeSurface) 
		: boost::shared_ptr<SDL_Surface>(prep, del)
	{}
   
	~Image() {}
};

#endif // IMAGE_H
