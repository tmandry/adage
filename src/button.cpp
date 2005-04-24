#include <iostream>

#include "SDL.h"

extern SDL_Surface *screen;

#include "button.h"
#include "rgbamask.h"
#include "image.h"

// Default constructor
Button::Button()
{
	init("", 0, 0, 0, 0, 0, false, screen);
}


// Overloaded constructor
Button::Button(const char* caption, const int x, const int y,
					 SDL_Surface* parent_surf)
{
	init(caption, x, y, 0, 0, 0, true, parent_surf);
}


// Overloaded constructor
Button::Button(const char* caption, const int x, const int y,
					 const int w, const int h, SDL_Surface* parent_surf)
{
	init(caption, x, y, w, h, 0, false, parent_surf);
}


// Overloaded constructor
Button::Button(const char* caption, const int x, const int y,
					 const int text_size, SDL_Surface* parent_surf)
{
	init(caption, x, y, 0, 0, text_size, true, parent_surf);
}


// Copy ctor
Button::Button(const Button& rhs)
{
	*this = rhs;
}


// Assignment operator
Button& Button::operator=(const Button& rhs)
{
	if (this == &rhs)
		return *this;

	m_hnd_click   = rhs.m_hnd_click;
	m_caption     = rhs.m_caption;
	m_surface     = rhs.m_surface;
	m_parent_surf = rhs.m_parent_surf;
	m_area        = rhs.m_area;
	m_state       = rhs.m_state;
	return *this;
}


// Destructor
Button::~Button()
{
}


void Button::init(const char* caption, const int x, const int y, 
	const int w, const int h, const int text_size, 
	const bool resize, SDL_Surface* parent_surf)
{
	m_state = BUTTON_STATE_UP;

	m_hnd_click = 0;

	m_parent_surf = parent_surf;
	
	move(x, y);
	resize(w, h);
	
	if (text_size) 
		resize_text(text_size);
	change_caption(caption);
	if (resize)
		size_to_text();
	m_caption.move(BUTTON_SIDE_WIDTH, BUTTON_SIDE_WIDTH);
}



bool Button::change_caption(const char* caption, const bool resize)
{
	if (!m_caption.change_caption(caption)) 
		return false;

	if (resize) 
		size_to_text();

	return true;
}

bool Button::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;

	return true;
}

bool Button::resize(const int w, const int h)
{
	m_area.w = w;
	m_area.h = h;

	Image tmp = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
			w, h, 32, rmask, gmask, bmask, amask);
	
	if (!tmp) {
		std::cout << "Button::Resize(): CreateRGBSurface() failed: "
			<< SDL_GetError() << std::endl;
		return false;
	}

	m_surface = tmp;
	m_caption.change_parent_surf(m_surface.get());

	return true;
}

bool Button::resize_text(int size, bool resize)
{
	if (!m_caption.resize(size)) 
		return false;

	if (resize) 
		return size_to_text();

	return true;
}

bool Button::size_to_text ()
{
	SDL_Rect size;
	size = m_caption.get_size();
	
	return resize(size.w + BUTTON_SIDE_WIDTH,
				size.h + BUTTON_SIDE_WIDTH);
}

