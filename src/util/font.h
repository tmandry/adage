/** \file font.h
 ** \brief Contains the Font class definition
 **/

#ifndef FONT_H
#define FONT_H

#include "config.h"

#include "SDL_ttf.h"
#include "boost/shared_ptr.hpp"

/// boost::shared_ptr, stripped and adapted for use with TTF_Font
class Font
{
private:

    // Borland 5.5.1 specific workaround
    typedef Font this_type;

public:

    typedef TTF_Font element_type;
    typedef TTF_Font value_type;
    typedef TTF_Font * pointer;
    typedef TTF_Font& reference;

    Font();

    template<class Y>
    explicit Font( Y * p ): px( p ) // Y must be complete
    {
		
#ifndef BOOST_NO_EXCEPTIONS
		
		try {
			pn = new int(1);
		} catch (...) {
			TTF_CloseFont(px);
			throw;
		}
		
#else
		
		pn = new int(1);
		
		if (pn == 0) {
			TTF_CloseFont(px);
			boost::throw_exception(std::bad_alloc());
		}
		
#endif
		
        /*boost::detail::sp_enable_shared_from_this( pn, p, p );*/
    }
	
	~Font();

//  generated copy constructor is fine

    Font & operator=(Font const & r); // never throws

    Font(Font const & r); // never throws

    void reset(); // never throws in 1.30+

    template<class Y> void reset(Y * p); // Y must be complete

    reference operator* () const; // never throws

    TTF_Font * operator-> () const; // never throws
    
    TTF_Font * get() const; // never throws
	
    // implicit conversion to "bool"

#if defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x530)

    operator bool () const;

#elif \
    ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
    ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) )

    typedef TTF_Font * (this_type::*unspecified_bool_type)() const;
    
    operator unspecified_bool_type() const; // never throws
	
#else 

    typedef TTF_Font * this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const; // never throws

#endif

    // operator! is redundant, but some compilers need it

    bool operator! () const; // never throws

    bool unique() const; // never throws

    long use_count() const; // never throws

    void swap(Font & other); // never throws

    bool _internal_less(Font const & rhs) const;

private:

    TTF_Font * px;                     // contained pointer
    int* pn;    // reference counter

};  // Font

#endif // FONT_H
