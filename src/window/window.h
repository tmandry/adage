/** \file window.h
 ** \brief Defines the Window class
 **/

#ifndef WINDOW_H
#define WINDOW_H

#include <list>

#include "SDL.h"

#include "widget/widgetbase.h"
#include "util/image.h"

/// Displays a window on the screen
class Window {
public:
	// Constructor
	Window();

	// Destructor
	~Window();

	// Members
	void move(const int x, const int y);
	void resize(const int w, const int h);

	Widget* register_widget(Widget* ptr);
	/*void unregister_widget(const Widget* ptr);*/

	void draw();
	void blit();

	bool handle_event(const SDL_Event& event);

private:
	/// The Window's own surface
	Image m_surface;

	/// Defines the location and size of the Window on m_parent_surf
	SDL_Rect m_area;

	/// List of all widgets
	std::list<Widget*> m_widgets;
};	

#endif // WINDOW_H
