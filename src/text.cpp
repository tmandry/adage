#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *screen;

#include "text.h"
#include "image.h"
#include "font.h"

Text::Text()
{
	init("", screen, "Neuropol.ttf", 10, 0, 0);
}

Text::Text(const std::string& caption, const int size,
	SDL_Surface* parent_surf)
{
	init(caption, parent_surf, "Neuropol.ttf", size, 0, 0);
}

Text::Text(const std::string& caption, const int x, const int y,
	SDL_Surface* parent_surf)
{
	init(caption, parent_surf, "Neuropol.ttf", 10, x, y);
}

Text::Text(const std::string& caption, const int size, const int x,
	const int y, SDL_Surface* parent_surf)
{
	init (caption, parent_surf, "Neuropol.ttf", size, x, y);
}

Text::Text(const std::string& caption, const std::string& Font, 
	const int size, const int x, const int y, SDL_Surface* parent_surf)
{
	init (caption, parent_surf, Font, size, x, y);
}


// Copy constructor
// FIXME: If we get any dynamically allocated memory,
// this will need to be rewritten
Text::Text(const Text& rhs)
{
	*this = rhs;
}


// Assignment operator
Text& Text::operator=(const Text& rhs)
{
	if (this == &rhs)
		return *this;

	m_caption        = rhs.m_caption;
	m_font_file      = rhs.m_font_file;
	m_font           = rhs.m_font;
	m_size           = rhs.m_size;
	m_style          = rhs.m_style;
	m_surface        = rhs.m_surface;
	m_parent_surf    = rhs.m_parent_surf;
	m_area           = rhs.m_area;
	m_text_color     = rhs.m_text_color;
	m_use_background = rhs.m_use_background;
	m_bg_color       = rhs.m_bg_color;
	return *this;
};


Text::~Text()
{
}

void Text::init(const std::string& caption, SDL_Surface* parent_surf,
	const std::string& font, const int size, const int x, const int y)
{
	m_use_background = false;
	m_parent_surf = parent_surf;

	m_font_file = font;
	resize(size);
	change_caption(caption);
	change_color(0xff, 0xff, 0xff);
	move(x, y);
	reset_style();
}

bool Text::resize(const int size)
{
	Font tmp_font = TTF_OpenFont(m_font_file.c_str(), size);
	if (!tmp_font) {
		std::cout << "TTF_OpenFont() error: " << TTF_GetError();
		std::cout << std::endl;
		return false;
	}

	m_size = size;
	m_font = tmp_font;

	return true;
}

bool Text::change_caption(const std::string& caption)
{
	m_caption = caption;
	return true;
}

bool Text::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;
	return true;
}

bool Text::change_color(const Uint8 r, const Uint8 g, const Uint8 b)
{
	m_text_color.r = r;
	m_text_color.g = g;
	m_text_color.b = b;
	return true;
}

bool Text::change_background_color(const Uint8 r, const Uint8 g, const Uint8 b)
{
	m_bg_color = SDL_MapRGB(m_parent_surf->format, r, g, b);
	m_use_background = true;
	return true;
}

bool Text::disable_background()
{
	m_use_background = false;
	return true;
}

bool Text::change_parent_surf(SDL_Surface* parent_surf)
{
	m_parent_surf = parent_surf;
	return true;
}


bool Text::bold()
{
	m_style ^= TTF_STYLE_BOLD;
	return (m_style & TTF_STYLE_BOLD) ? true : false;
}

bool Text::underline()
{
	m_style ^= TTF_STYLE_UNDERLINE;
	return (m_style & TTF_STYLE_UNDERLINE)  ? true : false;
}

bool Text::italic()
{
	m_style ^= TTF_STYLE_ITALIC;
	return (m_style & TTF_STYLE_ITALIC) ? true : false;
}

int Text::get_style()
{
	return m_style;
}

int Text::set_style(int style)
{
	m_style = style;
	return m_style;
}

int Text::reset_style()
{
	m_style = TTF_STYLE_NORMAL;
	return m_style;
}


bool Text::draw()
{
	TTF_SetFontStyle(m_font.get(), m_style);
	
	Image tmp = TTF_RenderText_Blended(m_font.get(), m_caption.c_str(), 
		m_text_color);
	
	if (!tmp) {
		std::cout << "TTF_RenderText_Blended() error: ";
		std::cout << TTF_GetError() << std::endl;
		return false;
	}
	
	m_surface = tmp;

	m_area.w = m_surface->w;
	m_area.h = m_surface->h;
	
	if (m_use_background) 
		SDL_FillRect(m_parent_surf, &m_area, m_bg_color);
	SDL_BlitSurface(m_surface.get(), NULL, m_parent_surf, &m_area);
	return true;
}


SDL_Rect Text::get_size()
{
	SDL_Rect size;
	int w, h;
	
	TTF_SetFontStyle(m_font.get(), m_style);
	TTF_SizeText(m_font.get(), m_caption.c_str(), &w, &h);
	size.w = w;
	size.h = h;
	return size;
}
