#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "handle.h"

SDL_Surface *screen;

#include "text.h"
#include "button.h"
#include "richtext.h"
//#include "input.h"

// FIXME: Globals are bad...
Button* button;
Button* change_color;
Text* text2;

Uint32 blank_color;

void button_click();
void button_change_color();

int main(int argc, char *argv[])
{
	using namespace std;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cerr << "SDL_Init() error: " << SDL_GetError() << endl;
		exit (1);
	}

	atexit (SDL_Quit);

	if (TTF_Init() < 0)	{
		cerr << "TTF_Init() error: " << TTF_GetError() << endl;
		exit (1);
	}

	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF |
		SDL_SRCALPHA);
	
	if (!screen) {
		cerr << "SDL_SetVideoMode error: " << SDL_GetError() << endl;
		exit (1);
	}

	SDL_WM_SetCaption("Adage Widget Set Preview", NULL);
	// TODO: Set an icon?

	Uint8 done = 0;
	SDL_Event event;
	Text text("HELLO!", 30, 100, 100);
	
	button = new Button(" Ping? ", 100, 150, 20);
	button->set_event_handler(BUTTON_EVENT_CLICK, button_click);

	change_color = new Button ("Thou shalt not poketh me!", 200, 400, 10);
	change_color->set_event_handler(BUTTON_EVENT_CLICK, button_change_color);

	text2 = new Text(" Pong!", 48, 300, 300);
	text2->change_color(0x00, 0x00, 0x00);
	text2->change_background_color(0x55, 0x55, 0xaa);

	RichText rtext(400, 200, 640, 200,
		 "^Gffffff^Cff0000Hello! ^Bbold\n^U^S12underline^g^B^U^I^s italic\n"
		 "^T^^caret!!^^ ^S22bye!^R :P", 
		 screen);

	/*InputClass Input (400, 200, 100);
	Input.SetText ("HeRrO!");*/

	blank_color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				button->mouse_button_event(event.button.button,
					event.button.state, event.button.x, event.button.y);
				change_color->mouse_button_event(event.button.button,
					event.button.state,	event.button.x,	event.button.y);
				break;
			case SDL_MOUSEMOTION:
				button->mouse_motion_event(event.motion.state,
					event.motion.x, event.motion.y);
				change_color->mouse_motion_event(event.motion.state,
					event.motion.x,	event.motion.y);
				break;
			}
		}
		
		SDL_FillRect(screen, NULL, blank_color);

		text.draw();
		button->draw();
		change_color->draw();
		text2->draw();
		rtext.draw();
		//Input.draw ();
		SDL_Flip(screen);
	}

	return 0;
}

void button_click()
{
	static Uint8 status = 0;
	status ^= 1;

	if (status) {
		button->change_caption(" Pong! ");
		text2->change_caption(" Ping? ");
	} else {
		button->change_caption(" Ping? ");
		text2->change_caption(" Pong! ");
	}
}

void button_change_color()
{
	static Uint8 status = 0;
	status ^= 1;

	if (status)
		blank_color = SDL_MapRGB(screen->format, 0x33, 0xdd, 0x33);
	else
		blank_color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
}
