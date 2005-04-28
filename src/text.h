#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "image.h"
#include "font.h"

class Text {
public:
	Text();
	Text(const std::string& caption, const int size,
		SDL_Surface* parent_surf = screen);
	Text(const std::string& caption, const int x, const int y,
		SDL_Surface* parent_surf = screen);
	Text(const std::string& caption, const int size, const int x,
		const int y, SDL_Surface* parent_surf = screen);
	Text(const std::string& caption, const std::string& font, 
		const int size, const int x, const int y, 
		SDL_Surface* parent_surf = screen);
	
	// copy and assignment
	Text(const Text& rhs);
	Text& operator=(const Text& rhs);

	~Text();

	bool resize(const int size);
	bool change_caption(const std::string& caption);
	bool move(const int x, const int y);
	bool change_color(const Uint8 r, const Uint8 g, const Uint8 b);
	bool change_background_color(const Uint8 r, const Uint8 g, const Uint8 b);
	bool change_background_color();
	bool change_parent_surf(SDL_Surface* parent_surf);

	bool bold();
	bool underline();
	bool italic();
	int get_style();
	int set_style(int style);
	int reset_style();
	
	bool draw();

	SDL_Rect get_size();

private:
	void init(const std::string& caption, SDL_Surface* parent_surf, 
		const std::string& font, const int size, const int x, const int y);
	
	std::string m_caption;
	std::string m_font_file;
	Font m_font;
	int m_size;
	int m_style;
	
	Image m_surface;
	SDL_Surface* m_parent_surf;
	SDL_Rect m_area;
	
	SDL_Color m_text_color;
	bool m_use_background;
	Uint32 m_bg_color;
};

#endif // TEXT_H
