#ifndef BUTTON_H
#define BUTTON_H

#include "text.h"
#include "image.h"


// TODO: Implement this as bool IsDown();
#define BUTTONDOWN (m_state==BUTTON_STATE_DOWN||m_state==BUTTON_STATE_FAKEDOWN)

// TODO: Move this inside Button
const int  BUTTON_STATE_UP		 =	0;
const int  BUTTON_STATE_DOWN	 =	1;
const int  BUTTON_STATE_FAKEDOWN =	2;
const int  BUTTON_STATE_HOVER	 =	3;
const int  BUTTON_EVENT_CLICK	 =	0;
const int  BUTTON_SIDE_WIDTH	 =	3;

class Button {
public:
	
	// Constructors
	Button();
	Button(const char* caption, const int x, const int y,
			   SDL_Surface* ParentSurf = screen);
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

	bool set_event_handler(const Uint8 event, void (*Handler)());

private:
	void init(const char* caption, const int x, const int y, const int w,
			 const int h, const int text_size, const bool resize, 
			 SDL_Surface* ParentSurf);
	
	void render_button(Uint32 face_color, const Uint32 side1_color,
				    const Uint32 side2_color);

	// FIXME: Make this name more descriptive...
	void (*m_hnd_click)();
	
	Text m_caption;
	Image m_surface;
	
	SDL_Surface* m_parent_surf;
	SDL_Rect m_area;

	Uint8 m_state;
};

#endif // BUTTON_H
