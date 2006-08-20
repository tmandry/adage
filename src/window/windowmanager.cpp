/** \file windowmanager.cpp
 ** \brief Contains source code for WindowManager class
 **/

#include "config.h"

#include <list>
#include <cassert>

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

// Default value of the pointer
WindowManager* WindowManager::m_ptr = NULL;

/// Returns a pointer to the singleton, allocating the singleton first if need be
WindowManager* WindowManager::get_ptr()
{
	if (m_ptr == NULL) m_ptr = new WindowManager;
	return m_ptr;
}

/// Reigsters a newly-created Window with the WindowManager
/**
 ** @param ptr A pointer to the new Window
 **/
void WindowManager::register_window(Window* ptr)
{
	assert (ptr);
	m_windows.push_front(ptr);
}

/// Unregisters an old Window with the WindowManager and deletes the singleton if there aren't any Windows left
/**
 ** @param ptr A pointer to the old Window
 **/
void WindowManager::unregister_window(Window* ptr)
{
	assert (ptr); assert (!m_windows.empty());
	m_windows.remove(ptr);
	
	if (m_windows.empty()) delete m_ptr;
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
