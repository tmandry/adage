#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *Screen;

#include "text.h"
#include "richtext.h"
#include "rgbamask.h"
#include "image.h"

RichTextClass::RichTextClass ()
{
	Init (0, 0, 0, 0, "", Screen);
}

RichTextClass::RichTextClass (const int x, const int y, const int w,
						const int h, SDL_Surface* ParentSurf)
{
	Init (x, y, w, h, "", ParentSurf);
}

RichTextClass::RichTextClass (const std::string& FmtText, 
		SDL_Surface *ParentSurf)
{
	Init (0, 0, FmtText, ParentSurf);
}

RichTextClass::RichTextClass (const int x, const int y, const int w,
	const int h, const std::string& FmtText, SDL_Surface *ParentSurf)
{
	Init (x, y, w, h, FmtText, ParentSurf);
}


RichTextClass::~RichTextClass ()
{
}


void RichTextClass::Init (const int x, const int y, const int w, const int h,
	 const std::string& FmtText, SDL_Surface* ParentSurf)
{
	_Size = 20; 
	_Style = TTF_STYLE_NORMAL;
	_Fr = _Fg = _Fb = 0xff;
	_Br = _Bg = _Bb = 0x00;
	_UseBg = true;

	Move (x, y);
	Resize (w, h);
	ChCaption (FmtText);
	ChParentSurf (ParentSurf);
}

void RichTextClass::Init (const int x, const int y, const std::string& FmtText,
					 SDL_Surface* ParentSurf)
{
	Move (x, y);
	ChCaption (FmtText);
	SizeToText ();
	ChParentSurf (ParentSurf);
}



bool RichTextClass::Move (const int x, const int y)
{
	_Rect.x = x; 
	_Rect.y = y;
	return true;
}

bool RichTextClass::Resize (const int w, const int h)
{
	_Rect.w = w; _Rect.h = h;
	
	Image tmp = SDL_CreateRGBSurface (SDL_SWSURFACE, w, h, 32,
			rmask, gmask, bmask, amask);
	
	if ( !tmp )
	{
		std::cout << "SDL_CreateRGBSurface() error: " << SDL_GetError ();
		std::cout << std::endl;
		return false;
	}

	_Surface = tmp;
	
	return true;
}

bool RichTextClass::ChCaption (const std::string& FmtText)
{
	_FmtText = FmtText;
	_Text.clear ();

	return Parse ();
}

bool RichTextClass::ChParentSurf (SDL_Surface* ParentSurf)
{
	_ParentSurf = ParentSurf;
	return true;
}

bool RichTextClass::SizeToText ()
{
	int w = 0, h = 0;

	std::vector<TextClass>::iterator i;
	for ( i = _Text.begin (); i < _Text.end (); i++ )
	{
		SDL_Rect Size;
		Size = i->GetSize ();

		int right = Size.x + Size.w;
		int bottom = Size.y + Size.h;
		if ( right > w ) w = right;
		if ( bottom > h ) h = bottom;
	}

	return Resize (w, h);
}


bool RichTextClass::Parse ()
{
	std::string Sandbox;
	std::string Atom;
	std::stringstream Convert;

	Sandbox = _FmtText;

	int x = 0, y = 0;
	int RowH = 0;
	int Size = _Size;
	int Style = _Style;
	Uint8 Fr = _Fr, Fg = _Fg, Fb = _Fb;
	Uint8 Br = _Br, Bg = _Bg, Bb = _Bb;
	bool UseBg = _UseBg;

	union
	{
		unsigned int Glob;
		struct
		{
			Uint16 zero;
			Uint8 r;
			Uint8 g;
			Uint8 b;
		};
	} RawToRgb;

	// FIXME: Review this!
	// *******************
	char CharToStr [2] = " ";


	for ( unsigned int i = 0; i < _FmtText.size(); i++ )
	{
		std::cout << _FmtText[i];
		if ( !( _FmtText[i] == '^' || _FmtText[i] == '\n' ) )
		{
			CharToStr[0] = _FmtText[i];
			Atom += CharToStr;
			continue;
		}

		// FIXME: Should there still be -1?
		if ( _FmtText[i] == '^' && i == _FmtText.size() - 1 )
		{
			CharToStr[0] = '^';
			Atom += CharToStr;
			continue;
		}

		if ( !Atom.empty () || _FmtText[i] == '\n' )
		{
			_Text.push_back (TextClass (Atom.c_str (), Size, x, y,
								   _Surface.get()));

			std::vector<TextClass>::iterator PrevAtom;
			PrevAtom = _Text.end () - 1;
			
			PrevAtom->SetStyle (Style);
			
			SDL_Rect AtomSize;
			AtomSize = PrevAtom->GetSize ();
			
			// This is to keep track of the overall height of the row,
			// so when there's a newline we know what to do
			if ( AtomSize.h > RowH ) RowH = AtomSize.h;
			
			if ( _FmtText[i] == '\n' )
			{
				x = 0;
				y += RowH;
				RowH = 0;
			}
			else
				x += AtomSize.w;

			PrevAtom->ChColor (Fr, Fg, Fb);
			if ( UseBg ) PrevAtom->ChBgColor (Br, Bg, Bb);

			Atom.clear ();
		}

		if ( _FmtText[i] == '\n' ) continue;
		
		i++;

		switch ( _FmtText[i] )
		{
		case 'B':
			// Bold
			Style ^= TTF_STYLE_BOLD;
			break;
		case 'U':
			// Underline
			Style ^= TTF_STYLE_UNDERLINE;
			break;
		case 'I':
			// Italic
			Style ^= TTF_STYLE_ITALIC;
			break;
		case 'T':
			// Reset style
			Style = _Style;
			break;
		case 'S':
			// Size
			/*Sandbox[i + 3] = 0;*/
			Convert << Sandbox;
			Convert >> Size;
			i += 2;
			break;
		case 's':
			// Reset size
			Size = _Size;
			break;
		case 'C':
			// Foreground color
			/*Sandbox[i + 7] = 0;*/
			Convert << Sandbox;
			Convert >> std::hex >> RawToRgb.Glob;
			Fr = RawToRgb.r; Fg = RawToRgb.g; Fb = RawToRgb.b;
			i += 6;
			break;
		case 'G':
			// Background color
			/*Sandbox[i + 7] = 0;*/
			Convert << Sandbox;
			Convert >> std::hex >> RawToRgb.Glob;
			Br = RawToRgb.r; Bg = RawToRgb.g; Bb = RawToRgb.b;
			UseBg = true;
			i += 6;
			break;
		case 'c':
			// Reset foreground color
			Fr = _Fr; Fg = _Fg; Fb = _Fb;
			break;
		case 'g':
			// Reset background color
			Br = _Br; Bg = _Bg; Bb = _Bb;
			UseBg = _UseBg;
			break;
		case 'b':
			// Clear background color
			UseBg = false;
			break;
		case 'R':
			// Reset EVERYTHING
			Style = _Style;
			Size = _Size;
			Fr = _Fr; Fg = _Fg; Fb = _Fb;
			Br = _Br; Bg = _Bg; Bb = _Bb;
			UseBg = _UseBg;
			break;
		}
	}

	return true;
}

bool RichTextClass::Draw ()
{
	std::vector<TextClass>::iterator i;
	for ( i = _Text.begin (); i < _Text.end (); i++ )
		i->Draw ();
	
	SDL_BlitSurface(_Surface.get(), 0, _ParentSurf, &_Rect);
	
	return true;
}
