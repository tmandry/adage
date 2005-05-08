#ifndef MANAGER_H
#define MANAGER_H

#include <list>

#include "SDL.h"

#include "widget.h"

class WidgetManager {
public:
	WidgetManager();
	
	~WidgetManager();
	
	// Get pointer to singleton
	static WidgetManager* get_ptr();
	
	// Register a new widget
	void register_widget(Widget* ptr);
	
	// Unregister an old widget
	void unregister_widget(Widget* ptr);
	
	// Draw all widgets to the screen
	bool draw();
	
	// Send an event to all widgets
	bool handle_event(SDL_Event& event);
	
private:
	// List of all widgets
	std::list<Widget*> m_widgets;
};

#endif // MANAGER_H
