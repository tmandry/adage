#include "SDL.h"

extern SDL_Surface *Screen;

#include "input.h"
#include "text.h"
#include "rgbamask.h"
#include "image.h"

InputClass::InputClass ()
{
	Init (0, 0, 0, 0, Screen, 12);
}

InputClass::InputClass (const int x, const int y, const int w,
				    SDL_Surface* ParentSurf, const int TextSize)
{
	Init (x, y, w, TextSize + 2, ParentSurf, TextSize);
}

InputClass::InputClass (const int x, const int y, const int w, const int h,
				    SDL_Surface* ParentSurf, const int TextSize)
{
	Init (x, y, w, h, ParentSurf, TextSize);
}


InputClass::~InputClass ()
{
	delete [] _Content;
}


void InputClass::Init (const int x, const int y, const int w, const int h,
				   SDL_Surface* ParentSurf, const int TextSize)
{
	_ParentSurf = ParentSurf;

	Move (x, y);
	Resize (w, h);
	ResizeText (TextSize);
	
	_Text.Move (1, 1);

	_BackColor = SDL_MapRGB (_Surface->format, 0x00, 0x00, 0x00);
	_FrameColor = SDL_MapRGB (_Surface->format, 0x22, 0x22, 0xbb);
}



bool InputClass::SetText (const std::string& Text)
{
    _Content = Text;
	return _Text.ChCaption (Text);
}

bool InputClass::Move (const int x, const int y)
{
	_Rect.x = x;
	_Rect.y = y;
	
	return true;
}

bool InputClass::Resize (const int w, const int h)
{
	Image tmp  = SDL_CreateRGBSurface (SDL_SWSURFACE, w, h, 32, 
			rmask, gmask, bmask, amask);
	tmp.set_deleter(SDL_FreeSurface);
	
	if ( !tmp )
	{
		std::cout <<  "InputClass::Resize(): SDL_CreateRGBSurface();
		std::cout << "failed: \n" << SDL_GetError () << std::endl;
		return false;
	}
	
	// I suppose this is meant to be here --lstor
	_Surface = tmp;
	
	_Text.ChParentSurf (_Surface.get());
	
	_Rect.w = w;
	_Rect.h = h;

	return true;
}

bool InputClass::ResizeText (const int TextSize)
{
	return _Text.Resize (TextSize);
}

bool InputClass::Draw ()
{
	// Background
	SDL_FillRect (_Surface.get(), 0, _BackColor);

	// Draw frame
	SDL_Rect Line;
	Line.x = 0; Line.y = 0;
	Line.w = _Rect.w; Line.h = 1;

	SDL_FillRect (_Surface.get(), &Line, _FrameColor);
	Line.w = 1; Line.h = _Rect.h;
	SDL_FillRect (_Surface.get(), &Line, _FrameColor);
	Line.x = _Rect.w - 1;
	SDL_FillRect (_Surface.get(), &Line, _FrameColor);
	Line.x = 0; Line.y = _Rect.h - 1;
	Line.w = _Rect.w; Line.h = 1;
	SDL_FillRect (_Surface.get(), &Line, _FrameColor);

	// Text
	_Text.Draw ();

	// Blit
	SDL_BlitSurface (_Surface.get(), 0, _ParentSurf, &_Rect);

	return true;
}


bool SetEventHandler (const Uint8 Event, const void (*Handler) (Uint32))
{
}


