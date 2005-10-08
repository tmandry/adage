/** \file main.cpp
 ** \brief Implements main()
 **/

#include <iostream>

#include "SDL.h"

#include "util/smartptr.h"
#include "util/image.h"
#include "widget.h"
#include "window/window.h"
#include "window/windowmanager.h"
#include "io/gameconfig.h"

Image screen;

int main(int argc, char* argv[])
{
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

	SDL_WM_SetCaption("Adage", NULL);
	// TODO: Set an icon
	
	Window window;
	window.move(0,0);
	window.resize(640,480);
	window.register_widget(new Button("Clicky", 320, 10));
	window.register_widget(new RichText("^Cff0000red ^C00ff00^Bgreen^B ^C0000ff"
		"^Iblue"))
		->move(320, 240);
	
	bool done = false;
	WindowManager* window_manager = WindowManager::get_ptr();
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = true;
				break;
			default:
				window_manager->handle_event(event);
			}
		}
		window_manager->draw();
		SDL_Flip(screen.get());
	}
	
	return 0;
}
