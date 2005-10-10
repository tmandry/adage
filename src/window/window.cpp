/** \file window.cpp
 ** \brief Contains the source code for the Window class
 **/

#include <iostream>
#include <list>

#include "SDL.h"

#include "window.h"
#include "windowmanager.h"
#include "misc/rgbamask.h"

/// Constructor; registers the Window with WindowManager
Window::Window()
{
	WindowManager::get_ptr()->register_window(this);
}

/// Destructor; unregisters the Window with WindowManager
Window::~Window()
{
	WindowManager::get_ptr()->unregister_window(this);
}


/// Moves the window to another location
/**
 ** @param x The x coordinate
 ** @param y The y coordinate
 **/
void Window::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;
}

/// Resizes m_surface
void Window::resize(const int w, const int h)
{
	m_area.w = w;
	m_area.h = h;

	Image tmp = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
			w, h, 32, rmask, gmask, bmask, amask);
	
	if (!tmp) {
		std::cerr << "Window::resize(): CreateRGBSurface() failed: "
			<< SDL_GetError() << std::endl;
		return;
	}

	m_surface = tmp;
}

/// Reigsters a newly-created Widget with the Window
/**
 ** @param ptr A pointer to the new Widget
 **/
Widget* Window::register_widget(Widget* ptr)
{
	m_widgets.push_front(ptr);
	
	/*std::list<Widget*> grandchildren(ptr->get_children());
	m_widgets.splice(m_widgets.begin(), grandchildren);*/
	ptr->set_parent(m_surface);
	return ptr;
}

/// Unregisters an old Widget with the Window
/*
 ** @param ptr A pointer to the old Widget
 **/
/*void Window::unregister_widget(const Widget* ptr)
{
	// Search for the pointer in the list and erase it
	std::list<Widget*>::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		if (*i == ptr) {
			m_widgets.erase (i);
			break;
		}
}*/

/// Tells widgets to render themselves on their own surface
void Window::draw()
{
	for (std::list<Widget*>::iterator i(m_widgets.begin());
		i != m_widgets.end();
		++i) {
			std::list<Widget*> grandchildren((*i)->get_children());
			for (std::list<Widget*>::iterator j(grandchildren.begin());
				j != m_widgets.end();
				++j)
					(*j)->draw();
			(*i)->draw();
		}
}

/// Tells widgets to blit themselves on the window's surface; blits window to screen
void Window::blit()
{
	for (std::list<Widget*>::iterator i(m_widgets.begin());
		i != m_widgets.end();
		++i) {
			std::list<Widget*> grandchildren((*i)->get_children());
			for (std::list<Widget*>::iterator j(grandchildren.begin());
				j != m_widgets.end();
				++j)
					(*j)->blit();
			(*i)->blit();
		}
}

/// Sends an event to every widget
/**
 ** @param event The event to send
 **/
bool Window::handle_event(const SDL_Event& event)
{
	std::list<Widget*>::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		if ((*i)->handle_event(event)) return true;
	
	return false;
}
