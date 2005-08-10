/** \file button.cpp
 ** \brief Contains the Button class source code
 **/

#include <iostream>
#include <string>

#include "SDL.h"

#include "button.h"
#include "misc/rgbamask.h"
#include "utils/image.h"

/// Default constructor
Button::Button()
{
	init("", 0, 0, 0, 0, 0, false, screen);
}


/// Overloaded constructor
/**
 ** @param caption The button's text
 ** @param x The x coordinate of the upper-left corner of the Button on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Button on the parent surface
 ** @param parent_surf The surface that the Button will be blitted onto
 **/
Button::Button(const std::string& caption, const int x, const int y,
	Image parent_surf)
{
	init(caption, x, y, 0, 0, 0, true, parent_surf);
}

/// Overloaded constructor
/**
 ** @param caption The button's text
 ** @param x The x coordinate of the upper-left corner of the Button on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Button on the parent surface
 ** @param w The width of the Button in pixels
 ** @param h The height of the Button in pixels
 ** @param parent_surf The surface that the Button will be blitted onto
 **/
Button::Button(const std::string& caption, const int x, const int y,
	const int w, const int h, Image parent_surf)
{
	init(caption, x, y, w, h, 0, false, parent_surf);
}

/// Overloaded constructor
/**
 ** @param caption The button's text
 ** @param x The x coordinate of the upper-left corner of the Button on the parent surface
 ** @param y The y coordinate of the upper-left corner of the Button on the parent surface
 ** @param text_size The size of the text
 ** @param parent_surf The surface that the Button will be blitted onto
 **/
Button::Button(const std::string& caption, const int x, const int y,
	const int text_size, Image parent_surf)
{
	init(caption, x, y, 0, 0, text_size, true, parent_surf);
}


/// Copy constructor
Button::Button(const Button& rhs)
{
	*this = rhs;
}


/// Assignment operator
Button& Button::operator=(const Button& rhs)
{
	if (this == &rhs)
		return *this;

	m_handle_click= rhs.m_handle_click;
	m_caption     = rhs.m_caption;
	set_parent     (rhs.get_parent());
	m_area        = rhs.m_area;
	m_state       = rhs.m_state;
	resize         (m_area.w, m_area.h);
	return *this;
}


/// Destructor
Button::~Button()
{
}


/// Performs initialization of the Button class and its members
void Button::init(const std::string& caption, const int x, const int y, 
	const int w, const int h, const int text_size, 
	const bool do_resize, Image parent_surf)
{
	m_state = button_state_up;

	m_handle_click = 0;

	set_parent(parent_surf);
	
	move(x, y);
	resize(w, h);
	
	if (text_size) 
		resize_text(text_size);
	set_caption(caption);
	if (do_resize)
		size_to_text();
	m_caption.move(side_width, side_width);
}



/// Sets the button text
/**
 ** @param caption The button text
 ** @param resize Whether or not to resize the button to fit the text
 **/
bool Button::set_caption(const std::string& caption, const bool resize)
{
	if (!m_caption.set_caption(caption)) 
		return false;

	if (resize) 
		size_to_text();

	return true;
}

/// Resizes the button
/**
 ** @param w The button width
 ** @param h The button height
 **/
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
	m_caption.set_parent(m_surface);

	return true;
}

/// Resizes the button text
/**
 ** @param size Text size
 ** @param resize Whether or not to resize the button to fit the text
 **/
bool Button::resize_text(int size, bool resize)
{
	if (!m_caption.resize(size)) 
		return false;

	if (resize) 
		return size_to_text();

	return true;
}

/// Resizes the button to fit the text
bool Button::size_to_text()
{
	SDL_Rect size;
	size = m_caption.get_size();
	
	return resize(size.w + side_width,
		size.h + side_width);
}

/// Draws the button on m_surface
/**
 ** @see Widget::draw()
 **/
bool Button::draw()
{
	Uint8 face_r, face_g, face_b;
	Uint8 side1_r, side1_g, side1_b;
	Uint8 side2_r, side2_g, side2_b;
	Uint32 face_color, side1_color, side2_color;

	switch (m_state) {
	case button_state_up:
		face_r  = face_g  = 0x55; face_b  = 0xee;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
		break;
	case button_state_down:
	case button_state_fakedown:
		face_r  = face_g  = 0x33; face_b  = 0xcc;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
		break;
	case button_state_hover:
		face_r  = face_g  = 0x66; face_b  = 0xff;
		side1_r = side1_g = 0x33; side1_b = 0xcc;
		side2_r = side2_g = 0x22; side2_b = 0xbb;
	}

	face_color  = SDL_MapRGB(m_surface->format, face_r, face_g, face_b);
	side1_color = SDL_MapRGB(m_surface->format, side1_r, side1_g, side1_b);
	side2_color = SDL_MapRGB(m_surface->format, side2_r, side2_g, side2_b);
	render_button(face_color, side1_color, side2_color);
	/*if ( SDL_FillRect (m_surface, 0, Color) < 0 )
	{
		printf ("Button::Draw(): SDL_FillRect() error: %s\n",
			   SDL_GetError ());
		return false;
	}*/

	int text_loc;
	text_loc = is_down() ? 3 : 0;
	m_caption.move(text_loc, text_loc);

	return true;
}

