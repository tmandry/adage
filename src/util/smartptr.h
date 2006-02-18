/** \file smartptr.h
 ** \brief Contains the SmartPtr class definition
 **/

#ifndef SMARTPTR_H
#define SMARTPTR_H

#include <iostream>
#include <cassert>

#include "SDL_ttf.h"

/// Smart pointer class.
/** Counts references and deletes the pointer when there aren't any more
 ** references. */
template <class T, class D = T>
class SmartPtr {
public:
	/// Prototype for optional function that will be called instead of deleting
	typedef void (*delete_function)(D*);

	// Constructor
	SmartPtr(T* prep = 0, delete_function = 0);

	// Copying and assignment
	template <class S>
	SmartPtr(const SmartPtr<S>& rhs);
	SmartPtr& operator=(const SmartPtr&);
	SmartPtr& operator=(T*);

	virtual ~SmartPtr();

	inline void release();

	T* get() const;
	T* operator->() const;
	T& operator*() const;
	virtual bool operator==(const T* rhs) const;
	virtual bool operator==(const SmartPtr<T,D>& rhs) const;

	operator bool() const;
	
	void set_deleter(delete_function);
	
protected:
	template <class S>
	friend class SmartPtr;

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
template <class T, class D>
SmartPtr<T,D>::SmartPtr(T* prep, typename SmartPtr<T,D>::delete_function del_func)
	: m_rep(prep), m_pcount(new int(1)), m_pdf(del_func)
{
}


/// Copy constructor
template <class T, class D>
template <class S>
SmartPtr<T,D>::SmartPtr(const SmartPtr<S>& rhs)
	: m_rep(rhs.m_rep), m_pcount(rhs.m_pcount), m_pdf(static_cast<delete_function>(rhs.m_pdf))
{
	assert (m_pcount);
	++(*m_pcount);
}


/// Destructor
template <class T, class D>
SmartPtr<T,D>::~SmartPtr()
{
	decrease_count();
}


/// Releases the assigned pointer, results in a pointer to NULL
template <class T, class D>
void SmartPtr<T,D>::release()
{
	*this = static_cast<T*>(NULL);
}


/// Assignment from SmartPtr
template <class T, class D>
SmartPtr<T,D>& SmartPtr<T,D>::operator=(const SmartPtr<T,D>& rhs)
{
	// Delete data if we're overwriting the last handle
	decrease_count();
	
	m_rep = rhs.m_rep;
	m_pcount = rhs.m_pcount;
	m_pdf = rhs.m_pdf;
	
	assert (m_pcount);
	(*m_pcount)++;
	return *this;
}


/// Assignment from T*
template <class T, class D>
SmartPtr<T,D>& SmartPtr<T,D>::operator=(T* rhs)
{
	assert (m_rep != rhs);

	decrease_count();

	m_rep = rhs;
	m_pcount = new int(1);
	return *this;
}


/// Returns the pointer
template <class T, class D>
T* SmartPtr<T,D>::get() const
{
	return m_rep;
}


// Operator->
template <class T, class D>
T* SmartPtr<T,D>::operator->() const
{
	return m_rep;
}


// Operator*
template <class T, class D>
T& SmartPtr<T,D>::operator*() const
{
	return *m_rep;
}

// Operator==
template <class T, class D>
bool SmartPtr<T,D>::operator==(const T* rhs) const
{
     return m_rep == rhs;
}

template <class T, class D>
bool SmartPtr<T,D>::operator==(const SmartPtr<T,D>& rhs) const
{
     return rhs == m_rep;
}     


/// Allow class to be used in if statements
template <class T, class D>
SmartPtr<T,D>::operator bool() const
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
template <class T, class D>
void SmartPtr<T,D>::set_deleter(delete_function del_func)
{
	m_pdf = del_func;
}


/// Clean up - decreases the reference count and deletes if it's at zero
template <class T, class D>
void SmartPtr<T,D>::decrease_count()
{
	assert (m_pcount);
	if (--(*m_pcount) == 0) {
		delete m_pcount; 
		m_pcount = 0;
		
		if (!m_rep) return;
			
		/*std::cout << "killing SmartPtr at " << m_rep << std::endl;*/
		if (m_pdf) {
			m_pdf(m_rep);
		} else {
			delete m_rep;
			m_rep = 0;
		}
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

	inline void release();

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
