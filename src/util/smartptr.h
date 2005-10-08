/** \file smartptr.h
 ** \brief Contains the SmartPtr class definition
 **/

#ifndef SMARTPTR_H
#define SMARTPTR_H

#include "SDL_ttf.h"

/// Smart pointer class.
/** Counts references and deletes the pointer when there aren't any more
 ** references. */
template <class T>
class SmartPtr {
public:
	/// Prototype for optional function that will be called instead of deleting
	typedef void (*delete_function)(T*);

	// Constructor
	SmartPtr(T* prep = 0, delete_function = 0);

	// Copying and assignment
	SmartPtr(const SmartPtr&);
	SmartPtr& operator=(const SmartPtr&);
	SmartPtr& operator=(T*);

	virtual ~SmartPtr();

	T* get() const;
	T* operator->() const;
	T& operator*() const;
	virtual bool operator==(const T* rhs) const;
	virtual bool operator==(const SmartPtr<T>& rhs) const;

	operator bool() const;
	
	void set_deleter(delete_function);
	
protected:
	/// The actual pointer
	T* m_rep;
	/// Pointer to the reference count
	int* m_pcount;
	/// Pointer to the delete function
	delete_function m_pdf;
	
	void decrease_count();
};


/// Constructor
/**
 ** @param prep Pointer to use
 ** @param del_func Pointer to the delete function
 **/
template <class T>
SmartPtr<T>::SmartPtr(T* prep, delete_function del_func)
	: m_rep(prep), m_pcount(new int(1)), m_pdf(del_func)
{
}


/// Copy-constructor
template <class T>
SmartPtr<T>::SmartPtr(const SmartPtr& rhs)
	: m_rep(rhs.m_rep), m_pcount(rhs.m_pcount), m_pdf(rhs.m_pdf)
{
	(*m_pcount)++;
}


/// Destructor
template <class T>
SmartPtr<T>::~SmartPtr()
{
	decrease_count();
}


/// Assignment from SmartPtr
template <class T>
SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& rhs)
{
	if (m_rep == rhs.m_rep)
		return *this;
	
	// Delete data if we're overwriting the last handle
	decrease_count();
	
	m_rep = rhs.m_rep;
	m_pcount = rhs.m_pcount;
	m_pdf = rhs.m_pdf;
	
	(*m_pcount)++;
	return *this;
}


/// Assignment from T*
template <class T>
SmartPtr<T>& SmartPtr<T>::operator=(T* rhs)
{
	if (m_rep == rhs)
		return *this;

	decrease_count();

	m_rep = rhs;
	m_pcount = new int(1);
	return *this;
}


/// Returns the pointer
template <class T>
T* SmartPtr<T>::get() const
{
	return m_rep;
}


// Operator->
template <class T>
T* SmartPtr<T>::operator->() const
{
	return m_rep;
}


// Operator*
template <class T>
T& SmartPtr<T>::operator*() const
{
	return *m_rep;
}

// Operator==
template <class T>
bool SmartPtr<T>::operator==(const T* rhs) const
{
     return m_rep == rhs;
}

template <class T>
bool SmartPtr<T>::operator==(const SmartPtr<T>& rhs) const
{
     return rhs == m_rep;
}     


/// Allow class to be used in if statements
template <class T>
SmartPtr<T>::operator bool() const
{
	if (m_rep) {
		return true;
	} else {
		return false;
	}
}


/// Set delete function
/**
 ** @param del_func Pointer to delete function
 **/
template <class T>
void SmartPtr<T>::set_deleter(delete_function del_func)
{
	m_pdf = del_func;
}


/// Clean up - decreases the reference count and deletes if it's at zero
template <class T>
void SmartPtr<T>::decrease_count()
{
	if (--(*m_pcount) == 0) {
		if (!m_rep) return;
		
		if (m_pdf) {
			m_pdf(m_rep);
		} else {
			delete m_rep;
			m_rep = 0;
		}
		
		delete m_pcount; 
		m_pcount = 0;
	}
}

/// A little hack to use in place of TTF_Font, which isn't defined in the header
class TTF_Font_Holder;

/// Specialized class for TTF_Fonts
template <>
class SmartPtr<TTF_Font_Holder> {
public:
	// Constructor
	SmartPtr(TTF_Font* prep = 0);

	// Copying and assignment
	SmartPtr(const SmartPtr<TTF_Font_Holder>&);
	SmartPtr& operator=(const SmartPtr<TTF_Font_Holder>&);
	SmartPtr& operator=(TTF_Font*);

	virtual ~SmartPtr();

	TTF_Font* get() const;
	TTF_Font* operator->() const;
	TTF_Font& operator*() const;
	virtual bool operator==(const TTF_Font* rhs) const;
	virtual bool operator==(const SmartPtr<TTF_Font_Holder>& rhs) const;

	operator bool() const;
	
protected:
	/// The actual pointer
	TTF_Font* m_rep;
	/// Pointer to the reference count
	int* m_pcount;
	
	void decrease_count();
};

#endif // SMARTPTR_H
