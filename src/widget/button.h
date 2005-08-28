/** \file button.h
 ** \brief Contains the Button class definition
 **/

#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "widget/widgetbase.h"
#include "widget/text.h"
#include "util/image.h"

extern Image screen;

/// A graphical button with text and a gradient fill
class Button : public Widget {
public:
	/// Prototype for Button event handler functions
	typedef void (*handler_function)(Button&);
	
	/// Button events
	enum event_type {button_event_click};
	
	// Constructors
	Button();
	Button(const std::string& caption, const int x, const int y,
		Image parent_surf = screen);
	Button(const std::string& caption, const int x, const int y, const int w,
		const int h, Image parent_surf = screen);
	Button(const std::string& caption, const int x, const int y,
		const int text_size, Image parent_surf = screen);
	
	// Copying and assignment
	Button(const Button& rhs);
	Button& operator=(const Button& rhs);

	~Button();

	bool set_caption(const std::string& caption, const bool resize = false);
	bool resize(const int w, const int h);
	bool resize_text(const int size, const bool resize = false);
	bool size_to_text();
	bool draw();

	bool handle_event(const SDL_Event& event);
	bool set_event_handler(const event_type event, handler_function handler);
	
	inline bool is_down();

private:
	void init(const std::string& caption, const int x, const int y,
		const int w, const int h, const int text_size, const bool resize,
		Image parent_surf);

	void render_button(Uint32 face_color, const Uint32 side1_color,
		const Uint32 side2_color);

	/// Pointer to the click event handler function
	handler_function m_handle_click;

	/// Button text
	Text m_caption;

	/// Current button state
	Uint8 m_state;

	/// Button states
	enum {button_state_up, button_state_down, button_state_fakedown,
		button_state_hover};

	/// The width of each side of the button
	enum {side_width = 3};
};

#endif // BUTTON_H
