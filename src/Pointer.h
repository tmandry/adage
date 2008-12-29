#ifndef POINTER_H_
#define POINTER_H_

template <class T>
class Pointer;

template <class T>
class Pointer
{
public:
	Pointer() : mPtr(0), mPtrRef(0) {}
	explicit Pointer(T* const p) : mPtr(new T*(p)), mPtrRef(new int(1)) {}
	Pointer(const Pointer<T>& p) : mPtr(p.mPtr), mPtrRef(p.mPtrRef) { if (mPtr) ++(*mPtrRef); }
	template <class U>
	Pointer(const Pointer<U>& p) : mPtrRef(p.mPtrRef) { mPtr = (T**)p.mPtr; if (mPtr) { T* temp(*p.mPtr); ++(*mPtrRef); } }

	template <class U>
	static Pointer<T> staticPointerCast(const Pointer<U>& p) { Pointer<T> ret; ret.mPtr = (T**)p.mPtr; ret.mPtrRef = p.mPtrRef; if (p.mPtr) { T* temp(static_cast<T*>(*p.mPtr)); ++(*ret.mPtrRef); } return ret; }

	void release() { if (!mPtr) return; --(*mPtrRef); if (*mPtrRef == 0) { delete mPtr; delete mPtrRef; } mPtr = 0; mPtrRef = 0; }
	~Pointer() { release(); }

	/*Pointer<T>& operator=(T* rhs) { release(); if (rhs) { mPtr = new T*(rhs); mPtrRef = new int(1); } return *this; }*/
	Pointer<T>& operator=(const Pointer<T>& rhs) { release(); mPtr = rhs.mPtr; mPtrRef = rhs.mPtrRef; if (mPtr) ++(*mPtrRef); return *this; }

	/*operator T*() { return *mPtr; }*/
	T* operator*() const { return *mPtr; }
	T* operator->() const { return *mPtr; }
	//T const* operator*() const { return *mPtr; }
	//T const* operator->() const { return *mPtr; }

	operator bool() const { return (mPtr && (*mPtr != 0)); }

	//implicitly static
	//operator Pointer<T>*() { return this; } //implicitly becomes a pointer to itself so that you can delete the class as if it were a real pointer
	void operator delete(void* p) { Pointer<T>* P (static_cast<Pointer<T>*>(p)); P->free(); }
	void free() const { delete (*mPtr); (*mPtr) = 0; }

	T* pointer() const { return *mPtr; }
	//T const* pointer() const { return *mPtr; }

private:
	template <class C>
	friend bool operator<(const Pointer<C>& lhs, const Pointer<C>& rhs);
	template <class C>
	friend bool operator>(const Pointer<C>& lhs, const Pointer<C>& rhs);
	template <class C>
	friend bool operator==(const Pointer<C>& lhs, const Pointer<C>& rhs);
	template <class C>
	friend bool operator!=(const Pointer<C>& lhs, const Pointer<C>& rhs);
	template <class C>
	friend bool operator<=(const Pointer<C>& lhs, const Pointer<C>& rhs);
	template <class C>
	friend bool operator>=(const Pointer<C>& lhs, const Pointer<C>& rhs);


	template <class C>
	friend class Pointer;

	T** mPtr;
	int* mPtrRef;
};

template <class T>
inline bool operator<(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr < rhs.mPtr);
}

template <class T>
inline bool operator>(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr > rhs.mPtr);
}

template <class T>
inline bool operator==(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr == rhs.mPtr);
}

template <class T>
inline bool operator!=(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr != rhs.mPtr);
}

template <class T>
inline bool operator<=(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr <= rhs.mPtr);
}

template <class T>
inline bool operator>=(const Pointer<T>& lhs, const Pointer<T>& rhs)
{
	return (lhs.mPtr >= rhs.mPtr);
}

#endif /* POINTER_H_ */
