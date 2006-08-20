/** \file windowmanager.h
 ** \brief Defines the WindowManager class
 **/

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "config.h"

#include <list>

#include "SDL.h"

#include "window.h"

/// A singleton class which keeps track of all windows and dispatches messages to them
class WindowManager {
public:
	WindowManager();

	~WindowManager();

	static WindowManager* get_ptr();

	void register_window(Window* ptr);
	void unregister_window(Window* ptr);

	void draw();

	bool handle_event(SDL_Event& event);

private:
	/// Pointer to the WindowManager instance
	static WindowManager* m_ptr;
	
	/// List of all windows
	std::list<Window*> m_windows;
};

#endif // WINDOWMANAGER_H
