#ifndef RAND_H_
#define RAND_H_

#include <cstdlib>
#include <ctime>

namespace Math {

inline void initRand()
{
	srand(time(NULL));
}

inline int randInt(int lower, int upper)
{
	return rand() % (upper - lower + 1) + lower;
}

inline float randFloat(float lower, float upper)
{
	return (static_cast<float>( rand() ) / RAND_MAX) * (upper - lower) + lower;
}

} //namespace Math

#endif /*RAND_H_*/
