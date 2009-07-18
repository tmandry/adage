#ifndef ENTITYLIST_H_
#define ENTITYLIST_H_

#include <QVector>
#include "Pointer.h"
#include "detail/StaticCastIterator.h"

namespace Detail {

template<class Container> class Wrapper {
public:
	Wrapper() {}
	Wrapper(Container c): mVec(c) {}
	Wrapper(const Wrapper<Container>& w): mVec(w.mVec) {}
	Container mVec;
};

} //namespace Detail

//EntityList container adapter that holds a REFERENCE to a vector of Pointer<Entity>s, acts like a container of Type* (by casting)
template<class Type, class Container = QVector<Pointer<Entity> > >
class EntityList : public Detail::Wrapper<Container>
{
private:
	typedef Detail::Wrapper<Container> Vec;
public:
	typedef Detail::StaticCastIterator<Pointer<Type>, typename Container::iterator > iterator;
	typedef Detail::StaticCastIterator<const Pointer<Type>, typename Container::const_iterator > const_iterator;

	typedef unsigned int size_type;

	EntityList(Container& vec): Vec(vec) {}
	EntityList(const Container& vec): Vec(vec) {}
	EntityList(): Vec() {} //empty constructor
	EntityList(const EntityList<Type, Container>& list): Vec(list.vec()) {}

	template <class From>
	EntityList(const EntityList<From, Container>& list)
		: Vec(list)
	{
		//check that From pointers can be assigned to Type pointers
		Type* t; From* f; t = f;
	}
	~EntityList() {}

	EntityList& operator=(const EntityList& rhs) { vec() = rhs.vec(); return *this; }

	iterator begin() { return iterator(vec().begin()); }
	const_iterator begin() const { return const_iterator(vec().begin()); }
	iterator end() { return iterator(vec().end()); }
	const_iterator end() const { return const_iterator(vec().end()); }

	Pointer<Type> front() { return Pointer<Type>::staticPointerCast(vec().front()); }
	const Type& front() const { return Pointer<const Type>::staticPointerCast(vec().front()); }
	Pointer<Type> back() { return Pointer<Type>::staticPointerCast(vec().back()); }
	const Type& back() const { return Pointer<const Type>::staticPointerCast(vec().back()); }

	Pointer<Type> operator[](size_type idx) { return Pointer<Type>::staticPointerCast(vec()[idx]); }
	const Pointer<Type> operator[](size_type idx) const { return Pointer<const Type>::staticPointerCast(vec()[idx]); }

	size_type size() const { return vec().size(); }
	bool empty() const { return vec().empty(); }

	iterator erase(iterator loc) { return typename Container::iterator(vec().erase(loc)); }

private:
	Container& vec() { return Vec::mVec; }
	const Container& vec() const { return Vec::mVec; }
	template<class Type2, class From, class Container2>
	friend void EntityListCopyHelper(EntityList<Type, Container>& to, const EntityList<From, Container>& from);
};

#endif /*ENTITYLIST_H_*/
