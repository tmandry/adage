#include <iostream>

#include "SDL.h"

#include "rgbamask.h"
#include "widget.h"
#include "manager.h"

Widget::Widget()
{
	// Register with the singleton WidgetManager class
	WidgetManager::get_ptr()->register_widget(this);
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

Widget::~Widget()
{
	// Unregister with WidgetManager
	WidgetManager::get_ptr()->unregister_widget(this);
}


void Widget::set_parent(const Image& parent)
{
	m_parent_surf = parent;
}

const Image Widget::get_parent() const
{
	return m_parent_surf;
}


bool Widget::move(const int x, const int y)
{
	m_area.x = x;
	m_area.y = y;

	return true;
}

bool Widget::resize(const int w, const int h)
{
	m_area.w = w;
	m_area.h = h;

	Image tmp = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
			w, h, 32, rmask, gmask, bmask, amask);
	
	if (!tmp) {
		std::cout << "Widget::resize(): CreateRGBSurface() failed: "
			<< SDL_GetError() << std::endl;
		return false;
	}

	m_surface = tmp;

	return true;
}


bool Widget::blit()
{
	if (SDL_BlitSurface(m_surface.get(), 0, m_parent_surf.get(), &m_area) < 0) {
		std::cout <<  "Widget::blit(): SDL_BlitSurface error: \n"
			<< SDL_GetError() << std::endl;
		return false;
	}

	return true;
}


bool Widget::handle_event(const SDL_Event& event)
{
	return false;
}
