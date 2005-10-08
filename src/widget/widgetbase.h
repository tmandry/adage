/** \file widgetbase.h
 ** \brief Contains the Widget base class definition
 **/

#ifndef WIDGET_BASE_H
#define WIDGET_BASE_H

#include <list>

#include "SDL.h"

#include "util/image.h"

/// Widget base class 
class Widget {
public:
	// Constructor
	Widget();
	
	// Destructor
	virtual ~Widget();
	
	// Members
	void set_parent(const Image& parent);
	std::list<Widget*> get_children();

	virtual const SDL_Rect& get_area() const;

	virtual bool move(const int x, const int y);
	virtual bool resize(const int w, const int h);
	
	/// Draws the Widget to m_surface
	virtual bool draw()=0;
	virtual bool blit();

	virtual bool handle_event(const SDL_Event& event);
	
protected:
	const Image get_parent() const;

	/// Adds to the list of the widget's children
	inline void Widget::add_child(Widget* child)
	{
		m_children.push_back(child);
	}

	/// Clears the list of children
	inline void Widget::clear_children()
	{
		m_children.clear();
	}

	/// Defines the location and size of the Widget on m_parent_surf
	SDL_Rect m_area;
	/// The Widget's own surface
	Image m_surface;
	/// Whether or not the widget has changed since last draw()
	bool m_changed;

private:
	/// The Widget's parent surface
	Image m_parent_surf;

	/// A list of the widget's children
	std::list<Widget*> m_children;
};	

#endif // WIDGET_BASE_H
