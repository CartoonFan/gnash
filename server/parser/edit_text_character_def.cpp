// edit_text_character_def.cpp:  Text tags, for Gnash.
//
//   Copyright (C) 2006, 2007, 2008 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//


// Based on the public domain text.cpp of Thatcher Ulrich <tu@tulrich.com> 2003

#include "smart_ptr.h" // GNASH_USE_GC
#include "stream.h"
#include "log.h"
#include "movie_definition.h" // for m_root_def use

#include "edit_text_character_def.h"
#include "edit_text_character.h"
#include "font.h" // for setReachable call

namespace gnash {

// Forward declarations
class movie_definition;

void
edit_text_character_def::read(stream* in, int tag_type,
		movie_definition* /*m*/)
{
	//assert(m != NULL);
	assert(tag_type == SWF::DEFINEEDITTEXT); // 37

	m_rect.read(in);

	in->align();
	in->ensureBytes(2);
    
    int flags = in->read_u8();
	m_has_text  = flags & (1 << 7);
	m_word_wrap = flags & (1 << 6);
	m_multiline = flags & (1 << 5);
	m_password  = flags & (1 << 4);
	m_readonly  = flags & (1 << 3); 
	bool  has_color      = flags & (1 << 2); 
	bool  has_max_length = flags & (1 << 1); 
	bool  has_font       = flags & (1 << 0); 

    flags = in->read_u8();
    // reserved_bit  = flags & (1 << 7);
	m_auto_size      = flags & (1 << 6); 
	bool  has_layout = flags & (1 << 5); 
	m_no_select      = flags & (1 << 4); 
	m_border         = flags & (1 << 3); 
	// reserved_bit  = flags & (1 << 2);
	m_html           = flags & (1 << 1); 
	m_use_outlines   = flags & (1 << 0); 

	if (has_font)
	{
		in->ensureBytes(4);
		m_font_id = in->read_u16();
		m_text_height = in->read_u16();
	}

	if (has_color)
	{
		m_color.read_rgba(in);
	}

	if (has_max_length)
	{
		in->ensureBytes(2);
		m_max_length = in->read_u16();
	}

	if (has_layout)
	{
		in->ensureBytes(9); //1 + 2 + 2 + 2 + 2
		m_alignment = static_cast<alignment>(in->read_u8());
		m_left_margin = in->read_u16(); // used to be cast to float
		m_right_margin = in->read_u16(); // used to be cast to float
		m_indent = in->read_s16();
		m_leading = in->read_s16();
	}

	in->read_string(m_variable_name);

	if (m_has_text)
	{
		in->read_string(m_default_text);
	}

	IF_VERBOSE_PARSE (
		log_parse("edit_text_char:\n"
			" varname = %s\n"
			" text = ``%s''\n"
			" font_id: %d\n"
			" text_height: %d",
			m_variable_name.c_str(),
			m_default_text.c_str(),
			m_font_id,
			m_text_height);
	);
}

const font*
edit_text_character_def::get_font()
{
	if (m_font == NULL)
	{
		// Resolve the font, if possible.
		m_font = m_root_def->get_font(m_font_id);
		if (m_font == NULL)
		{
			// this is fine, the textfield would use a default device font
			//log_debug(_("text style with undefined font; font_id = %d; using a default font"), m_font_id);
			m_font = fontlib::get_default_font().get();
		}
	}

	return m_font;
}

character*
edit_text_character_def::create_character_instance(character* parent,
		int id)
{
	// Resolve the font, if possible
	get_font();
	edit_text_character* ch = new edit_text_character(parent, this, id);

	// This gives an "instance name" to the TextField, but
	// it is not really what we need.
	//
	// First of all the VariableName ("m_variable_name") is
	// NOT the default name of an instance, rather it is
	// a variable associated with it and can contain path
	// information (ie. we can associate a variable in a different
	// timeline)
	//
	// We actually need to set that variable to an object which
	// is a TextField dinamic variable. The object should take
	// care of updating this TextField text when assigned to
	// and to retrive this TextField text when extracted value from.
	//
	// The DefineEditTextVariableNameTest.swf file under
	// testsuite/misc-ming.all gives an idea of the problem
	// (in particular it shows a case in which VariableName is
	// outside of TextField timeline/scope)
	//
	//ch->set_name(m_variable_name.c_str());

	return ch;
}

#ifdef GNASH_USE_GC
void
edit_text_character_def::markReachableResources() const
{
	if ( m_root_def ) m_root_def->setReachable();
	if ( m_font ) m_font->setReachable();
}
#endif // GNASH_USE_GC

} // namespace gnash

// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
