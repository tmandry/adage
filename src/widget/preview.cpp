/** \file preview.cpp
 ** \brief Contains the source code for the Adage %Widget Set Preview
 **/

#include <cstdlib>
#include <iostream>

#include "SDL.h"

#include "boost/shared_ptr.hpp"
#include "util/image.h"
#include "widget.h"
#include "window/window.h"
#include "window/windowmanager.h"
#include "io/gameconfig.h"
/*#include "input.h"*/

Image screen;

// Globals like this won't happen outside the preview; everything will be owned
// by a class
Text* text2;

Uint32 blank_color;

void clean_up();

void button_click(Button&);
void button_change_color(Button&);

int main(int argc, char* argv[])
{
	using namespace std;
	
	Config* config = Config::get_ptr();
	config->read_config();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init() error: " << SDL_GetError() << std::endl;
		exit (1);
	}

	atexit (SDL_Quit);

	if (TTF_Init() < 0)	{
		std::cerr << "TTF_Init() error: " << TTF_GetError() << std::endl;
		exit (1);
	}

	screen = SDL_SetVideoMode(
		config->get_int("screenw"),
		config->get_int("screenh"),
		config->get_int("bpp"),
		SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_SRCALPHA |
		(config->get_bool("fullscreen") ? SDL_FULLSCREEN : 0));
	
	if (!screen) {
		std::cerr << "SDL_SetVideoMode error: " << SDL_GetError() << std::endl;
		exit (1);
	}
	
	atexit(clean_up);

	SDL_WM_SetCaption("Adage Widget Set Preview", NULL);
	// TODO: Set an icon?

	Window window(screen);
//	window.move(0,0);
//	window.resize(config->get_int("screenw"),config->get_int("screenh"));
	
	Text* text;
	Button* button1;
	Button* button2;
	RichText* richtext;
	
	window.register_widget(text = new Text("HELLO!", 30, 100, 100));
	
	window.register_widget(button1 = new Button(" Ping? ", 100, 150, 20));
	button1->set_event_handler(Button::button_event_click, button_click);

	window.register_widget(button2 =
		new Button("Thou shalt not poketh me!",200, 400, 10));
	button2->set_event_handler(Button::button_event_click, button_change_color);

	window.register_widget(text2 = new Text(" Pong! ", 48, 300, 300));
	text2->change_color(0x00, 0x00, 0x00);
	text2->change_background_color(0x55, 0x55, 0xaa);

	window.register_widget(richtext =
		new RichText(400, 200, 640, 200,
			"asdf^Gffffff^Cff0000Hello! ^Bbold\n^U^S12underline^g^B^U^I"
			"^s italic\n^T^^caret!!^^ ^S22bye!^R^Cffffff :P",
			screen)
	);
	
	blank_color = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	
	Uint8 done = 0;
	WindowManager* window_manager = WindowManager::get_ptr();
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
				break;
			default:
				window_manager->handle_event(event);
			}
		}
		
		SDL_FillRect(screen.get(), NULL, blank_color);

		window_manager->draw();
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

//This is done before SDL_Quit, because otherwise SDL_Quit would deallocate it
//automatically, and then the class would try to deallocate it
void clean_up()
{
	screen.reset();
}
