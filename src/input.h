#ifndef INPUT_H
#define INPUT_H

#include "text.h"
#include "image.h"

#include <string>

#include "SDL.h"

class Input {
public:
	Input();
	Input(const int x, const int y, const int w,
		SDL_Surface* parent_surf = screen, const int text_size = 12);
	Input(const int x, const int y, const int w, const int h,
		SDL_Surface* parent_surf = screen, const int text_size = 12);
	
	// copy constructor
	Input(const Input& rhs);
	void operator=(const Input &Old);
	
	~Input();

	bool set_text(const std::string& text);
	bool move(const int x, const int y);
	bool resize(const int w, const int h);
	bool resize_text(const int text_size = 12);
	bool draw();
	
	char* get_text();

	bool set_event_handler(const Uint8 event, const void (*handler) (Uint32));

private:
	void Init(const int x, const int y, const int w, const int h,
			 SDL_Surface* parent_surf, const int text_size);

	SDL_Rect m_area;
	TextClass m_text;
	
	std::string m_content;
	int m_text_size;

	Uint32 m_back_color;
	Uint32 m_frame_color;

	Image m_surface;
	SDL_Surface* m_parent_surf;
};

#endif // INPUT_H
