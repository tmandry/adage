#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "image.h"
#include "font.h"

class TextClass
{
public:
	TextClass ();
	TextClass (const std::string& Caption, const int Size,
			 SDL_Surface* ParentSurf = Screen);
	TextClass (const std::string& Caption, const int x, const int y,
			 SDL_Surface* ParentSurf = Screen);
	TextClass (const std::string& Caption, const int Size, const int x,
			 const int y, SDL_Surface* ParentSurf = Screen);
	TextClass (const std::string& Caption, const std::string& Font, 
			const int Size, const int x, const int y, 
			SDL_Surface* ParentSurf = Screen);
	
	// copy and assignment
	TextClass (const TextClass& rhs);
	TextClass& operator= (const TextClass& rhs);

	~TextClass ();

	bool Resize (const int Size);
	bool ChCaption (const std::string& Caption);
	bool Move (const int x, const int y);
	bool ChColor (const Uint8 r, const Uint8 g, const Uint8 b);
	bool ChBgColor (const Uint8 r, const Uint8 g, const Uint8 b);
	bool ChBgColor ();
	bool ChParentSurf (SDL_Surface* ParentSurf);

	bool Bold ();
	bool Underline ();
	bool Italic ();
	int GetStyle ();
	int SetStyle (int Style);
	int ResetStyle ();
	
	bool Draw ();

	SDL_Rect GetSize ();

private:
	void Init (const std::string& Caption, SDL_Surface* ParentSurf, 
		const std::string& Font, const int Size, const int x, const int y);
	
	std::string _Caption;
	std::string _FontFile;
	Font _Font;
	int _Size;
	int _Style;
	
	Image _Surface;
	SDL_Surface* _ParentSurf;
	SDL_Rect _Rect;
	
	SDL_Color _TextColor;
	bool _UseBg;
	Uint32 _BgColor;
};

#endif // TEXT_H
