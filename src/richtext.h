#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <vector>

#include "SDL.h"

#include "text.h"

using std::vector;

class RichTextClass
{
	private:

	SDL_Surface *_Surface;
	SDL_Rect _Rect;
	SDL_Surface *_ParentSurf;

	char *_FmtText;

	int _Size;
	int _Style;
	Uint8 _Fr, _Fg, _Fb;
	Uint8 _Br, _Bg, _Bb;
	bool _UseBg;

	void Init (const int x, const int y, const int w, const int h,
			 const char *FmtText, SDL_Surface *ParentSurf);
	void Init (const int x, const int y, const char *FmtText,
			 SDL_Surface *ParentSurf);
	
	bool Parse ();


	public:
	
	vector <TextClass> _Text;	

	RichTextClass ();
	RichTextClass (const int x, const int y, const int w, const int h,
				SDL_Surface *ParentSurf = Screen);
	RichTextClass (const char *FmtText, SDL_Surface *ParentSurf = Screen);
	RichTextClass (const int x, const int y, const int w, const int h,
				const char *FmtText, SDL_Surface *ParentSurf = Screen);
	
	// copy constructor
	RichTextClass::RichTextClass (const RichTextClass &Old)
	{
		_Surface = NULL;
		_FmtText = NULL;
		*this = Old;
	}

	void operator= (const RichTextClass &Old)
	{
		if ( _Surface != NULL ) SDL_FreeSurface (_Surface);
		delete [] _FmtText;
		memcpy (this, &Old, sizeof (RichTextClass));
		_Text = Old._Text;
	
		char *FmtText = new char [strlen (_FmtText) + 1];
		strcpy (FmtText, _FmtText);
		_FmtText = FmtText;
	}


	~RichTextClass ();


	bool Move (const int x, const int y);
	bool Resize (const int w, const int h);
	bool ChCaption (const char *FmtText);
	bool ChParentSurf (SDL_Surface *ParentSurf);
	bool SizeToText ();

	bool Draw ();
};

#endif // RICHTEXT_H
