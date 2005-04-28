#ifndef FONT_H
#define FONT_H

#include "SDL_ttf.h"
#include "handle.h"

class Font : public basic_handle<TTF_Font> {
public:
	Font(TTF_Font* prep = 0, delete_function del = TTF_CloseFont):
		basic_handle<TTF_Font>(prep, del)
	{}
          
	~Font(){}
};

#endif // FONT_H
