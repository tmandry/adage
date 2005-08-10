/** \file font.h
 ** \brief Contains the Font class definition
 **/

#ifndef FONT_H
#define FONT_H

#include "SDL_ttf.h"
#include "smartptr.h"

/// A lazy way to use the SmartPtr class for a TTF_Font
class Font : public SmartPtr<TTF_Font_Holder> {
public:
	Font(TTF_Font* prep = 0):
		SmartPtr<TTF_Font_Holder>(prep)
	{}
          
	~Font(){}
};

#endif // FONT_H
