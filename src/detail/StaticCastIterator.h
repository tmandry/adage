#ifndef STATICCASTITERATOR_H_
#define STATICCASTITERATOR_H_

namespace Detail {

template<class Type, class Iterator>
class StaticCastIterator;

template<class Type, class Iterator>
bool operator==(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs)
{ return lhs.mIt == rhs.mIt; }

template<class Type, class Iterator>
bool operator!=(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs)
{ return lhs.mIt != rhs.mIt; }

template<class Type, class Iterator>
bool operator<(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs)
{ return lhs.mIt < rhs.mIt; }

template<class Type, class Iterator>
StaticCastIterator<Type,Iterator> operator+(StaticCastIterator<Type,Iterator> lhs, unsigned int rhs)
{ return StaticCastIterator<Type,Iterator>(lhs.mIt + rhs); }

template<class Type, class Iterator>
StaticCastIterator<Type,Iterator> operator-(StaticCastIterator<Type,Iterator> lhs, unsigned int rhs)
{ return lhs.mIt - rhs; }

template<class Type, class Iterator>
unsigned int operator-(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs)
{ return lhs.mIt - rhs.mIt; }

template<class Type, class Iterator>
class StaticCastIterator
{
public:
	StaticCastIterator(Iterator it): mIt(it) {}
	virtual ~StaticCastIterator() {}

	Type operator*()
	{
		//assert(mIt->inherits<Type>());
		return static_cast<Type>(*mIt);
	}

	Type operator->()
	{
		//assert(mIt->inherits<Type>());
		return static_cast<Type>(*mIt);
	}

	void operator++() { mIt++; }
	void operator--() { mIt--; }

	void operator=(StaticCastIterator<Type,Iterator> rhs) { mIt = rhs.mIt; }

private:
	friend bool operator==<>(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs);
	friend bool operator!=<>(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs);
	friend bool operator< <> (StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs);
	friend StaticCastIterator<Type,Iterator> operator+<>(StaticCastIterator<Type,Iterator> lhs, unsigned int rhs);
	friend StaticCastIterator<Type,Iterator> operator-<>(StaticCastIterator<Type,Iterator> lhs, unsigned int rhs);
	friend unsigned int operator-<>(StaticCastIterator<Type,Iterator> lhs, StaticCastIterator<Type,Iterator> rhs);

	Iterator mIt;
};

} //namespace Detail

#endif /*STATICCASTITERATOR_H_*/
