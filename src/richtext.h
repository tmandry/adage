#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <vector>

#include "SDL.h"

#include "text.h"
#include "image.h"


class RichTextClass
{
public:

	RichTextClass ();
	RichTextClass (const int x, const int y, const int w, const int h,
				SDL_Surface* ParentSurf = Screen);
	RichTextClass (const std::string& FmtText, 
			SDL_Surface* ParentSurf = Screen);
	RichTextClass (const int x, const int y, const int w, const int h,
				const std::string& FmtText, SDL_Surface *ParentSurf = Screen);
	
	// copying and assignmening and assignmentt 
	RichTextClass::RichTextClass (const RichTextClass& rhs);
	RichTextClass& operator= (const RichTextClass& rhs);

	~RichTextClass ();


	bool Move (const int x, const int y);
	bool Resize (const int w, const int h);
	bool ChCaption (const std::string& FmtText);
	bool ChParentSurf (SDL_Surface *ParentSurf);
	bool SizeToText ();

	bool Draw ();
	
private:
	Image _Surface;
	SDL_Rect _Rect;
	SDL_Surface* _ParentSurf;

	std::string _FmtText;

	int _Size;
	int _Style;
	Uint8 _Fr, _Fg, _Fb;
	Uint8 _Br, _Bg, _Bb;
	bool _UseBg;

	void Init (const int x, const int y, const int w, const int h,
			 const std::string& FmtText, SDL_Surface* ParentSurf);
	void Init (const int x, const int y, const std::string& FmtText,
			 SDL_Surface* ParentSurf);
	
	bool Parse ();
	std::vector<TextClass> _Text;	
};

#endif // RICHTEXT_H
