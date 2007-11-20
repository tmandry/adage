// const.h
//  Defines mathematic constants

#include <limits>

#ifndef CONST_H
#define CONST_H

namespace Math {

const double minDouble	= std::numeric_limits<double>::min();
const double maxDouble	= std::numeric_limits<double>::max();
const float minFloat	= std::numeric_limits<float>::min();
const float maxFloat	= std::numeric_limits<float>::max();
const double epsilon	= 1E-12;

const double E = 2.718281828459045;
const double PI = 3.14159265358979323846; 

} // namespace Math

#endif /*CONST_H*/
