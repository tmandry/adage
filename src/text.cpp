#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *Screen;

#include "text.h"
#include "image.h"
#include "font.h"

TextClass::TextClass ()
{
	Init ("", Screen, "Neuropol.ttf", 10, 0, 0);
}

TextClass::TextClass (const std::string& Caption, const int Size,
				  SDL_Surface* ParentSurf)
{
	Init (Caption, ParentSurf, "Neuropol.ttf", Size, 0, 0);
}

TextClass::TextClass (const std::string& Caption, const int x, const int y,
				  SDL_Surface* ParentSurf)
{
	Init (Caption, ParentSurf, "Neuropol.ttf", 10, x, y);
}

TextClass::TextClass (const std::string& Caption, const int Size, const int x,
				  const int y, SDL_Surface* ParentSurf)
{
	Init (Caption, ParentSurf, "Neuropol.ttf", Size, x, y);
}

TextClass::TextClass (const std::string& Caption, const std::string& Font, 
		const int Size, const int x, const int y, SDL_Surface* ParentSurf)
{
	Init (Caption, ParentSurf, Font, Size, x, y);
}


// Copy constructor
// FIXME: Write this in a better way =)
TextClass::TextClass(const TextClass& rhs)
{
	*this = rhs;
}


// Assignment operator
TextClass& TextClass::operator=(const TextClass& rhs)
{
	if (this == &rhs)
		return *this;

	_Caption    = rhs._Caption;
	_FontFile   = rhs._FontFile;
	_Font       = rhs._Font;
	_Size       = rhs._Size;
	_Style      = rhs._Style;
	_Surface    = rhs._Surface;
	_ParentSurf = rhs._ParentSurf;
	_Rect       = rhs._Rect;
	_TextColor  = rhs._TextColor;
	_UseBg      = rhs._UseBg;
	_BgColor    = rhs._BgColor;
	return *this;
};


TextClass::~TextClass ()
{
}

void TextClass::Init (const std::string& Caption, SDL_Surface* ParentSurf,
				  const std::string& Font, const int Size, const int x,
				  const int y)
{
	_UseBg = false;
	_ParentSurf = ParentSurf;

	_FontFile = Font;
	Resize (Size);
	ChCaption (Caption);
	ChColor (0xff, 0xff, 0xff);
	Move (x, y);
	ResetStyle ();
}

bool TextClass::Resize (const int Size)
{
	Font TmpFont = TTF_OpenFont (_FontFile.c_str(), Size);
	if ( !TmpFont )
	{
		std::cout << "TTF_OpenFont() error: " << TTF_GetError ();
		std::cout << std::endl;
		return false;
	}

	_Size = Size;
	_Font = TmpFont;

	return true;
}

bool TextClass::ChCaption (const std::string& Caption)
{
	_Caption = Caption;
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
	TTF_SetFontStyle (_Font.get(), _Style);
	
	Image tmp = TTF_RenderText_Blended (_Font.get(), _Caption.c_str(), 
			_TextColor);
	
	if ( !tmp )
	{
		std::cout << "TTF_RenderText_Blended() error: ";
		std::cout << TTF_GetError () << std::endl;
		return false;
	}
	_Surface = tmp;

	_Rect.w = _Surface->w;
	_Rect.h = _Surface->h;
	
	if ( _UseBg ) SDL_FillRect (_ParentSurf, &_Rect, _BgColor);
	SDL_BlitSurface(_Surface.get(), NULL, _ParentSurf, &_Rect);
	return true;
}


SDL_Rect TextClass::GetSize ()
{
	SDL_Rect Size;
	int w, h;
	
	TTF_SetFontStyle (_Font.get(), _Style);
	TTF_SizeText (_Font.get(), _Caption.c_str(), &w, &h);
	Size.w = w;
	Size.h = h;
	return Size;
}
