#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "handle.h"

SDL_Surface *Screen;

#include "text.h"
#include "button.h"
#include "richtext.h"
//#include "input.h"

ButtonClass *Button;
ButtonClass *BtnChColor;
TextClass *Text2;

Uint32 BlankColor;

void Button_Click ();
void BtnChColor_Click ();

int main (int argc, char *argv[])
{
	using namespace std;
	
	if ( SDL_Init (SDL_INIT_VIDEO) < 0 )
	{
		cerr << "SDL_Init() error: " << SDL_GetError () << endl;
		exit (1);
	}

	atexit (SDL_Quit);

	if ( TTF_Init () < 0 )
	{
		cerr << "TTF_Init() error: " << TTF_GetError () << endl;
		exit (1);
	}

	Screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF |
						 SDL_SRCALPHA);
	if ( !Screen )
	{
		cerr << "SDL_SetVideoMode error: " << SDL_GetError () << endl;
		exit (1);
	}

	SDL_WM_SetCaption ("Adage Widget Set Preview", NULL);

	Uint8 Done = 0;
	SDL_Event Event;
	TextClass Text ("HELLO!", 30, 100, 100);
	
	Button = new ButtonClass (" Ping? ", 100, 150, 20);
	Button->SetEventHandler (BUTTON_EVENT_CLICK, Button_Click);

	BtnChColor = new ButtonClass ("Thou shalt not poketh me!", 200, 400, 10);
	BtnChColor->SetEventHandler (BUTTON_EVENT_CLICK, BtnChColor_Click);

	Text2 = new TextClass (" Pong!", 48, 300, 300);
	Text2->ChColor (0x00, 0x00, 0x00);
	Text2->ChBgColor (0x55, 0x55, 0xaa);

	RichTextClass RText (400, 200, 640, 200,
		 "^Gffffff^Cff0000Hello! ^Bbold\n^U^S12underline^g^B^U^I^s italic\n"
		 "^T^^caret!!^^ ^S22bye!^R :P", 
		 Screen);

	/*InputClass Input (400, 200, 100);
	Input.SetText ("HeRrO!");*/

	BlankColor = SDL_MapRGB (Screen->format, 0x00, 0x00, 0x00);
	while ( !Done )
	{
		while ( SDL_PollEvent (&Event) )
		{
			switch ( Event.type )
			{
			case SDL_QUIT:
				Done = 1;
				break;
			case SDL_KEYDOWN:
				if ( Event.key.keysym.sym == SDLK_ESCAPE )
					Done = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				Button->MouseButtonEvent (Event.button.button,
									 Event.button.state,
									 Event.button.x, Event.button.y);
				BtnChColor->MouseButtonEvent (Event.button.button,
										Event.button.state,
										Event.button.x,
										Event.button.y);
				break;
			case SDL_MOUSEMOTION:
				Button->MouseMotionEvent (Event.motion.state,
									 Event.motion.x, Event.motion.y);
				BtnChColor->MouseMotionEvent (Event.motion.state,
										Event.motion.x,
										Event.motion.y);
				break;
			}
		}
		
		SDL_FillRect (Screen, NULL, BlankColor);

		Text.Draw ();
		Button->Draw ();
		BtnChColor->Draw ();
		Text2->Draw ();
		RText.Draw ();
		//Input.Draw ();
		SDL_Flip (Screen);
	}

	return 0;
}

void Button_Click ()
{
	static Uint8 status = 0;
	status ^= 1;

	if ( status )
	{
		Button->ChCaption (" Pong! ");
		Text2->ChCaption (" Ping? ");
	}
	else
	{
		Button->ChCaption (" Ping? ");
		Text2->ChCaption (" Pong! ");
	}
}

void BtnChColor_Click ()
{
	static Uint8 status = 0;
	status ^= 1;

	if ( status )
		BlankColor = SDL_MapRGB (Screen->format, 0x33, 0xdd, 0x33);
	else
		BlankColor = SDL_MapRGB (Screen->format, 0x00, 0x00, 0x00);
}
