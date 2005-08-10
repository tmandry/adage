/** \file text.h
 ** \brief Contains the Text class definition
 **/

#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "widget.h"
#include "utils/image.h"
#include "utils/font.h"

extern Image screen;

/// Renders text with limited decoration
class Text : public Widget {
public:
	// Constructors
	Text();
	Text(const std::string& caption, const int size,
		Image parent_surf = screen);
	Text(const std::string& caption, const int x, const int y,
		Image parent_surf = screen);
	Text(const std::string& caption, const int size, const int x, const int y,
		Image parent_surf = screen);
	Text(const std::string& caption, const std::string& font, const int size,
		const int x, const int y, Image parent_surf = screen);
	
	// Copy and assignment
	Text(const Text& rhs);
	Text& operator=(const Text& rhs);

	~Text();

	bool resize(const int w, const int h);
	bool resize(const int size);
	bool set_caption(const std::string& caption);
	
	bool change_color(const Uint8 r, const Uint8 g, const Uint8 b);
	bool change_background_color(const Uint8 r, const Uint8 g, const Uint8 b);
	bool disable_background();

	bool bold();
	bool underline();
	bool italic();
	int get_style();
	int set_style(int style);
	int reset_style();
	
	bool draw();

	SDL_Rect get_size();

private:
	void init(const std::string& caption, Image parent_surf,
		const std::string& font, const int size, const int x, const int y);

	/// The text to display
	std::string m_caption;
	/// Font filename
	std::string m_font_file;
	Font m_font;
	/// Text size
	int m_size;
	/// Text style code
	int m_style;

	/// Text color
	SDL_Color m_text_color;
	/// Whether or not to use m_bg_color as the background
	bool m_use_background;
	/// Background color
	SDL_Color m_bg_color;
};

#endif // TEXT_H
