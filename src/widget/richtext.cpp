/** \file richtext.cpp
 ** \brief Contains the RichText class source code
 **/

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

#include "SDL.h"
#include "SDL_ttf.h"

#include "text.h"
#include "richtext.h"
#include "misc/rgbamask.h"
#include "util/image.h"

/// Default constructor
RichText::RichText()
{
	init(0, 0, 0, 0, "", screen);
}

/// Overloaded constructor
/**
 ** @param x The x coordinate of the upper-left corner of the RichText on the parent surface
 ** @param y The y coordinate of the upper-left corner of the RichText on the parent surface
 ** @param w The width of the RichText
 ** @param h The height of the RichText
 ** @param parent_surf The surface that the RichText will be blitted onto
 **/
RichText::RichText(const int x, const int y, const int w, const int h,
	Image parent_surf)
{
	init(x, y, w, h, "", parent_surf);
}

/// Overloaded constructor
/**
 ** @param format_text The formatted text
 ** @param parent_surf The surface that the RichText will be blitted onto
 **/
RichText::RichText(const std::string& format_text, Image parent_surf)
{
	init(0, 0, 0, 0, format_text, parent_surf, true);
}

/// Overloaded constructor
/**
 ** @param x The x coordinate of the upper-left corner of the RichText on the parent surface
 ** @param y The y coordinate of the upper-left corner of the RichText on the parent surface
 ** @param w The width of the RichText
 ** @param h The height of the RichText
 ** @param format_text The formatted text
 ** @param parent_surf The surface that the RichText will be blitted onto
 **/
RichText::RichText(const int x, const int y, const int w,
	const int h, const std::string& format_text, Image parent_surf)
{
	init(x, y, w, h, format_text, parent_surf);
}

/// Destructor
RichText::~RichText()
{
}


/// Performs initialization of the RichText class and its members
void RichText::init(const int x, const int y, const int w, const int h,
	const std::string& format_text, Image parent_surf, bool auto_size)
{
	assert (parent_surf);
	
	m_size = 20; 
	m_style = TTF_STYLE_NORMAL;
	m_fr = m_fg = m_fb = 0xff;
	m_br = m_bg = m_bb = 0x00;
	m_use_background = false;

	move(x, y);
	m_format_text = format_text;	
	if (auto_size) {
		parse();
		size_to_text();
	} else {
		resize(w, h);
		parse();
	}
	set_parent(parent_surf);
}


/// Resizes m_surface
/**
 ** @param w The width of the surface
 ** @param h The height of the surface
 **/
bool RichText::resize(const int w, const int h)
{
	m_area.w = w; m_area.h = h;
	
	Image tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
		rmask, gmask, bmask, amask);
	
	if (!tmp) {
		std::cerr << "SDL_CreateRGBSurface() error: " << SDL_GetError();
		std::cerr << std::endl;
		return false;
	}

	m_surface = tmp;
	
	std::list<Text>::iterator i;
	for (i = m_text.begin(); i != m_text.end(); ++i)
		i->set_parent(m_surface);
	
	return true;
}

/// Sets the text of the RichText
/**
 ** @param format_text The formatted text string
 **/
bool RichText::set_caption(const std::string& format_text)
{
	m_format_text = format_text;

	return parse();
}

/// Resizes m_surface to fit the text
bool RichText::size_to_text()
{
	int w = 0, h = 0;

	std::list<Text>::iterator i;
	for (i = m_text.begin(); i != m_text.end(); ++i) {
		SDL_Rect size;
		size = i->get_area();

		int right = size.x + size.w;
		int bottom = size.y + size.h;
		if (right > w) 
			w = right;
		if (bottom > h) 
			h = bottom;
	}

	return resize(w, h);
}

/// Converts a formatted string of text into a vector<Text>
bool RichText::parse()
{
	std::string atom;

	int x = 0, y = 0;
	int row_height = 0;
	int size = m_size;
	int style = m_style;
	Uint8 fr = m_fr, fg = m_fg, fb = m_fb;
	Uint8 br = m_fr, bg = m_fg, bb = m_fb;
	bool use_background = m_use_background;

	union {
		unsigned long int Glob;
		struct {
			Uint8 b;
			Uint8 g;
			Uint8 r;
		};
	} raw_to_rgb;

	clear_children();
	m_text.clear();

	for (unsigned int i = 0; i < m_format_text.length(); ++i) {
		if (!(m_format_text[i] == '^' || m_format_text[i] == '\n')) {
			atom += m_format_text[i];
			continue;
		}

		if (!atom.empty()) {
			m_text.push_back(Text(atom, size, x, y, m_surface));

			Text& prev_atom(m_text.back());
			add_child(&prev_atom);
			
			prev_atom.set_style(style);
			prev_atom.resize(size);
			prev_atom.change_color(fr, fg, fb);
			if (use_background) prev_atom.change_background_color (br, bg, bb);
			
			SDL_Rect atom_size;
			atom_size = prev_atom.get_area();
			
			// This is to keep track of the overall height of the row,
			// so when there's a newline we know what to do
			if (atom_size.h > row_height) 
				row_height = atom_size.h;
			
			x += atom_size.w;

			atom.clear();
		}

		if (m_format_text[i] == '\n') {
			// We've hit a newline
			x = 0;
			y += row_height;
			row_height = 0;
			continue;
		}
		
		// We've hit a format code
		++i;

		switch (m_format_text[i]) {
		case 'B': // Bold
			style ^= TTF_STYLE_BOLD;
			break;

		case 'U': // Underline
			style ^= TTF_STYLE_UNDERLINE;
			break;

		case 'I': // Italic
			style ^= TTF_STYLE_ITALIC;
			break;

		case 'T': // Reset style
			style = m_style;
			break;

		case 'S': { // size
			std::stringstream convert;
			convert << m_format_text.substr(i+1, 2);
			convert >> size;
			i += 2;
			break;
		}

		case 's': // Reset size
			size = m_size;
			break;

		case 'C': { // Foreground color
			std::stringstream convert;
			convert << m_format_text.substr (i + 1, 6);
			convert >> std::hex >> raw_to_rgb.Glob;
			fr = raw_to_rgb.r; fg = raw_to_rgb.g; fb = raw_to_rgb.b;
			i += 6;
			break;
		}
		
		case 'G': { // Background color
			std::stringstream convert;
			convert << m_format_text.substr (i+1, 6);
			convert >> std::hex >> raw_to_rgb.Glob;
			br = raw_to_rgb.r; bg = raw_to_rgb.g; bb = raw_to_rgb.b;
			use_background = true;
			i += 6;
			break;
		}
		
		case 'c': // Reset foreground color
			fr = m_fr; fg = m_fg; fb = m_fb;
			break;

		case 'g': // Reset background color
			br = m_fr; bg = m_fg; bb = m_fb;
			use_background = m_use_background;
			break;

		case 'b': // Clear background color
			use_background = false;
			break;

		case 'R': // Reset EVERYTHING
			style = m_style;
			size = m_size;
			fr = m_fr; fg = m_fg; fb = m_fb;
			br = m_fr; bg = m_fg; bb = m_fb;
			use_background = m_use_background;
			break;
		
		case '^': // Literal ^ (caret)
			atom += '^';
			break;
		}
	}

	return true;
}

/// Draws the RichText.
/** In fact this function doesn't do anything; child widgets are the only thing
 ** that need to be drawn, and WidgetManager takes care of that.
 **
 ** @see Widget::draw()
 **/
bool RichText::draw()
{
	return true;
}
