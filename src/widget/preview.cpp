/** \file preview.cpp
 ** \brief Contains the source code for the Adage %Widget Set Preview
 **/

#include <cstdlib>
#include <iostream>

#include "SDL.h"

#include "widget.h"
#include "util/smartptr.h"
#include "util/image.h"
#include "widget/manager.h"
/*#include "input.h"*/

Image screen;

// Globals like this won't happen outside the preview; everything will be owned
// by a class
Text* text2;

Uint32 blank_color;

void button_click(Button&);
void button_change_color(Button&);

int main(int argc, char* argv[])
{
	using namespace std;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
	Text text("HELLO!", 30, 100, 100);
	
	Button button(" Ping? ", 100, 150, 20);
	button.set_event_handler(Button::button_event_click, button_click);

	Button change_color("Thou shalt not poketh me!", 200, 400, 10);
	change_color.set_event_handler(Button::button_event_click,
		button_change_color);

	text2 = new Text(" Pong! ", 48, 300, 300);
	text2->change_color(0x00, 0x00, 0x00);
	text2->change_background_color(0x55, 0x55, 0xaa);

	RichText rtext(400, 200, 640, 200,
		 "asdf^Gffffff^Cff0000Hello! ^Bbold\n^U^S12underline^g^B^U^I^s italic\n"
		 "^T^^caret!!^^ ^S22bye!^R^Cffffff :P",
		 screen);
	
	SDL_Event event;
	WidgetManager* widget_manager = WidgetManager::get_ptr();
	blank_color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	
	while (!done) {
		widget_manager->handle_event(event);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
				break;
			}
		}
		
		SDL_FillRect(screen.get(), NULL, blank_color);

		widget_manager->draw();
		SDL_Flip(screen.get());
	}

	return 0;
}

void button_click(Button& button)
{
	static Uint8 status = 0;
	status ^= 1;

	if (status) {
		button.set_caption(" Pong! ");
		text2->set_caption(" Ping? ");
	} else {
		button.set_caption(" Ping? ");
		text2->set_caption(" Pong! ");
	}
}

void button_change_color(Button& change_color)
{
	static Uint8 status = 0;
	status ^= 1;

	if (status)
		blank_color = SDL_MapRGB(screen->format, 0x33, 0xdd, 0x33);
	else
		blank_color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
}
