#include <iostream>

using namespace std;

#include "SDL.h"

extern SDL_Surface *Screen;

#include "button.h"
#include "rgbamask.h"

ButtonClass::ButtonClass ()
{
	Init ("", 0, 0, 0, 0, 0, false, Screen);
}

ButtonClass::ButtonClass (const char *Caption, const int x, const int y,
					 SDL_Surface *ParentSurf)
{
	Init (Caption, x, y, 0, 0, 0, true, ParentSurf);
}

ButtonClass::ButtonClass (const char *Caption, const int x, const int y,
					 const int w, const int h, SDL_Surface *ParentSurf)
{
	Init (Caption, x, y, w, h, 0, false, ParentSurf);
}

ButtonClass::ButtonClass (const char *Caption, const int x, const int y,
					 const int TextSize, SDL_Surface *ParentSurf)
{
	Init (Caption, x, y, 0, 0, TextSize, true, ParentSurf);
}


ButtonClass::~ButtonClass ()
{
	SDL_FreeSurface (_Surface);
}


void ButtonClass::Init (const char *Caption, const int x, const int y,
				    const int w, const int h, const int TextSize,
				    const bool resize, SDL_Surface *ParentSurf)
{
	_Surface = NULL;
	_State = BUTTON_STATE_UP;

	_HndClick = NULL;

	_ParentSurf = ParentSurf;
	Move (x, y);
	Resize (w, h);
	if ( TextSize ) ResizeText (TextSize);
	ChCaption (Caption);
	if ( resize ) SizeToText ();
	_Caption.Move (BUTTON_SIDE_WIDTH, BUTTON_SIDE_WIDTH);
}



bool ButtonClass::ChCaption (const char *Caption, const bool resize)
{
	if ( !_Caption.ChCaption (Caption) ) return false;

	if ( resize ) SizeToText ();

	return true;
}

bool ButtonClass::Move (const int x, const int y)
{
	_Rect.x = x;
	_Rect.y = y;

	return true;
}

bool ButtonClass::Resize (const int w, const int h)
{
	_Rect.w = w;
	_Rect.h = h;

	if ( _Surface != NULL ) SDL_FreeSurface (_Surface);
	_Surface = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA, w, h, 32,
							   rmask, gmask, bmask, amask);
	
	if ( _Surface == NULL )
	{
		printf ("ButtonClass::Resize(): CreateRGBSurface() failed: %s\n",
			   SDL_GetError ());
		return false;
	}

	//_Surface = Screen;

	_Caption.ChParentSurf (_Surface);

	return true;
}

bool ButtonClass::ResizeText (int size, bool resize)
{
	if ( !_Caption.Resize (size) ) return false;

	if ( resize ) return SizeToText ();

	return true;
}

bool ButtonClass::SizeToText ()
{
	SDL_Rect Size;
	Size = _Caption.GetSize ();
	return Resize (Size.w + BUTTON_SIDE_WIDTH,
				Size.h + BUTTON_SIDE_WIDTH);
}

bool ButtonClass::Draw ()
{
	Uint8 FaceR, FaceG, FaceB;
	Uint8 Side1R, Side1G, Side1B;
	Uint8 Side2R, Side2G, Side2B;
	Uint32 FaceC, Side1C, Side2C;

	switch ( _State )
	{
		case BUTTON_STATE_UP:
			FaceR = FaceG = 0x55; FaceB = 0xee;
			Side1R = Side1G = 0x33; Side1B = 0xcc;
			Side2R = Side2G = 0x22; Side2B = 0xbb;
			break;
		case BUTTON_STATE_DOWN:
		case BUTTON_STATE_FAKEDOWN:
			FaceR = FaceG = 0x33; FaceB = 0xcc;
			Side1R = Side1G = 0x33; Side1B = 0xcc;
			Side2R = Side2G = 0x22; Side2B = 0xbb;
			break;
		case BUTTON_STATE_HOVER:
			FaceR = FaceG = 0x66; FaceB = 0xff;
			Side1R = Side1G = 0x33; Side1B = 0xcc;
			Side2R = Side2G = 0x22; Side2B = 0xbb;
	}

	FaceC = SDL_MapRGB (_Surface->format, FaceR, FaceG, FaceB);
	Side1C = SDL_MapRGB (_Surface->format, Side1R, Side1G, Side1B);
	Side2C = SDL_MapRGB (_Surface->format, Side2R, Side2G, Side2B);
	RenderButton (FaceC, Side1C, Side2C);
	/*if ( SDL_FillRect (_Surface, NULL, Color) < 0 )
	{
		printf ("ButtonClass::Draw(): SDL_FillRect() error: %s\n",
			   SDL_GetError ());
		return false;
	}*/

	int TextLoc;
	TextLoc = BUTTONDOWN ? 3 : 0;
	_Caption.Move (TextLoc, TextLoc);
	if ( !_Caption.Draw () ) return false;

	if ( SDL_BlitSurface (_Surface, NULL, _ParentSurf, &_Rect) < 0 )
	{
		printf ("ButtonClass::Draw(): SDL_BlitSurface error: %s\n",
			   SDL_GetError ());
		return false;
	}

	return true;
}

