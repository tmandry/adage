#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"
#include "text.h"
#include "image.h"

extern Image screen;

class Button : public Widget {
public:
	typedef void (*handler_function)(Button&);
	
	enum {button_event_click};
	
	// Constructors
	Button();
	Button(const char* caption, const int x, const int y,
		Image parent_surf = screen);
	Button(const char* caption, const int x, const int y, const int w,
		const int h, Image parent_surf = screen);
	Button(const char* caption, const int x, const int y,
		const int text_size, Image parent_surf = screen);
	
	// Copying and assignment
	Button(const Button& rhs);
	Button& operator=(const Button& rhs);

	~Button();

	bool change_caption(const char* caption, const bool resize = false);
	bool resize(const int w, const int h);
	bool resize_text(const int size, const bool resize = false);
	bool size_to_text();
	bool draw();

	bool handle_event(const SDL_Event& event);
	bool set_event_handler(const Uint8 event, handler_function handler);
	
	inline bool is_down();

private:
	void init(const char* caption, const int x, const int y, const int w,
		const int h, const int text_size, const bool resize, 
		Image parent_surf);
	
	void render_button(Uint32 face_color, const Uint32 side1_color,
		const Uint32 side2_color);

	handler_function m_handle_click;
	
	Text m_caption;

	Uint8 m_state;
	
	enum {button_state_up, button_state_down, button_state_fakedown,
		button_state_hover};

	enum {side_width = 3};
};

#endif // BUTTON_H
