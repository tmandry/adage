/** \file smartptr.cpp
 ** \brief Contains SmartPtr<TTF_Font_Holder> specialized template class source code
 **/

#include <cassert>

#include "smartptr.h"
#include "SDL_ttf.h"

/// Constructor
/**
 ** @param prep Pointer to use
 **/
SmartPtr<TTF_Font_Holder>::SmartPtr(TTF_Font* prep)
	: m_rep(prep), m_pcount(new int(1))
{
}


/// Copy-constructor
SmartPtr<TTF_Font_Holder>::SmartPtr(const SmartPtr<TTF_Font_Holder>& rhs)
	: m_rep(rhs.m_rep), m_pcount(rhs.m_pcount)
{
	assert (m_pcount);
	(*m_pcount)++;
}


/// Destructor
SmartPtr<TTF_Font_Holder>::~SmartPtr()
{
	decrease_count();
}


/// Releases the assigned pointer, results in a pointer to NULL
void SmartPtr<TTF_Font_Holder>::release()
{
	*this = static_cast<TTF_Font*>(NULL);
}


/// Assignment from SmartPtr
SmartPtr<TTF_Font_Holder>& SmartPtr<TTF_Font_Holder>::operator=
	(const SmartPtr<TTF_Font_Holder>& rhs)
{
	// Delete data if we're overwriting the last handle
	decrease_count();
	
	m_rep = rhs.m_rep;
	m_pcount = rhs.m_pcount;
	
	assert (m_pcount);
	(*m_pcount)++;
	return *this;
}


/// Assignment from TTF_Font_Holder*
SmartPtr<TTF_Font_Holder>& SmartPtr<TTF_Font_Holder>::operator=
	(TTF_Font* rhs)
{
	assert (m_rep != rhs);
	
	decrease_count();

	m_rep = rhs;
	m_pcount = new int(1);
	return *this;
}


/// Returns the pointer
TTF_Font* SmartPtr<TTF_Font_Holder>::get() const
{
	return m_rep;
}


// Operator->
TTF_Font* SmartPtr<TTF_Font_Holder>::operator->() const
{
	return m_rep;
}


// Operator*
TTF_Font& SmartPtr<TTF_Font_Holder>::operator*() const
{
	return *m_rep;
}

// Operator==
bool SmartPtr<TTF_Font_Holder>::operator==(const TTF_Font* rhs) const
{
     return m_rep == rhs;
}

bool SmartPtr<TTF_Font_Holder>::operator==(const SmartPtr<TTF_Font_Holder>& rhs)
	const
{
     return rhs == m_rep;
}     


/// Allow class to be used in if statements
SmartPtr<TTF_Font_Holder>::operator bool() const
{
	return (m_rep)?true:false;
}


/// Clean up - decreases the reference count and deletes if it's at zero
void SmartPtr<TTF_Font_Holder>::decrease_count()
{
	assert (m_pcount);
	if (--(*m_pcount) == 0) {
		delete m_pcount; 
		m_pcount = 0;
		
		if (m_rep)
			TTF_CloseFont(m_rep);
	}
}
