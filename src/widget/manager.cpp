/** \file manager.cpp
 ** \brief Contains the WidgetManager class source code
 **/

#include <vector>
#include <iostream>

#include "SDL.h"

#include "manager.h"
#include "widget.h"

/// Constructor
WidgetManager::WidgetManager()
{
}

/// Destructor
WidgetManager::~WidgetManager()
{
}

/// Returns a pointer to the singleton
WidgetManager* WidgetManager::get_ptr()
{
	static WidgetManager widget_manager;
	
	return &widget_manager;
}

/// Reigsters a newly-created Widget with the WidgetManager
/**
 ** @param ptr A pointer to the new Widget
 **/
void WidgetManager::register_widget(Widget* ptr)
{
	m_widgets.push_front(ptr);
}

/// Unregisters an old Widget with the WidgetManager
/**
 ** @param ptr A pointer to the old Widget
 **/
void WidgetManager::unregister_widget(Widget* ptr)
{
	// Search for the pointer in the vector and erase it
	std::list<Widget*>::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		if (*i == ptr) {
			m_widgets.erase (i);
			break;
		}
}

/// Draws all widgets to the screen
bool WidgetManager::draw()
{
	bool status = true;
	std::list<Widget*>::iterator i;
	
	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		status &= (*i)->draw();
	
	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		status &= (*i)->blit();
	
	return status;
}

// TODO: Optimize this!
/// Sends an event to every widget
/**
 ** @param event The event to send
 **/
bool WidgetManager::handle_event(SDL_Event& event)
{
	bool status = false;
	std::list<Widget*>::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		status |= (*i)->handle_event(event);
	
	return status;
}
