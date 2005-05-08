#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <vector>

#include "SDL.h"

#include "widget.h"
#include "text.h"
#include "image.h"

extern Image screen;

class RichText : public Widget {
public:

	RichText();
	RichText(const int x, const int y, const int w, const int h,
		Image parent_surf = screen);
	RichText(const std::string& format_text, Image parent_surf = screen);
	RichText(const int x, const int y, const int w, const int h,
		const std::string& format_text, Image parent_surf = screen);
	
	// Copying and assignment
	RichText::RichText(const RichText& rhs);
	RichText& operator=(const RichText& rhs);

	~RichText();

	bool resize(const int w, const int h);
	bool change_caption(const std::string& format_text);
	bool size_to_text();

	bool draw();
	
private:
	std::string m_format_text;

	int m_size;
	int m_style;
	// Foreground red, green, blue
	Uint8 m_fr, m_fg, m_fb;
	// Background red, green, blue
	Uint8 m_br, m_bg, m_bb;
	
	bool m_use_background;

	void init(const int x, const int y, const int w, const int h,
			 const std::string& format_text, Image parent_surf);
	void init(const int x, const int y, const std::string& format_text,
			 Image parent_surf);
	
	bool parse();
	std::vector<Text> m_text;	
};

#endif // RICHTEXT_H
