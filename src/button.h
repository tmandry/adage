#ifndef BUTTON_H
#define BUTTON_H

#include "text.h"
#include "image.h"

const int  BUTTON_STATE_UP		 =	0;
const int  BUTTON_STATE_DOWN	 =	1;
const int  BUTTON_STATE_FAKEDOWN =	2;
const int  BUTTON_STATE_HOVER	 =	3;
const int  BUTTON_EVENT_CLICK	 =	0;
const int  BUTTON_SIDE_WIDTH	 =	3;

// TODO: Implement this as bool IsDown();
#define BUTTONDOWN (_State==BUTTON_STATE_DOWN||_State==BUTTON_STATE_FAKEDOWN)

class ButtonClass
{
public:
	// Constructors
	ButtonClass ();
	ButtonClass (const char* Caption, const int x, const int y,
			   SDL_Surface* ParentSurf = Screen);
	ButtonClass (const char* Caption, const int x, const int y, const int w,
			   const int h, SDL_Surface* ParentSurf = Screen);
	ButtonClass (const char* Caption, const int x, const int y,
			   const int TextSize, SDL_Surface* ParentSurf = Screen);
	
	// Copying and assignment
	ButtonClass (const ButtonClass& rhs);
	ButtonClass& operator= (const ButtonClass& rhs);

	~ButtonClass ();

	bool ChCaption (const char* Caption, const bool resize = false);
	bool Move (const int x, const int y);
	bool Resize (const int w, const int h);
	bool ResizeText (const int size, const bool resize = false);
	bool SizeToText ();
	bool Draw ();

	void MouseButtonEvent (const Uint8 Button, const Uint8 State,
					   const Uint16 x, const Uint16 y);
	void MouseMotionEvent (const Uint8 State, const Uint16 x, const Uint16 y);

	bool SetEventHandler (const Uint8 Event, void (*Handler) ());

private:
	void Init (const char* Caption, const int x, const int y, const int w,
			 const int h, const int TextSize, const bool resize,
			 SDL_Surface* ParentSurf);
	
	void RenderButton (Uint32 FaceC, const Uint32 Side1C,
				    const Uint32 Side2C);

	void (*_HndClick) ();
	
	TextClass _Caption;

	Image _Surface;
	SDL_Surface* _ParentSurf;
	SDL_Rect _Rect;

	Uint8 _State;
};

#endif // BUTTON_H
