#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *screen;

#include "text.h"
#include "image.h"
#include "font.h"

Text::Text()
{
	init("", screen, "Neuropol.ttf", 10, 0, 0);
}

Text::Text(const std::string& Caption, const int Size,
				  SDL_Surface* parent_surf)
{
	init(Caption, parent_surf, "Neuropol.ttf", Size, 0, 0);
}

Text::Text(const std::string& Caption, const int x, const int y,
				  SDL_Surface* parent_surf)
{
	init(Caption, parent_surf, "Neuropol.ttf", 10, x, y);
}

Text::Text(const std::string& Caption, const int Size, const int x,
				  const int y, SDL_Surface* parent_surf)
{
	init (Caption, parent_surf, "Neuropol.ttf", Size, x, y);
}

Text::Text(const std::string& Caption, const std::string& Font, 
		const int Size, const int x, const int y, SDL_Surface* parent_surf)
{
	init (Caption, parent_surf, Font, Size, x, y);
}


// Copy constructor
// FIXME: Write this in a better way =)
Text::Text(const Text& rhs)
{
	*this = rhs;
}


// Assignment operator
Text& Text::operator=(const Text& rhs)
{
	if (this == &rhs)
		return *this;

	_Caption    = rhs._Caption;
	_FontFile   = rhs._FontFile;
	_Font       = rhs._Font;
	_Size       = rhs._Size;
	_Style      = rhs._Style;
	_Surface    = rhs._Surface;
	_parent_surf = rhs._parent_surf;
	_Rect       = rhs._Rect;
	_TextColor  = rhs._TextColor;
	_UseBg      = rhs._UseBg;
	_BgColor    = rhs._BgColor;
	return *this;
};


Text::~Text ()
{
}

void Text::Init (const std::string& Caption, SDL_Surface* parent_surf,
				  const std::string& Font, const int Size, const int x,
				  const int y)
{
	_UseBg = false;
	_parent_surf = parent_surf;

	_FontFile = Font;
	Resize (Size);
	ChCaption (Caption);
	ChColor (0xff, 0xff, 0xff);
	Move (x, y);
	ResetStyle ();
}

bool Text::Resize (const int Size)
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

bool Text::ChCaption (const std::string& Caption)
{
	_Caption = Caption;
	return true;
}

bool Text::Move (const int x, const int y)
{
	_Rect.x = x;
	_Rect.y = y;
	return true;
}

bool Text::ChColor (const Uint8 r, const Uint8 g, const Uint8 b)
{
	_TextColor.r = r;
	_TextColor.g = g;
	_TextColor.b = b;
	return true;
}

bool Text::ChBgColor (const Uint8 r, const Uint8 g, const Uint8 b)
{
	_BgColor = SDL_MapRGB (_parent_surf->format, r, g, b);
	_UseBg = true;
	return true;
}

bool Text::ChBgColor ()
{
	_UseBg = false;
	return true;
}

bool Text::Chparent_surf (SDL_Surface *parent_surf)
{
	_parent_surf = parent_surf;
	return true;
}


bool Text::Bold ()
{
	_Style ^= TTF_STYLE_BOLD;
	return ( _Style & TTF_STYLE_BOLD ) ? true : false;
}

bool Text::Underline ()
{
	_Style ^= TTF_STYLE_UNDERLINE;
	return ( _Style & TTF_STYLE_UNDERLINE ) ? true : false;
}

bool Text::Italic ()
{
	_Style ^= TTF_STYLE_ITALIC;
	return ( _Style & TTF_STYLE_ITALIC ) ? true : false;
}

int Text::GetStyle ()
{
	return _Style;
}

int Text::SetStyle (int Style)
{
	_Style = Style;
	return _Style;
}

int Text::ResetStyle ()
{
	_Style = TTF_STYLE_NORMAL;
	return _Style;
}


bool Text::Draw ()
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
	
	if ( _UseBg ) SDL_FillRect (_parent_surf, &_Rect, _BgColor);
	SDL_BlitSurface(_Surface.get(), NULL, _parent_surf, &_Rect);
	return true;
}


SDL_Rect Text::GetSize ()
{
	SDL_Rect Size;
	int w, h;
	
	TTF_SetFontStyle (_Font.get(), _Style);
	TTF_SizeText (_Font.get(), _Caption.c_str(), &w, &h);
	Size.w = w;
	Size.h = h;
	return Size;
}
