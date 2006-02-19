/** \file richtext.h
 ** \brief Contains the RichText class definition
 **/

#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <list>

#include "SDL.h"

#include "widgetbase.h"
#include "text.h"
#include "boost/shared_ptr.hpp"
#include "util/image.h"

extern Image screen;

/// Renders rich text using a customized set of format codes
class RichText : public Widget {
public:
	typedef boost::shared_ptr<Text> TextPtr;
	typedef std::list<TextPtr> TextList;

	// Constructors
	RichText();
	RichText(const int x, const int y, const int w, const int h,
		Image parent_surf = screen);
	RichText(const std::string& format_text, Image parent_surf = screen);
	RichText(const int x, const int y, const int w, const int h,
		const std::string& format_text, Image parent_surf = screen);
	
	// Copying and assignment
	RichText::RichText(const RichText& rhs);
	RichText& operator=(const RichText& rhs);

	~RichText();

	bool resize(const int w, const int h);
	bool set_caption(const std::string& format_text);
	bool size_to_text();

	bool draw();
	
private:
	/// Formatted text, can override any of the defaults stored in the members
	std::string m_format_text;

	/// Default text size
	int m_size;
	/// Default text style code
	int m_style;
	/// Default foreground color
	Uint8 m_fr, m_fg, m_fb;
	/// Default background color
	Uint8 m_br, m_bg, m_bb;

	/// All the instances of Text used to render the RichText.
	/**
	 ** RichText uses a new Text instance each time the style is changed and
	 ** then renders all the instances next to each other to create rich text.
	 ** This is possible because every Text instance can have a style for the
	 ** whole glob of text, but not for individual globs (hence why RichText was
	 ** created).
	 **/
	TextList m_text;

	/// Whether or not to use a background color
	/** If this is false the background is transparent by default. **/
	bool m_use_background;

	void init(const int x, const int y, const int w, const int h,
			 const std::string& format_text, Image parent_surf,
			 bool auto_size = false);

	bool parse();
};

#endif // RICHTEXT_H
