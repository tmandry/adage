/** \file font.h
 ** \brief Contains the Font class definition
 **/

#ifndef FONT_H
#define FONT_H

#include "SDL_ttf.h"
#include "smartptr.h"

/// A lazy way to use the SmartPtr class for a TTF_Font
class Font : public SmartPtr<TTF_Font> {
public:
	Font(TTF_Font* prep = 0, delete_function del = TTF_CloseFont):
		SmartPtr<TTF_Font>(prep, del)
	{}
          
	~Font(){}
};

#endif // FONT_H
