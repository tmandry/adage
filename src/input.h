#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

#include "text.h"
#include "image.h"

class InputClass
{
public:
	InputClass ();
	InputClass (const int x, const int y, const int w,
			  SDL_Surface* ParentSurf = Screen, const int TextSize = 12);
	InputClass (const int x, const int y, const int w, const int h,
			  SDL_Surface* ParentSurf = Screen, const int TextSize = 12);
	
	// copy constructor
	InputClass::InputClass (const InputClass& rhs)
	{	
		_Content = 0;
		_Surface = 0;
		*this = Old;
	}

	void InputClass::operator= (const InputClass &Old)
	{
		delete [] _Content;
		memcpy (this, &Old, sizeof (InputClass));

		char* Content = new char [strlen (_Content) + 1];
		_Content = Content;

		_Text = Old._Text;
	}
	
	~InputClass ();

	bool SetText (const std::string& Text);
	bool Move (const int x, const int y);
	bool Resize (const int w, const int h);
	bool ResizeText (const int TextSize = 12);
	bool Draw ();
	
	char* GetText ();

	bool SetEventHandler (const Uint8 Event, const void (*Handler) (Uint32));
	void 

private:
	void Init (const int x, const int y, const int w, const int h,
			 SDL_Surface* ParentSurf, const int TextSize);

	SDL_Rect _Rect;
	TextClass _Text;
	
	std::string _Content;
	int _TextSize;

	Uint32 _BackColor;
	Uint32 _FrameColor;

	Image _Surface;
	SDL_Surface* _ParentSurf;
};

#endif // INPUT_H
