/** \file windowmanager.cpp
 ** \brief Contains source code for WindowManager class
 **/

#include <list>

#include "SDL.h"

#include "windowmanager.h"
#include "window.h"

/// Constructor
WindowManager::WindowManager()
{
}

/// Destructor
WindowManager::~WindowManager()
{
}

/// Returns a pointer to the singleton
WindowManager* WindowManager::get_ptr()
{
	static WindowManager window_manager;
	
	return &window_manager;
}

/// Reigsters a newly-created Window with the WindowManager
/**
 ** @param ptr A pointer to the new Window
 **/
void WindowManager::register_window(Window* ptr)
{
	m_windows.push_front(ptr);
}

/// Unregisters an old Window with the WindowManager
/**
 ** @param ptr A pointer to the old Window
 **/
void WindowManager::unregister_window(Window* ptr)
{
	m_windows.remove(ptr);
}

/// Draws all windows to the screen
void WindowManager::draw()
{
	std::list<Window*>::iterator i;
	
	for (i = m_windows.begin(); i != m_windows.end(); i++)
		(*i)->draw();

	for (i = m_windows.begin(); i != m_windows.end(); i++)
		(*i)->blit();
}

// TODO: Optimize this!
/// Sends an event to every window
/**
 ** @param event The event to send
 **/
bool WindowManager::handle_event(SDL_Event& event)
{
	std::list<Window*>::iterator i;

	for (i = m_windows.begin(); i != m_windows.end(); i++)
		if ((*i)->handle_event(event)) return true;
	
	return false;
}
