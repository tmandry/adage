/** \file widget.h
 ** \brief Contains the Widget base class definition
 **/

#ifndef WIDGET_H
#define WIDGET_H

#include "SDL.h"

#include "utils/image.h"

/// Widget base class 
class Widget {
public:
	// Constructor
	Widget();
	
	// Destructor
	virtual ~Widget();
	
	// Members
	void set_parent(const Image& parent);
	
	virtual bool move(const int x, const int y);
	virtual bool resize(const int w, const int h);
	
	/// Draws the Widget to m_surface
	virtual bool draw()=0;

	virtual bool blit();
	
	virtual bool handle_event(const SDL_Event& event);
	
protected:
	const Image get_parent() const;

	/// Defines the location and size of the Widget on m_parent_surf
	SDL_Rect m_area;
	/// The Widget's own surface
	Image m_surface;
	/// Whether or not the widget has changed since last draw()
	bool m_changed;

private:
	/// The Widget's parent surface
	Image m_parent_surf;
};	

#endif // WIDGET_H
