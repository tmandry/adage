#ifndef WIDGET_H
#define WIDGET_H

#include "SDL.h"

#include "image.h"

// Widget barebones class
class Widget {
public:
	Widget();
	
	virtual ~Widget();
	
	// Members
	void set_parent(const Image& parent);
	
	virtual bool move(const int x, const int y);
	virtual bool resize(const int w, const int h);
	
	virtual bool draw()=0;
	virtual bool blit();
	
	virtual bool handle_event(const SDL_Event& event);
	
protected:
	const Image get_parent() const;
	
	SDL_Rect m_area;
	Image m_surface;

private:
	Image m_parent_surf;
};	

#endif // WIDGET_H