void ButtonClass::RenderButton (Uint32 FaceC, const Uint32 Side1C,
						  const Uint32 Side2C)
{
	SDL_Rect Face, Side, Black;

	Face.x = BUTTONDOWN ? BUTTON_SIDE_WIDTH : 0;
	Face.y = BUTTONDOWN ? BUTTON_SIDE_WIDTH : 0;
	Face.w = _Rect.w - BUTTON_SIDE_WIDTH;
	Face.h = _Rect.h - BUTTON_SIDE_WIDTH;
	Side.x = BUTTONDOWN ? 0 : Face.w;
	Side.w = 0; Side.h = 1;
	Black.x = Face.w; Black.y = 0;
	Black.w = BUTTON_SIDE_WIDTH; Black.h = 1;

	// Most of this will be covered
	SDL_FillRect (_Surface, NULL, Side2C);

	// Face
	Uint8 FaceR, FaceG, FaceB;
	SDL_GetRGB (FaceC, _Surface->format, &FaceR, &FaceG, &FaceB);
	
	int i = ( Face.w > Face.h ) ? Face.w : Face.h;
	SDL_Rect Grad;
	SDL_Rect GradClip;
	for ( GradClip.x = GradClip.y = Grad.x = Grad.y = Face.x,
		 Grad.w = Grad.h = i; i >= 0; i -= 1 )
	{
		GradClip.w = ( Grad.w > Face.w ) ? Face.w : Grad.w;
		GradClip.h = ( Grad.h > Face.h ) ? Face.h : Grad.h;
		
		SDL_FillRect (_Surface, &GradClip, FaceC);
		
		StepGrad (&FaceR, &FaceG, &FaceB);
		FaceC = SDL_MapRGB (_Surface->format, FaceR, FaceG, FaceB);
		
		Grad.h = i;
		Grad.w -= 1;
		Grad.x += 1;
	}

	Uint32 BlackC;
	BlackC = SDL_MapRGBA (_Surface->format, 0x00, 0x00, 0x00, 0x00);

	// Other side
	for ( Side.y = 0; Side.y < _Rect.h; ++Side.y )
	{
		if ( Side.y < BUTTON_SIDE_WIDTH )
		{
			++Side.w;
			++Black.x; --Black.w;
			Black.y = Side.y;
			
			SDL_FillRect (_Surface, &Black, BlackC);
		}

		if ( Side.y == Face.h )
		{
			Black.x = 0;
			Black.w = 0;
		}
			
		if ( Side.y > Face.h )
		{
			++Side.x; --Side.w;
			++Black.w;
			Black.y = Side.y;

			SDL_FillRect (_Surface, &Black, BlackC);
		}

		SDL_FillRect (_Surface, &Side, Side1C);
	}
}

void ButtonClass::StepGrad (Uint8 *r, Uint8 *g, Uint8 *b)
{
	if ( ( (*r) > 0 ) && ( (*g) > 0 ) && ( (*b) > 0 ) )
	{
		(*r)--;
		(*g)--;
		(*b)--;
	}
}


void ButtonClass::MouseButtonEvent (const Uint8 Button, const Uint8 State,
							 const Uint16 x, const Uint16 y)
{
	if ( x >= _Rect.x && x <= ( _Rect.x + _Rect.w ) &&
		y >= _Rect.y && y <= ( _Rect.y + _Rect.h ) )
	{
		if ( State == SDL_PRESSED )
			_State = BUTTON_STATE_DOWN;
		else
		{
			if ( _State == BUTTON_STATE_DOWN )
				if ( _HndClick != NULL ) _HndClick ();

			_State = BUTTON_STATE_HOVER;
		}
	}
	else
		if ( State == SDL_RELEASED )
			_State = BUTTON_STATE_UP;
}

void ButtonClass::MouseMotionEvent (const Uint8 State, const Uint16 x,
							 const Uint16 y)
{
	if ( x >= _Rect.x && x <= ( _Rect.x + _Rect.w ) &&
		y >= _Rect.y && y <= ( _Rect.y + _Rect.h ) )
	{
		if ( _State != BUTTON_STATE_DOWN )
		{
			if ( State & SDL_BUTTON (SDL_BUTTON_LEFT) )
				_State = BUTTON_STATE_FAKEDOWN;
		else
				_State = BUTTON_STATE_HOVER;
		}
	}
	else
		if ( !( _State == BUTTON_STATE_DOWN &&
			State & SDL_BUTTON (SDL_BUTTON_LEFT) ) )
			_State = BUTTON_STATE_UP;
}


bool ButtonClass::SetEventHandler (const Uint8 Event, void (*Handler) ())
{
	switch ( Event )
	{
	case BUTTON_EVENT_CLICK:
		_HndClick = Handler;
		break;
	default:
		return false;
	}

	return true;
}
