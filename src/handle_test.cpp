#include "handle.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
	using namespace std;
	
	typedef basic_handle<SDL_Surface> Image;
	typedef basic_handle<TTF_Font> Font;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cerr << "Unable to init SDL\n";
		exit(1);
	}
	atexit(SDL_Quit);

	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16, 0);
	if (!screen) {
		cerr << "Unable to set video mode\n";
		exit(1);
	}

	if (TTF_Init() != 0) {
		cerr << "Unable to init TTF\n";
		exit(1);
	}
	atexit(TTF_Quit);

	Font f1 = TTF_OpenFont("Neuropol.ttf", 15);
	f1.set_deleter(TTF_CloseFont);
//	f1.set_name("f1");
	
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
	
	Image i1 = TTF_RenderText_Solid(f1.get(), "Hello, world", white);
	i1.set_deleter(SDL_FreeSurface);
//	i1.set_name("i1");
	
	SDL_BlitSurface(i1.get(), 0, screen, 0);
	SDL_Flip(screen);
	SDL_Delay(2000);

	Image i2 = i1;
//	i2.set_name("i2");
	SDL_Rect middle = { 320, 240, 0, 0 };
	SDL_BlitSurface(i2.get(), 0, screen, &middle);
	SDL_Flip(screen);
	SDL_Delay(2000);

	if (i2) {
		cout << "i2 is a good pointer\n";
	} else {
		cout << "i2 is a bad pointer\n";
	}
	
	i2 = TTF_RenderText_Solid(f1.get(), "Goodbye, world!", white);
//	i2.set_deleter(SDL_FreeSurface);
	SDL_FillRect(screen, 0, 0);
	SDL_BlitSurface(i2.get(), 0, screen, 0);
	SDL_BlitSurface(i1.get(), 0, screen, &middle);
	SDL_Flip(screen);
	SDL_Delay(2000);
	
 return 0;
}
