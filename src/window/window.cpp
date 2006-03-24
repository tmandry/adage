/** \file window.cpp
 ** \brief Contains the source code for the Window class
 **/

#include <iostream>
#include <list>
#include <cassert>

#include "SDL.h"

#include "window.h"
#include "windowmanager.h"
#include "misc/rgbamask.h"
#include "boost/shared_ptr.hpp"

extern Image screen;

/// Constructor; registers the Window with WindowManager
Window::Window()
{
	WindowManager::get_ptr()->register_window(this);
}

/// Constructor; registers the Window with WindowManager, and sets
/// surface to an already created surface.
Window::Window(const Image& surface)
{
	WindowManager::get_ptr()->register_window(this);

	m_surface = surface;
	m_area.x = m_area.y = 0;
	m_area.w = m_surface->w;
	m_area.h = m_surface->h;
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

	SDL_Surface *surf = NULL;

	if(!m_surface)
		surf = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
			w, h, 32, rmask, gmask, bmask, amask);
	else
		surf = SDL_CreateRGBSurfaceFrom(m_surface->pixels, w, h, 32,
			m_surface->pitch, rmask, gmask, bmask, amask);

	Image tmp(surf);
	
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
Window::WidgetPointer Window::register_widget(Widget* ptr)
{
	assert (ptr);
	WidgetPointer widget(ptr);
	
	m_widgets.push_front(widget);
	widget->set_parent(m_surface);
	return widget;
}

/// Unregisters an old Widget with the Window
/*
 ** @param ptr A pointer to the old Widget
 **/
/*void Window::unregister_widget(const Widget* ptr)
{
	m_widgets.remove(ptr);
}*/

/// Tells widgets to render themselves on their own surface
void Window::draw()
{
	for (WidgetList::iterator i(m_widgets.begin());
		i != m_widgets.end();
		++i) {
			WidgetList grandchildren((*i)->get_children());
			for (WidgetList::iterator j(grandchildren.begin());
				j != grandchildren.end();
				++j)
					(*j)->draw();
			(*i)->draw();
		}
}

/// Tells widgets to blit themselves on the window's surface; blits window to screen
void Window::blit()
{
	for (WidgetList::iterator i(m_widgets.begin());
		i != m_widgets.end();
		++i) {
			WidgetList grandchildren((*i)->get_children());
			for (WidgetList::iterator j(grandchildren.begin());
				j != grandchildren.end();
				++j)
					(*j)->blit();
			(*i)->blit();
		}
	
	if (SDL_BlitSurface(m_surface.get(), 0, screen.get(), &m_area) < 0)
		std::cerr <<  "Window::blit(): SDL_BlitSurface error: \n"
			<< SDL_GetError() << std::endl;
}

/// Sends an event to every widget
/**
 ** @param event The event to send
 **/
bool Window::handle_event(const SDL_Event& event)
{
	WidgetList::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		if ((*i)->handle_event(event)) return true;
	
	return false;
}
