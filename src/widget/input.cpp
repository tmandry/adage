#include "SDL.h"

extern SDL_Surface *screen;

#include "input.h"
#include "text.h"
#include "misc/rgbamask.h"
#include "util/image.h"

Input::Input()
{
	init(0, 0, 0, 0, screen, 12);
}

Input::Input(const int x, const int y, const int w,
	SDL_Surface* parent_surf, const int text_size)
{
	init(x, y, w, text_size + 2, parent_surf, text_size);
}

Input::Input(const int x, const int y, const int w, const int h,
	SDL_Surface* parent_surf, const int text_size)
{
	init(x, y, w, h, parent_surf, text_size);
}

Input::Input(const Input& rhs)
{
	m_content = 0;
	m_surface = 0;
	*this = rhs;
}

Input& Input::operator=(const Input& rhs)
{
	delete[] m_content;

	// FIXME: Change this. We're through with C :)
	memcpy(this, &rhs, sizeof(Input));

	char* content = new char[strlen(m_content) + 1];
	m_content = content;

	m_text = rhs.m_text;
}

Input::~Input()
{
	delete[] m_content;
}


void Input::init(const int x, const int y, const int w, const int h,
	SDL_Surface* parent_surf, const int text_size)
{
	m_parent_surf = parent_surf;

	move(x, y);
	resize(w, h);
	resize_text(text_size);
	
	m_text.Move(1, 1);

	m_back_color = SDL_MapRGB(m_surface->format, 0x00, 0x00, 0x00);
	m_frame_color = SDL_MapRGB(m_surface->format, 0x22, 0x22, 0xbb);
}



bool Input::set_text(const std::string& text)
{
    m_content = text;
	return m_text.set_caption(text);
}

bool Input::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;
	
	return true;
}

bool Input::resize(const int w, const int h)
{
	Image tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 
		rmask, gmask, bmask, amask);
	tmp.set_deleter(SDL_FreeSurface);
	
	if (!tmp) {
		std::cout << "Input::resize(): SDL_CreateRGBSurface();
		std::cout << "failed: \n" << SDL_GetError() << std::endl;
		return false;
	}
	
	// I suppose this is meant to be here --lstor
	m_surface = tmp;
	
	m_text.change_parent_surf(m_surface.get());
	
	m_area.w = w;
	m_area.h = h;

	return true;
}

bool Input::resize_text(const int text_size)
{
	return m_text.resize(text_size);
}

bool Input::draw()
{
	// Background
	SDL_FillRect(m_surface.get(), 0, m_back_color);

	// Draw frame
	SDL_Rect line;
	line.x = 0; line.y = 0;
	line.w = m_area.w; line.h = 1;

	SDL_FillRect (m_surface.get(), &line, m_frame_color);
	line.w = 1; line.h = m_area.h;
	SDL_FillRect (m_surface.get(), &line, m_frame_color);
	line.x = m_area.w - 1;
	SDL_FillRect (m_surface.get(), &line, m_frame_color);
	line.x = 0; line.y = m_area.h - 1;
	line.w = m_area.w; line.h = 1;
	SDL_FillRect (m_surface.get(), &line, m_frame_color);

	// Text
	m_text.draw();

	// Blit
	SDL_BlitSurface(m_surface.get(), 0, m_parent_surf, &m_area);

	return true;
}

bool set_event_handler(const Uint8 event, const void (*handler)(Uint32))
{
}
