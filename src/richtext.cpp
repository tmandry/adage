#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *screen;

#include "text.h"
#include "richtext.h"
#include "rgbamask.h"
#include "image.h"

//using std::cout;
//using std::endl;

RichText::RichText()
{
	init(0, 0, 0, 0, "", screen);
}

RichText::RichText(const int x, const int y, const int w,
	const int h, SDL_Surface* parent_surf)
{
	init(x, y, w, h, "", parent_surf);
}

RichText::RichText(const std::string& format_text, 
	SDL_Surface* parent_surf)
{
	init(0, 0, format_text, parent_surf);
}

RichText::RichText(const int x, const int y, const int w,
	const int h, const std::string& format_text, SDL_Surface* parent_surf)
{
	init(x, y, w, h, format_text, parent_surf);
}

RichText::~RichText()
{
}


void RichText::init(const int x, const int y, const int w, const int h,
	const std::string& format_text, SDL_Surface* parent_surf)
{
	m_size = 20; 
	m_style = TTF_STYLE_NORMAL;
	m_fr = m_fg = m_fb = 0xff;
	m_fr = m_fg = m_fb = 0x00;
	m_use_background = true;

	move(x, y);
	resize(w, h);
	change_caption(format_text);
	change_parent_surf(parent_surf);
}

void RichText::init(const int x, const int y, const std::string& format_text,
	SDL_Surface* parent_surf)
{
	m_style = TTF_STYLE_NORMAL;
	m_fr = m_fg = m_fb = 0xff;
	m_fr = m_fg = m_fb = 0x00;
	m_use_background = true;

	move(x, y);
	change_caption(format_text);
	size_to_text();
	change_parent_surf(parent_surf);
}

bool RichText::move(const int x, const int y)
{
	m_area.x = x; 
	m_area.y = y;
	return true;
}

bool RichText::resize(const int w, const int h)
{
	m_area.w = w; m_area.h = h;
	
	Image tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
		rmask, gmask, bmask, amask);
	
	if (!tmp) {
		std::cout << "SDL_CreateRGBSurface() error: " << SDL_GetError();
		std::cout << std::endl;
		return false;
	}

	m_surface = tmp;
	
	return true;
}

bool RichText::change_caption(const std::string& format_text)
{
	m_format_text = format_text;
	m_text.clear();

	return parse();
}

bool RichText::change_parent_surf(SDL_Surface* parent_surf)
{
	m_parent_surf = parent_surf;
	return true;
}

bool RichText::size_to_text()
{
	int w = 0, h = 0;

	std::vector<Text>::iterator i;
	for (i = m_text.begin(); i < m_text.end(); ++i)	{
		SDL_Rect size;
		size = i->get_size();

		int right = size.x + size.w;
		int bottom = size.y + size.h;
		if (right > w) 
			w = right;
		if (bottom > h) 
			h = bottom;
	}

	return resize(w, h);
}


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

	for (unsigned int i = 0; i < m_format_text.size(); ++i) {
		if (!(m_format_text[i] == '^' || m_format_text[i] == '\n'))	{
			atom += m_format_text[i];
			continue;
		}

		if (m_format_text[i] == '^' && i == m_format_text.size() - 1) {
			atom += m_format_text[i];
			continue;
		}

		if (!atom.empty() || m_format_text[i] == '\n') {
			m_text.push_back(Text(atom.c_str(), size, x, y, m_surface.get()));

			std::vector<Text>::iterator prev_atom;
			prev_atom = m_text.end() - 1;
			
			prev_atom->set_style(style);
			prev_atom->resize(size);
			
			SDL_Rect atom_size;
			atom_size = prev_atom->get_size();
			
			// This is to keep track of the overall height of the row,
			// so when there's a newline we know what to do
			if (atom_size.h > row_height) 
				row_height = atom_size.h;
			
			if (m_format_text[i] == '\n') {
				x = 0;
				y += row_height;
				row_height = 0;
			} else
				x += atom_size.w;

			prev_atom->change_color(fr, fg, fb);
			if (use_background) 
				prev_atom->change_background_color (br, bg, bb);

			atom.clear();
		}

		if (m_format_text[i] == '\n') 
			continue;
		
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

bool RichText::draw()
{
	std::vector<Text>::iterator i;
	for (i = m_text.begin(); i < m_text.end(); ++i)	
		i->draw();
	
	SDL_BlitSurface(m_surface.get(), 0, m_parent_surf, &m_area);
	
	return true;
}
