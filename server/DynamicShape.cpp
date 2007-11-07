// 
//   Copyright (C) 2007 Free Software Foundation, Inc.
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



/* $Id: DynamicShape.cpp,v 1.11 2007/11/07 12:08:16 strk Exp $ */

#include "DynamicShape.h"

#include <cfloat>
#include <algorithm>


namespace gnash {

DynamicShape::DynamicShape()
	:
	shape_character_def(),
	_currpath(0),
	_currfill(0),
	_currline(0),
	_x(0),
	_y(0),
	_changed(0)
{}

void
DynamicShape::clear()
{
	//clear_meshes();
	m_paths.clear();
	m_fill_styles.clear();
	m_line_styles.clear();
	m_bound.set_null();
}

void
DynamicShape::add_path(const path& pth)
{
	m_paths.push_back(pth);
	_currpath = &(m_paths.back());
	//compute_bound(&m_bound);
}

void
DynamicShape::endFill()
{
	// Close the path
	if ( _currpath ) _currpath->close();

	// Remove reference to the "current" path, as
	// next drawing will happen on a different one
	_currpath = NULL;

	// Remove fill information
	_currfill = 0;

	// TODO: should I also clear _currline ?
}

void
DynamicShape::beginFill(const rgba& color)
{
	// Add the new fill style and set as current
	fill_style style; style.setSolid(color);

	endFill();

	_currfill = add_fill_style(style);
	// TODO: how to know wheter the fill should be set
	//       as *left* or *right* fill ?
	//       A quick test shows that *left* always work fine !
	path newPath(_x, _y, _currfill, 0, _currline, true); // new fill start new subshapes
	add_path(newPath);
}

void
DynamicShape::startNewPath(bool newShape)
{
	// Close any pending filled style
	// The DrawingApiTest.swf file shows we should NOT
	// necessarely end the current fill when starting a new one.
	//endFill();

	// A quick test shows that *left* always work fine !
	// More than that, using a *right* fill seems to break the tests !
	path newPath(_x, _y, _currfill, 0, _currline, newShape);
	add_path(newPath);
}

void
DynamicShape::finalize()
{
	// Nothing to do if not changed
	if ( ! _changed ) return;

	// TODO: check consistency of fills and such !

	_changed = false;
}

void
DynamicShape::lineStyle(uint16_t thickness, const rgba& color)
{
	line_style style(thickness, color);
	_currline = add_line_style(style);
	startNewPath(false); // don't make this the start of a new subshape (to verify)
}

void
DynamicShape::resetLineStyle()
{
	_currline = 0;
	startNewPath(false); // don't make this the start of a new subshape (to verify)
}

void
DynamicShape::moveTo(float x, float y)
{
	if ( x != _x || y != _y )
	{
		_x = x;
		_y = y;
		// TODO: close previous path if any and filled ?
		startNewPath(false); // don't make this the start of a new subshape (to verify)
	}
}

void
DynamicShape::lineTo(float x, float y)
{
	if ( ! _currpath ) startNewPath(true); // first shape is always new (I hope this doesn't break anything)
	assert(_currpath);

	_currpath->drawLineTo(x, y);

	// Update bounds 
	unsigned thickness = _currline ? m_line_styles[_currline-1].get_width() : 0;
	if ( _currpath->size() == 1 ) {
		_currpath->expandBounds(m_bound, thickness);
	} else {
		m_bound.expand_to_circle(x, y, thickness);
	}

	// Update current pen position
	_x = x;
	_y = y;

	// Mark as changed
	changed();
}

void
DynamicShape::curveTo(float cx, float cy, float ax, float ay)
{
	if ( ! _currpath ) startNewPath(true); // first shape is always new (I hope this doesn't break anything)
	assert(_currpath);

	_currpath->drawCurveTo(cx, cy, ax, ay);

	// Update bounds 
	unsigned thickness = _currline ? m_line_styles[_currline-1].get_width() : 0;
	if ( _currpath->size() == 1 ) {
		_currpath->expandBounds(m_bound, thickness);
	} else {
		m_bound.expand_to_circle(ax, ay, thickness);
		m_bound.expand_to_circle(cx, cy, thickness);
	}

	// Update current pen position
	_x = ax;
	_y = ay;

	// Mark as changed
	changed();
}

size_t
DynamicShape::add_fill_style(const fill_style& stl)
{
	typedef FillStyleVect V;
	V& v=m_fill_styles;

	// TODO: check if the style is already in our list
	//       (needs operator== defined for fill_style)
	v.push_back(stl);
	return v.size(); // 1-based !
}

size_t
DynamicShape::add_line_style(const line_style& stl)
{
	typedef LineStyleVect V;
	V& v=m_line_styles;

	// TODO: check if the style is already in our list
	//       (needs operator== defined for line_style)
	v.push_back(stl);
	return v.size(); // 1-based !
}
	
}	// end namespace gnash


// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
