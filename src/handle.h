#ifndef HANDLE_H 
#define HANDLE_H

//#include <string>
//#include <iostream>

template <class T>
class basic_handle {
public:
	typedef void (*delete_function)(T*);
	
	basic_handle(T* prep = 0, delete_function = 0);
	basic_handle(const basic_handle&);
	virtual ~basic_handle();

	basic_handle& operator=(const basic_handle&);
	basic_handle& operator=(T*);

	T* get() const;
	T* operator->() const;
	T& operator*() const;
	virtual bool operator==(const T* rhs) const;
	virtual bool operator==(const basic_handle<T>& rhs) const;

	operator bool() const;
	
	void set_deleter(delete_function);
//	void set_name(const std::string& new_name) { name = new_name; }
	
protected:
	T* rep;
	int* pcount;
	delete_function pdf; // Pointer to Delete Function
	
	void decrease_count();

//	std::string name;
};


// Constructor
template <class T>
basic_handle<T>::basic_handle(T* prep, delete_function del_func)
	: rep(prep), pcount(new int(1)), pdf(del_func)
{
}


// Copy-constructor
template <class T>
basic_handle<T>::basic_handle(const basic_handle& rhs)
	: rep(rhs.rep), pcount(rhs.pcount), pdf(rhs.pdf)
{
	(*pcount)++;
}


// Destructor
template <class T>
basic_handle<T>::~basic_handle()
{
//	std::cout << "Decreasing count of " << name << "\n";
	decrease_count();
}


// Operator=
template <class T>
basic_handle<T>& basic_handle<T>::operator=(const basic_handle<T>& rhs)
{
//	std::cout << "Assigning from " << rhs.name << " to " << name << "\n";
	if (rep == rhs.rep)
		return *this;
	
	// Delete data if we're overwriting the last handle
	decrease_count();
	
	rep = rhs.rep;
	pcount = rhs.pcount;
	pdf = rhs.pdf;
	
	(*pcount)++;
 return *this;
}


// Assignment from T*
template <class T>
basic_handle<T>& basic_handle<T>::operator=(T* rhs)
{
//	std::cout << "Assigning from raw ptr to " << name << "\n";
	if (rep == rhs)
		return *this;

	decrease_count();

	rep = rhs;
	pcount = new int(1);
 return *this;
}


// Return the pointer
template <class T>
T* basic_handle<T>::get() const
{
	return rep;
}


// Operator->
template <class T>
T* basic_handle<T>::operator->() const
{
	return rep;
}


// Operator*
template <class T>
T& basic_handle<T>::operator*() const
{
	return *rep;
}

// Operator==
template <class T>
bool basic_handle<T>::operator==(const T* rhs) const
{
     return rep == rhs;
}

template <class T>
bool basic_handle<T>::operator==(const basic_handle<T>& rhs) const
{
     return rhs == rep;
}     


// Allow if(basic_handle<blabla>) ...
template <class T>
basic_handle<T>::operator bool() const
{
	if (rep) {
		return true;
	} else {
		return false;
	}
}


// Set deleting function
template <class T>
void basic_handle<T>::set_deleter(delete_function del_func)
{
	pdf = del_func;
}


// Clean up
template <class T>
void basic_handle<T>::decrease_count()
{
	if (--(*pcount) == 0) {
//		std::cout << "Deleting object " << name << "\n";
		
		if (!rep) return;
		
		if (pdf) {
			pdf(rep);
		} else {
			delete rep; 
			rep = 0;
		}
		
		delete pcount; 
		pcount = 0;
	}
}

#endif // HANDLE_H 

