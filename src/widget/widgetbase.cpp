/** \file widgetbase.cpp
 ** \brief Contains the Widget base class source code
 **/

#include <iostream>
#include <cassert>

#include "SDL.h"

#include "boost/shared_ptr.hpp"
#include "misc/rgbamask.h"
#include "widgetbase.h"

/// Constructor
Widget::Widget(): m_changed(true)
{
}

/*Widget::Widget(const Widget& rhs)
{
	*this = rhs;
	WidgetManager::get_ptr()->register_widget(this);
}

Widget& Widget::operator=(const Widget& rhs)
{
	m_area = rhs.m_area;
}*/

/// Destructor
Widget::~Widget()
{
}


/// Sets the Widget's parent surface
/**
 ** @param parent The new parent surface
 **/
void Widget::set_parent(const Image& parent)
{
	/*assert (parent);*/
	m_parent_surf = parent;
}

/// Returns the Widget's parent surface
const Image Widget::get_parent() const
{
	return m_parent_surf;
}

Widget::WidgetList Widget::get_children() const
{
	WidgetList ret;

	for (WidgetList::const_iterator i = m_children.begin();
		i != m_children.end(); ++i) {
			WidgetList grandchildren((*i)->get_children());
			ret.splice(ret.begin(), grandchildren);
	}
	
	ret.insert(ret.end(), m_children.begin(), m_children.end());
	
	return ret;
}


const SDL_Rect& Widget::get_area() const
{
	return m_area;
}

/// Moves the widget to another location
/**
 ** @param x The x coordinate
 ** @param y The y coordinate
 **/
bool Widget::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;

	return true;
}

/// Resizes m_surface
bool Widget::resize(const int w, const int h)
{
	m_area.w = w;
	m_area.h = h;

	Image tmp (SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
			w, h, 32, rmask, gmask, bmask, amask));
	
	if (!tmp) {
		std::cerr << "Widget::resize(): CreateRGBSurface() failed: "
			<< SDL_GetError() << std::endl;
		return false;
	}

	m_surface = tmp;

	return true;
}


/// Blits the widget onto m_parent_surf
bool Widget::blit()
{
	assert (m_surface && m_parent_surf);
	if (SDL_BlitSurface(m_surface.get(), 0, m_parent_surf.get(), &m_area) < 0) {
		std::cerr <<  "Widget::blit(): SDL_BlitSurface error: \n"
			<< SDL_GetError() << std::endl;
		return false;
	}

	return true;
}


/// Default event handler
bool Widget::handle_event(const SDL_Event& event)
{
	return false;
}
