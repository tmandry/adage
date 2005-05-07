#ifndef BUTTON_H
#define BUTTON_H

#include "text.h"
#include "image.h"

class Button {
public:
	enum {button_event_click};
	
	// Constructors
	Button();
	Button(const char* caption, const int x, const int y,
		SDL_Surface* parent_surf = screen);
	Button(const char* caption, const int x, const int y, const int w,
		const int h, SDL_Surface* parent_surf = screen);
	Button(const char* caption, const int x, const int y,
		const int text_size, SDL_Surface* parent_surf = screen);
	
	// Copying and assignment
	Button(const Button& rhs);
	Button& operator=(const Button& rhs);

	~Button();

	bool change_caption(const char* caption, const bool resize = false);
	bool move(const int x, const int y);
	bool resize(const int w, const int h);
	bool resize_text(const int size, const bool resize = false);
	bool size_to_text();
	bool draw();

	void mouse_button_event(const Uint8 button, const Uint8 state,
		const Uint16 x, const Uint16 y);
	void mouse_motion_event(const Uint8 state, const Uint16 x, const Uint16 y);

	bool set_event_handler(const Uint8 event, void (*handler)());
	
	inline bool is_down();

private:
	void init(const char* caption, const int x, const int y, const int w,
		const int h, const int text_size, const bool resize, 
		SDL_Surface* parent_surf);
	
	void render_button(Uint32 face_color, const Uint32 side1_color,
		const Uint32 side2_color);

	void (*m_handle_click)();
	

	Text m_caption;
	Image m_surface;
	
	SDL_Surface* m_parent_surf;
	SDL_Rect m_area;

	Uint8 m_state;
	
	enum {button_state_up, button_state_down, button_state_fakedown,
		button_state_hover};

	enum {side_width = 3};
};

#endif // BUTTON_H
