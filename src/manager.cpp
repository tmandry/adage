#include <vector>

#include "SDL.h"

#include "manager.h"
#include "widget.h"

WidgetManager::WidgetManager()
{
}

WidgetManager::~WidgetManager()
{
}

WidgetManager* WidgetManager::get_ptr()
{
	static WidgetManager widget_manager;
	
	return &widget_manager;
}

void WidgetManager::register_widget(Widget* ptr)
{
	m_widgets.push_front(ptr);
}

void WidgetManager::unregister_widget(Widget* ptr)
{
	// Search for the pointer in the vector and erase it
	std::list<Widget*>::iterator i;
	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		if (*i == ptr) m_widgets.erase (i);
}

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

bool WidgetManager::handle_event(SDL_Event& event)
{
	bool status = false;
	std::list<Widget*>::iterator i;

	for (i = m_widgets.begin(); i != m_widgets.end(); i++)
		status |= (*i)->handle_event(event);
	
	return status;
}
