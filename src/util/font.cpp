/** \file font.cpp
 ** \brief Contains the FontHolder<void> methods
 **/

#include "util/font.h"
#include "boost/shared_ptr.hpp"
#include "SDL_ttf.h"

typedef TTF_Font element_type;
typedef TTF_Font value_type;
typedef TTF_Font * pointer;
typedef TTF_Font& reference;
typedef Font this_type;

Font::Font(): px(0), pn(0) // never throws in 1.30+
{
}

Font::~Font()
{
	if (pn == 0) return;
	BOOST_ASSERT(px != 0);
	
	--(*pn);
	if ((*pn) == 0) {
		TTF_CloseFont(px);
		delete pn;
	}
}

Font & Font::operator=(Font const & r) // never throws
{
	reset();
	
    px = r.px;
    pn = r.pn;
	++(*pn);
    return *this;
}

Font::Font(Font const & r): px(r.px), pn(r.pn) // never throws
{
	++(*pn);
}

void Font::reset() // never throws in 1.30+
{
    this_type().swap(*this);
}

reference Font::operator* () const // never throws
{
    BOOST_ASSERT(px != 0);
    return *px;
}

TTF_Font * Font::operator-> () const // never throws
{
    BOOST_ASSERT(px != 0);
    return px;
}
    
TTF_Font * Font::get() const // never throws
{
    return px;
}

// implicit conversion to "bool"

#if defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x530)

Font::operator bool () const
{
    return px != 0;
}

#elif \
    ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
    ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) )

typedef TTF_Font * (this_type::*unspecified_bool_type)() const;

Font::operator unspecified_bool_type() const // never throws
{
    return px == 0? 0: &this_type::get;
}

#else 

typedef TTF_Font * this_type::*unspecified_bool_type;

Font::operator unspecified_bool_type() const // never throws
{
    return px == 0? 0: &this_type::px;
}

#endif

// operator! is redundant, but some compilers need it

bool Font::operator! () const // never throws
{
    return px == 0;
}

bool Font::unique() const // never throws
{
    return (*pn) == 1;
}

long Font::use_count() const // never throws
{
    return (*pn);
}

void Font::swap(Font & other) // never throws
{
    std::swap(px, other.px);
    std::swap(pn, other.pn);
}

bool Font::_internal_less(Font const & rhs) const
{
    return pn < rhs.pn;
}
