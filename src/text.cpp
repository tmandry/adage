#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *Screen;

#include "text.h"

TextClass::TextClass ()
{
	init ("", Screen, "Neuropol.ttf", 10, 0, 0);
}

TextClass::TextClass (const char *Caption, const int Size,
				  SDL_Surface *ParentSurf)
{
	init (Caption, ParentSurf, "Neuropol.ttf", Size, 0, 0);
}

TextClass::TextClass (const char *Caption, const int x, const int y,
				  SDL_Surface *ParentSurf)
{
	init (Caption, ParentSurf, "Neuropol.ttf", 10, x, y);
}

TextClass::TextClass (const char *Caption, const int Size, const int x,
				  const int y, SDL_Surface *ParentSurf)
{
	init (Caption, ParentSurf, "Neuropol.ttf", Size, x, y);
}

TextClass::TextClass (const char *Caption, const char *Font, const int Size,
				  const int x, const int y, SDL_Surface *ParentSurf)
{
	init (Caption, ParentSurf, Font, Size, x, y);
}


TextClass::~TextClass ()
{
	TTF_CloseFont (_Font);
	delete [] _Caption;
}

void TextClass::init (const char *Caption, SDL_Surface *ParentSurf,
				  const char *Font, const int Size, const int x,
				  const int y)
{
	_Font = NULL;
	_Caption = NULL;
	_UseBg = false;
	_Surface = NULL;
	_ParentSurf = ParentSurf;

	strcpy (_FontFile, Font);
	Resize (Size);
	ChCaption (Caption);
	ChColor (0xff, 0xff, 0xff);
	Move (x, y);
	ResetStyle ();
}

bool TextClass::Resize (const int Size)
{
	TTF_Font *Font;
	Font = TTF_OpenFont (_FontFile, Size);
	if ( Font == NULL )
	{
		printf ("TTF_OpenFont() error: %s\n", TTF_GetError ());
		return false;
	}

	_Size = Size;

	if ( _Font != NULL ) TTF_CloseFont (_Font);
	_Font = Font;

	return true;
}

bool TextClass::ChCaption (const char *Caption)
{
	if ( _Caption != NULL ) delete [] _Caption;
	_Caption = new char [strlen (Caption) + 1];
	if ( _Caption == NULL )
	{
		printf ("TextClass::ChCaption() error: Could not allocate memory\n");
		return false;
	}
	strcpy (_Caption, Caption);
	return true;
}

bool TextClass::Move (const int x, const int y)
{
	_Rect.x = x;
	_Rect.y = y;
	return true;
}

bool TextClass::ChColor (const Uint8 r, const Uint8 g, const Uint8 b)
{
	_TextColor.r = r;
	_TextColor.g = g;
	_TextColor.b = b;
	return true;
}

bool TextClass::ChBgColor (const Uint8 r, const Uint8 g, const Uint8 b)
{
	_BgColor = SDL_MapRGB (_ParentSurf->format, r, g, b);
	_UseBg = true;
	return true;
}

bool TextClass::ChBgColor ()
{
	_UseBg = false;
	return true;
}

bool TextClass::ChParentSurf (SDL_Surface *ParentSurf)
{
	_ParentSurf = ParentSurf;
	return true;
}


bool TextClass::Bold ()
{
	_Style ^= TTF_STYLE_BOLD;
	return ( _Style & TTF_STYLE_BOLD ) ? true : false;
}

bool TextClass::Underline ()
{
	_Style ^= TTF_STYLE_UNDERLINE;
	return ( _Style & TTF_STYLE_UNDERLINE ) ? true : false;
}

bool TextClass::Italic ()
{
	_Style ^= TTF_STYLE_ITALIC;
	return ( _Style & TTF_STYLE_ITALIC ) ? true : false;
}

int TextClass::GetStyle ()
{
	return _Style;
}

int TextClass::SetStyle (int Style)
{
	_Style = Style;
	return _Style;
}

int TextClass::ResetStyle ()
{
	_Style = TTF_STYLE_NORMAL;
	return _Style;
}


bool TextClass::Draw ()
{
	TTF_SetFontStyle (_Font, _Style);
	
	if ( _Surface != NULL ) SDL_FreeSurface (_Surface);
	_Surface = TTF_RenderText_Blended (_Font, _Caption, _TextColor);
	if ( _Surface == NULL )
	{
		printf ("TTF_RenderText_Blended() error: %s\n", TTF_GetError ());
		return false;
	}

	_Rect.w = _Surface->w;
	_Rect.h = _Surface->h;
	
	if ( _UseBg ) SDL_FillRect (_ParentSurf, &_Rect, _BgColor);
	SDL_BlitSurface (_Surface, NULL, _ParentSurf, &_Rect);
	return true;
}


SDL_Rect TextClass::GetSize ()
{
	SDL_Rect Size;
	int w, h;
	
	TTF_SetFontStyle (_Font, _Style);
	TTF_SizeText (_Font, _Caption, &w, &h);
	Size.w = w;
	Size.h = h;
	return Size;
}