bool Button::draw()
{
	Uint8 face_r, face_g, face_b;
	Uint8 side1_r, side1_g, side1_b;
	Uint8 side2_r, side2_g, side2_b;
	Uint32 face_color, side1_color, side2_color;

	switch (m_state) {
	case BUTTON_STATE_UP:
		face_r  = face_g  = 0x55; face_b  = 0xee;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
		break;
	case BUTTON_STATE_DOWN:
	case BUTTON_STATE_FAKEDOWN:
		face_r  = face_g  = 0x33; face_b  = 0xcc;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
		break;
	case BUTTON_STATE_HOVER:
		face_r  = face_g  = 0x66; face_b  = 0xff;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
	}

	face_color  = SDL_MapRGB(m_surface->format, face_r, face_g, face_b);
	side1_color = SDL_MapRGB(m_surface->format, side1_r, side1_g, side1_b);
	side2_color = SDL_MapRGB(m_surface->format, side2_r, side2_g, side2_b);
	RenderButton(face_color, side1_color, side2_color);
	/*if ( SDL_FillRect (m_surface, 0, Color) < 0 )
	{
		printf ("Button::Draw(): SDL_FillRect() error: %s\n",
			   SDL_GetError ());
		return false;
	}*/

	int text_loc;
	text_loc = BUTTONDOWN ? 3 : 0;
	m_caption.move(text_loc, text_loc);
	if (!m_caption.draw()) 
		return false;

	if (SDL_BlitSurface(m_surface.get(), 0, m_parent_surf, &m_area) < 0) {
		std::cout <<  "Button::Draw(): SDL_BlitSurface error: \n"
			<< SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

inline void step_grad(Uint8 &r, Uint8 &g, Uint8 &b)
{
	if ((r > 0) && (g > 0) && (b > 0))	{
		r--;
		g--;
		b--;
	}
}

void Button::render_button(Uint32 face_color, const Uint32 side1_color,
	const Uint32 side2_color)
{
	// FIXME: Comments!
	SDL_Rect face, side, black;

	face.x = BUTTONDOWN ? BUTTON_SIDE_WIDTH : 0;
	face.y = BUTTONDOWN ? BUTTON_SIDE_WIDTH : 0;
	face.w = m_area.w - BUTTON_SIDE_WIDTH;
	face.h = m_area.h - BUTTON_SIDE_WIDTH;
	side.x = BUTTONDOWN ? 0 : face.w;
	side.w = 0; side.h = 1;
	black.x = face.w; black.y = 0;
	black.w = BUTTON_SIDE_WIDTH; black.h = 1;

	// Most of this will be covered
	SDL_FillRect(m_surface.get(), 0, side2_color);

	// Face
	Uint8 face_r, face_g, face_b;
	SDL_GetRGB(face_color, m_surface->format, &face_r, &face_g, &face_b);
	
	int i = (face.w > face.h) ? face.w : face.h;
	SDL_Rect grad;
	SDL_Rect grad_clip;

	// FIXME: Clean this up...
	for (grad_clip.x = grad_clip.y = grad.x = grad.y = face.x,
		 grad.w = grad.h = i; i >= 0; i -= 1) {
		grad_clip.w = (grad.w > face.w) ? face.w : grad.w;
		grad_clip.h = (grad.h > face.h) ? face.h : grad.h;
		
		SDL_FillRect(m_surface.get(), &grad_clip, face_color);
		
		step_grad(face_r, face_g, face_b);
		face_color = SDL_MapRGB(m_surface->format, face_r, face_g, face_b);
		
		grad.h  = i;
		grad.w -= 1;
		grad.x += 1;
	}

	Uint32 black_color;
	black_color = SDL_MapRGBA (m_surface->format, 0x00, 0x00, 0x00, 0x00);

	// Other side
	// FIXME: Comments! What is going on here?
	for (side.y = 0; side.y < m_area.h; ++side.y) {
		if (side.y < BUTTON_SIDE_WIDTH) {
			++side.w;
			++black.x; 
			--black.w;
			black.y = side.y;
			
			SDL_FillRect(m_surface.get(), &black, black_color);
		}

		if (side.y == face.h) {
			black.x = 0;
			black.w = 0;
		}
			
		if (side.y > face.h) {
			++side.x; 
			--side.w;
			++black.w;
			black.y = side.y;

			SDL_FillRect(m_surface.get(), &black, black_color);
		}

		SDL_FillRect(m_surface.get(), &side, side1_color);
	}
}

void Button::mouse_button_event(const Uint8 button, const Uint8 state,
	const Uint16 x, const Uint16 y)
{
	// Is the mouse pointer over the button?
	if (x >= m_area.x && x <= (m_area.x + m_area.w) &&
		y >= m_area.y && y <= (m_area.y + m_area.h)) {

		// Is it already clicked?
		if (state == SDL_PRESSED) {
			m_state = BUTTON_STATE_DOWN;
		} else {
			// If not, perform the callback
			if (m_state == BUTTON_STATE_DOWN && m_hnd_click) {
				m_hnd_click();
			}

			m_state = BUTTON_STATE_HOVER;
		}
	} else {
		if (state == SDL_RELEASED)
			m_state = BUTTON_STATE_UP;
	}
}

void Button::mouse_motion_event(const Uint8 state, const Uint16 x,
	const Uint16 y)
{
	// Is the mouse pointer over the button?
	if (x >= m_area.x && x <= (m_area.x + m_area.w) &&
		y >= m_area.y && y <= (m_area.y + m_area.h)) {

		if (m_state != BUTTON_STATE_DOWN) {
			if (State & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				m_state = BUTTON_STATE_FAKEDOWN;
			} else {
				m_state = BUTTON_STATE_HOVER;
			}
		}
	} else {
		if (!(m_state == BUTTON_STATE_DOWN &&
			State & SDL_BUTTON (SDL_BUTTON_LEFT))) {
			m_state = BUTTON_STATE_UP;
		}
	}
}


bool Button::set_event_handler(const Uint8 event, void (*handler)())
{
	switch (event) {
	case BUTTON_EVENT_CLICK:
		m_hnd_click = handler;
		break;
	default:
		return false;
	}

	return true;
}
