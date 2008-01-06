#ifndef ENTITYLIST_H_
#define ENTITYLIST_H_

#include "detail/StaticCastIterator.h"

template<class Type, class Container = std::vector<Entity*> >
class EntityList
{
public:
	typedef Detail::StaticCastIterator<Type*, typename Container::iterator > iterator;
	typedef Detail::StaticCastIterator<const Type*, typename Container::const_iterator > const_iterator;
	
	typedef unsigned int size_type;
	
	EntityList(Container& vec): mVec(vec) {}
	~EntityList() {}
	
	iterator begin() { return iterator(mVec.begin()); }
	const_iterator begin() const { return const_iterator(mVec.begin()); }
	iterator end() { return iterator(mVec.end()); }
	const_iterator end() const { return const_iterator(mVec.end()); }
	
	Type* front() { return static_cast<Type*>(mVec.front()); }
	const Type& front() const { return static_cast<const Type*>(mVec.front()); }
	Type* back() { return static_cast<Type*>(mVec.back()); }
	const Type& back() const { return static_cast<const Type*>(mVec.back()); }
	
	Type* operator[](size_type idx) { return static_cast<Type*>(mVec[idx]); }
	const Type* operator[](size_type idx) const { return static_cast<const Type*>(mVec[idx]); }
	
	size_type size() const { return mVec.size(); }
	bool empty() const { return mVec.empty(); }
	
private:
	Container& mVec;
};

template<class Type>
class ConstEntityList : public EntityList<Type, const std::vector<Entity*> >
{
public:
	ConstEntityList(const std::vector<Entity*>& vec) : EntityList<Type, const std::vector<Entity*> >(vec) {}
};

#endif /*ENTITYLIST_H_*/
