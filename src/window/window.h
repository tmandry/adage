/** \file window.h
 ** \brief Defines the Window class
 **/

#ifndef WINDOW_H
#define WINDOW_H

#include <list>
#include <memory>

#include "SDL.h"

#include "widget/widgetbase.h"
#include "boost/shared_ptr.hpp"
#include "util/image.h"

/// Displays a window on the screen
class Window {
public:
	typedef Widget::WidgetPointer WidgetPointer;
	typedef std::list<WidgetPointer> WidgetList;

	// Constructor
	Window();

	// Destructor
	~Window();

	// Members
	void move(const int x, const int y);
	void resize(const int w, const int h);

	WidgetPointer register_widget(Widget* ptr);
	/*void unregister_widget(const Widget* ptr);*/

	void draw();
	void blit();

	bool handle_event(const SDL_Event& event);

private:
	/// The Window's own surface
	Image m_surface;

	/// Defines the location and size of the Window on the screen
	SDL_Rect m_area;

	/// List of all widgets
	WidgetList m_widgets;
};	

#endif // WINDOW_H
