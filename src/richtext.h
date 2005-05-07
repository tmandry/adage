#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <vector>

#include "SDL.h"

#include "text.h"
#include "image.h"


class RichText {
public:

	RichText();
	RichText(const int x, const int y, const int w, const int h,
		SDL_Surface* parent_surf = screen);
	RichText(const std::string& format_text, 
		SDL_Surface* parent_surf = screen);
	RichText(const int x, const int y, const int w, const int h,
		const std::string& format_text, SDL_Surface* parent_surf = screen);
	
	// copying and assignmening and assignmentt 
	RichText::RichText(const RichText& rhs);
	RichText& operator=(const RichText& rhs);

	~RichText();


	bool move(const int x, const int y);
	bool resize(const int w, const int h);
	bool change_caption(const std::string& format_text);
	bool change_parent_surf(SDL_Surface* parent_surf);
	bool size_to_text();

	bool draw();
	
private:
	Image m_surface;
	SDL_Rect m_area;
	SDL_Surface* m_parent_surf;

	std::string m_format_text;

	int m_size;
	int m_style;
	// Foreground red, green, blue
	Uint8 m_fr, m_fg, m_fb;
	// Background red, green, blue
	Uint8 m_br, m_bg, m_bb;
	
	bool m_use_background;

	void init(const int x, const int y, const int w, const int h,
			 const std::string& format_text, SDL_Surface* parent_surf);
	void init(const int x, const int y, const std::string& format_text,
			 SDL_Surface* parent_surf);
	
	bool parse();
	std::vector<Text> m_text;	
};

#endif // RICHTEXT_H
