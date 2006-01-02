/** \file text.cpp
 ** \brief Contains the Text class source code
 **/

#include <string>
#include <iostream>
#include <cassert>

#include "SDL.h"
#include "SDL_ttf.h"

#include "text.h"
#include "util/image.h"
#include "util/font.h"

/// Default constructor
Text::Text()
{
	init("", screen, "Neuropol.ttf", 10, 0, 0);
}

/// Overloaded constructor
/**
 ** @param caption The text to display
 ** @param size The size of the text
 ** @param parent_surf The surface that the Text will be blitted onto
 **/
Text::Text(const std::string& caption, const int size, Image parent_surf)
{
	init(caption, parent_surf, "Neuropol.ttf", size, 0, 0);
}

/// Overloaded constructor
/**
 ** @param caption The text to display
 ** @param x The x coordinate of the upper-left corner of the Text on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Text on the parent surface
 ** @param parent_surf The surface that the Text will be blitted onto
 **/
Text::Text(const std::string& caption, const int x, const int y,
	Image parent_surf)
{
	init(caption, parent_surf, "Neuropol.ttf", 10, x, y);
}

/// Overloaded constructor
/**
 ** @param caption The text to display
 ** @param size The size of the text
 ** @param x The x coordinate of the upper-left corner of the Text on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Text on the parent surface
 ** @param parent_surf The surface that the Text will be blitted onto
 **/
Text::Text(const std::string& caption, const int size, const int x, const int y,
	Image parent_surf)
{
	init (caption, parent_surf, "Neuropol.ttf", size, x, y);
}

/// Overloaded constructor
/**
 ** @param caption The text to display
 ** @param font The filename of the font to use
 ** @param size The size of the text
 ** @param x The x coordinate of the upper-left corner of the Text on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Text on the parent surface
 ** @param parent_surf The surface that the Text will be blitted onto
 **/
Text::Text(const std::string& caption, const std::string& font, const int size,
	const int x, const int y, Image parent_surf)
{
	init (caption, parent_surf, font, size, x, y);
}


// FIXME: If we get any dynamically allocated memory,
// this will need to be rewritten
/// Copy constructor
Text::Text(const Text& rhs)
{
	*this = rhs;
}


/// Assignment operator
Text& Text::operator=(const Text& rhs)
{
	assert (this != &rhs);

	m_caption        = rhs.m_caption;
	m_font_file      = rhs.m_font_file;
	m_font           = rhs.m_font;
	m_size           = rhs.m_size;
	m_style          = rhs.m_style;
	m_surface        = rhs.m_surface;
	set_parent        (rhs.get_parent());
	m_area           = rhs.m_area;
	m_text_color     = rhs.m_text_color;
	m_use_background = rhs.m_use_background;
	m_bg_color       = rhs.m_bg_color;
	return *this;
};


/// Destructor
Text::~Text()
{
}


/// Performs initialization of the Text class and its members
void Text::init(const std::string& caption, Image parent_surf,
	const std::string& font, const int size, const int x, const int y)
{
	/*assert (parent_surf);*/
	
	m_use_background = false;
	set_parent(parent_surf);

	m_font_file = font;
	resize(size);
	set_caption(caption);
	change_color(0xff, 0xff, 0xff);
	move(x, y);
	reset_style();
}


/// Does nothing - resizing the text will resize the widget
bool Text::resize(const int w, const int h)
{
	// Should be resized with text size!
	
	// There may be something calling this function legally (a function that
	// processes general Widgets), but it's not likely and this assertion will
	// be removed in that case
	assert (false);
	return false;
}

/// Resizes the text and the widget
/**
 ** @param size The size of the text
 **/
bool Text::resize(const int size)
{
	Font tmp_font = TTF_OpenFont(m_font_file.c_str(), size);
	if (!tmp_font) {
		std::cerr << "TTF_OpenFont() error: " << TTF_GetError();
		std::cerr << std::endl;
		return false;
	}

	m_size = size;
	m_font = tmp_font;

	return true;
}

/// Sets the text to be displayed by the Text class
/**
 ** @param caption An unformatted string of text
 **/
bool Text::set_caption(const std::string& caption)
{
	m_caption = caption;
	return true;
}

/// Changes the color of the text
/**
 ** @param r The red component
 ** @param g The green component
 ** @param b The blue component
 **/
bool Text::change_color(const Uint8 r, const Uint8 g, const Uint8 b)
{
	m_text_color.r = r;
	m_text_color.g = g;
	m_text_color.b = b;
	return true;
}

/// Changes the background color of the text
/**
 ** @param r The red component
 ** @param g The green component
 ** @param b The blue component
 **/
bool Text::change_background_color(const Uint8 r, const Uint8 g, const Uint8 b)
{
	m_bg_color.r = r;
	m_bg_color.g = g;
	m_bg_color.b = b;
	m_use_background = true;
	return true;
}

/// Disables the background of the text, making it transparent
bool Text::disable_background()
{
	m_use_background = false;
	return true;
}


/// Bolds the text
bool Text::bold()
{
	m_style ^= TTF_STYLE_BOLD;
	return (m_style & TTF_STYLE_BOLD) ? true : false;
}

/// Underlines the text
bool Text::underline()
{
	m_style ^= TTF_STYLE_UNDERLINE;
	return (m_style & TTF_STYLE_UNDERLINE)  ? true : false;
}

/// Italicizes the text
bool Text::italic()
{
	m_style ^= TTF_STYLE_ITALIC;
	return (m_style & TTF_STYLE_ITALIC) ? true : false;
}

/// Returns the style code of the text
int Text::get_style()
{
	return m_style;
}

/// Sets the style code of the text
int Text::set_style(int style)
{
	m_style = style;
	return m_style;
}

/// Resets the text style
int Text::reset_style()
{
	m_style = TTF_STYLE_NORMAL;
	return m_style;
}


/// Draws the text to m_surface
/**
 ** @see Widget::draw()
 **/
bool Text::draw()
{
	TTF_SetFontStyle(m_font.get(), m_style);
	
	Image tmp;
	if (m_use_background)
		tmp = TTF_RenderText_Shaded(m_font.get(), m_caption.c_str(), 
			m_text_color, m_bg_color);
	else
		tmp = TTF_RenderText_Solid(m_font.get(), m_caption.c_str(), 
			m_text_color);
	
	if (!tmp) {
		std::cerr << "TTF_RenderText_Blended/Solid() error: ";
		std::cerr << TTF_GetError() << std::endl;
		return false;
	}
	
	m_surface = tmp;

	m_area.w = m_surface->w;
	m_area.h = m_surface->h;


	return true;
}


// TODO: When caching is implemented, make this more efficient
/// Returns the size of the text
const SDL_Rect& Text::get_area()
{
	int w, h;
	
	TTF_SetFontStyle(m_font.get(), m_style);
	TTF_SizeText(m_font.get(), m_caption.c_str(), &w, &h);
	m_area.w = w;
	m_area.h = h;
	return m_area;
}