/// Used by render_button() to move a color a "step" towards black
inline void step_grad(Uint8 &r, Uint8 &g, Uint8 &b)
{
	if ((r > 0) && (g > 0) && (b > 0))	{
		r--;
		g--;
		b--;
	}
}

/// Renders the actual button graphic
/**
 ** @param face_color The 'base' color of the face gradient
 ** @param side1_color The color of the side of the button
 ** @param side2_color The color of the top or bottom of the button
 **/
void Button::render_button(Uint32 face_color, const Uint32 side1_color,
	const Uint32 side2_color)
{
	// face is the main color of the button, side is the brighter-colored side,
	// and black is the rectangle that covers up the side where it shouldn't be
	// (on the corners)
	SDL_Rect face, side, black;

	face.x = is_down() ? side_width : 0;
	face.y = is_down() ? side_width : 0;
	face.w = m_area.w - side_width;
	face.h = m_area.h - side_width;
	side.x = is_down() ? 0 : face.w;
	side.w = 0; side.h = 1;

	// Side - most of this will be covered
	SDL_FillRect(m_surface.get(), 0, side2_color);

	// Face
	Uint8 face_r, face_g, face_b;
	SDL_GetRGB(face_color, m_surface->format, &face_r, &face_g, &face_b);
	
	// Paint the gradient
	int i = (face.w > face.h) ? face.w : face.h;
	SDL_Rect grad;
	SDL_Rect grad_clip;

	grad_clip.x = grad_clip.y = grad.x = grad.y = face.x, grad.w = grad.h = i;
	for (; i >= 0; i -= 1) {
		grad_clip.w = (grad.w > face.w) ? face.w : grad.w;
		grad_clip.h = (grad.h > face.h) ? face.h : grad.h;
		
		SDL_FillRect(m_surface.get(), &grad_clip, face_color);
		
		step_grad(face_r, face_g, face_b);
		face_color = SDL_MapRGB(m_surface->format, face_r, face_g, face_b);
		
		grad.h  = i;
		grad.w -= 1;
		grad.x += 1;
	}

	black.x = face.w; black.y = 0;
	black.w = side_width; black.h = 1;
	
	Uint32 black_color;
	black_color = SDL_MapRGBA (m_surface->format, 0x00, 0x00, 0x00, 0x00);

	// Create the side on the short side of the button
	for (side.y = 0; side.y < m_area.h; ++side.y) {
		if (side.y < side_width) {
			// First step:
			// Increase the side width while moving the black over one,
			// making an angle at the corner
			++side.w;
			++black.x; 
			--black.w;
			black.y = side.y;
			
			SDL_FillRect(m_surface.get(), &black, black_color);
		}

		// Transition to second step
		if (side.y == face.h) {
			black.x = 0;
			black.w = 0;
		}

		if (side.y > face.h) {
			// Second step:
			// We're doing two things at once here. The black is covering up
			// the long side and making the angle at the other corner, while
			// the short side is getting smaller and making an angle where it
			// meets the long one.
			++side.x; 
			--side.w;
			++black.w;
			black.y = side.y;

			SDL_FillRect(m_surface.get(), &black, black_color);
		}

		SDL_FillRect(m_surface.get(), &side, side1_color);
	}
}


/// Receives an event from the WidgetManager and handles it
/**
 ** @param event The event to handle
 ** @see WidgetManager::handle_event()
 **/
bool Button::handle_event(const SDL_Event& event)
{
	Uint8 button, state;
	Uint16 x, y;
	
	// These will only be used if it's a mouse event
	button = event.button.button; state = event.button.state;
	x = event.button.x; y = event.button.y;
	
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		// Is the mouse pointer over the button?
		if (x >= m_area.x && x <= (m_area.x + m_area.w) &&
			y >= m_area.y && y <= (m_area.y + m_area.h)) {

			// Is it already clicked?
			if (state == SDL_PRESSED) {
				m_state = button_state_down;
			} else {
				// If not, perform the callback
				if (m_state == button_state_down && m_handle_click) {
					m_handle_click(*this);
				}

				m_state = button_state_hover;
			}
		} else {
			if (state == SDL_RELEASED)
				m_state = button_state_up;
		}
	case SDL_MOUSEMOTION:
		// Is the mouse pointer over the button?
		if (x >= m_area.x && x <= (m_area.x + m_area.w) &&
			y >= m_area.y && y <= (m_area.y + m_area.h)) {

			if (m_state != button_state_down) {
				m_state = button_state_hover;
			}
		} else {
			if (!(m_state == button_state_down &&
				m_state & SDL_BUTTON (SDL_BUTTON_LEFT))) {
				m_state = button_state_up;
			}
		}
	default:
		return false;
	}
	
	return true;
}

/// Sets the event handler function for the specified event
/**
 ** @param event The event to set the handler for
 ** @param handler A pointer to the handler function
 **/
bool Button::set_event_handler(const event_type event, handler_function handler)
{
	switch (event) {
	case button_event_click:
		m_handle_click = handler;
		break;
	default:
		return false;
	}

	return true;
}


/// Checks to see if the button is down
bool Button::is_down()
{
	return (m_state == button_state_down || m_state == button_state_fakedown);
}
