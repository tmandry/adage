#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

//using namespace std;

extern SDL_Surface *Screen;

class TextClass
{
	public:
	TextClass ();
	TextClass (const char *Caption, const int Size,
			 SDL_Surface *ParentSurf = Screen);
	TextClass (const char *Caption, const int x, const int y,
			 SDL_Surface *ParentSurf = Screen);
	TextClass (const char *Caption, const int Size, const int x,
			 const int y, SDL_Surface *ParentSurf = Screen);
	TextClass (const char *Caption, const char *Font, const int Size,
			 const int x, const int y, SDL_Surface *ParentSurf = Screen);
	
	// copy constructor
	TextClass (const TextClass &Old)
	{
		_Surface = NULL;
		_Caption = NULL;
		_Font = NULL;
		*this = Old;
	}

	void operator= (const TextClass &Old)
	{
		if ( _Surface != NULL ) SDL_FreeSurface (_Surface);
		if ( _Font != NULL ) TTF_CloseFont (_Font);
		delete [] _Caption;
		memcpy (this, &Old, sizeof (TextClass));
	
		char *Caption = new char [strlen (_Caption) + 1];
		strcpy (Caption, _Caption);
		_Caption = Caption;

		_Font = NULL;
		Resize (_Size);
	}

	
	~TextClass ();

	bool Resize (const int Size);
	bool ChCaption (const char *Caption);
	bool Move (const int x, const int y);
	bool ChColor (const Uint8 r, const Uint8 g, const Uint8 b);
	bool ChBgColor (const Uint8 r, const Uint8 g, const Uint8 b);
	bool ChBgColor ();
	bool ChParentSurf (SDL_Surface *ParentSurf);

	bool Bold ();
	bool Underline ();
	bool Italic ();
	int GetStyle ();
	int SetStyle (int Style);
	int ResetStyle ();
	
	bool Draw ();

	SDL_Rect GetSize ();

	private:
	void init (const char *Caption, SDL_Surface *ParentSurf, const char *Font,
			 const int Size, const int x, const int y);
	
	char *_Caption;
	
	char _FontFile[64];
	TTF_Font *_Font;
	int _Size;
	int _Style;
	
	SDL_Surface *_Surface;
	SDL_Surface *_ParentSurf;
	SDL_Rect _Rect;
	
	SDL_Color _TextColor;
	bool _UseBg;
	Uint32 _BgColor;
};

#endif // TEXT_H
