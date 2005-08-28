/** \file smartptr.cpp
 ** \brief Contains SmartPtr<TTF_Font_Holder> specialized template class source code
 **/

#include "smartptr.h"

/// Constructor
/**
 ** @param prep Pointer to use
 **/
template<>
SmartPtr<TTF_Font_Holder>::SmartPtr(TTF_Font* prep)
	: m_rep(prep), m_pcount(new int(1))
{
}


/// Copy-constructor
template<>
SmartPtr<TTF_Font_Holder>::SmartPtr(const SmartPtr<TTF_Font_Holder>& rhs)
	: m_rep(rhs.m_rep), m_pcount(rhs.m_pcount)
{
	(*m_pcount)++;
}


/// Destructor
template<>
SmartPtr<TTF_Font_Holder>::~SmartPtr()
{
	decrease_count();
}


/// Assignment from SmartPtr
template<>
SmartPtr<TTF_Font_Holder>& SmartPtr<TTF_Font_Holder>::operator=
	(const SmartPtr<TTF_Font_Holder>& rhs)
{
	if (m_rep == rhs.m_rep)
		return *this;
	
	// Delete data if we're overwriting the last handle
	decrease_count();
	
	m_rep = rhs.m_rep;
	m_pcount = rhs.m_pcount;
	
	(*m_pcount)++;
	return *this;
}


/// Assignment from TTF_Font_Holder*
template<>
SmartPtr<TTF_Font_Holder>& SmartPtr<TTF_Font_Holder>::operator=
	(TTF_Font* rhs)
{
	if (m_rep == rhs)
		return *this;

	decrease_count();

	m_rep = rhs;
	m_pcount = new int(1);
	return *this;
}


/// Returns the pointer
template<>
TTF_Font* SmartPtr<TTF_Font_Holder>::get() const
{
	return m_rep;
}


// Operator->
template<>
TTF_Font* SmartPtr<TTF_Font_Holder>::operator->() const
{
	return m_rep;
}


// Operator*
template<>
TTF_Font& SmartPtr<TTF_Font_Holder>::operator*() const
{
	return *m_rep;
}

// Operator==
template<>
bool SmartPtr<TTF_Font_Holder>::operator==(const TTF_Font* rhs) const
{
     return m_rep == rhs;
}

template<>
bool SmartPtr<TTF_Font_Holder>::operator==(const SmartPtr<TTF_Font_Holder>& rhs)
	const
{
     return rhs == m_rep;
}     


/// Allow class to be used in if statements
template<>
SmartPtr<TTF_Font_Holder>::operator bool() const
{
	return (m_rep)?true:false;
}


/// Clean up - decreases the reference count and deletes if it's at zero
template<>
void SmartPtr<TTF_Font_Holder>::decrease_count()
{
	if (--(*m_pcount) == 0) {
		if (!m_rep) return;
		
		TTF_CloseFont(m_rep);
		
		delete m_pcount; 
		m_pcount = 0;
	}
}
