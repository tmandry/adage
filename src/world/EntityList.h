#ifndef ENTITYLIST_H_
#define ENTITYLIST_H_

#include "Pointer.h"
#include "detail/StaticCastIterator.h"

//EntityList container adapter that holds a REFERENCE to a vector of Pointer<Entity>s, acts like a container of Type* (by casting)
template<class Type, class Container = std::vector<Pointer<Entity> > >
class EntityList
{
public:
	typedef Detail::StaticCastIterator<Pointer<Type>, typename Container::iterator > iterator;
	typedef Detail::StaticCastIterator<const Pointer<Type>, typename Container::const_iterator > const_iterator;

	typedef unsigned int size_type;

	EntityList(Container& vec): mVec(&vec) {}
	EntityList(): mVec(0) {}
	//EntityList(const Container& vec): mVec(Container(vec)) {}
	EntityList(const EntityList<Type, Container>& list): mVec(list.mVec) {}
	~EntityList() {}

	EntityList& operator=(const EntityList& rhs) { mVec = rhs.mVec; return *this; }

	iterator begin() { return iterator(mVec->begin()); }
	const_iterator begin() const { return const_iterator(mVec->begin()); }
	iterator end() { return iterator(mVec->end()); }
	const_iterator end() const { return const_iterator(mVec->end()); }

	Pointer<Type> front() { return Pointer<Type>::staticPointerCast(mVec->front()); }
	const Type& front() const { return Pointer<const Type>::staticPointerCast(mVec->front()); }
	Pointer<Type> back() { return Pointer<Type>::staticPointerCast(mVec->back()); }
	const Type& back() const { return Pointer<const Type>::staticPointerCast(mVec->back()); }

	Pointer<Type> operator[](size_type idx) { return Pointer<Type>::staticPointerCast((*mVec)[idx]); }
	const Pointer<Type> operator[](size_type idx) const { return Pointer<const Type>::staticPointerCast((*mVec)[idx]); }

	size_type size() const { return mVec->size(); }
	bool empty() const { return mVec->empty(); }

	iterator erase(iterator loc) { return typename Container::iterator(mVec->erase(loc)); }

protected:
	Container* mVec;
};

//ConstEntityList - like EntityList, but holds a const reference to the vector
template<class Type>
class ConstEntityList : public EntityList<Type, const std::vector<Pointer<Entity> > >
{
public:
	//ConstEntityList(const std::vector<Pointer<Entity>>& vec) : EntityList<Type, const std::vector<Pointer<Entity>> >(vec) {}
	ConstEntityList(const EntityList<Type, const std::vector<Pointer<Entity> > >& vec) : EntityList<Type, const std::vector<Pointer<Entity> > >(vec) {}
	ConstEntityList() {}

	ConstEntityList& operator=(const ConstEntityList& rhs) { EntityList<Type, const std::vector<Pointer<Entity> > >::mVec = rhs.mVec; return *this; }
};

#endif /*ENTITYLIST_H_*/
