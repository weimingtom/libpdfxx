/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

/**	\file	Glyph.hh
    \brief	definition the Glyph class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GLYPH_HEADER_INCLUDED__
#define __PDF_GLYPH_HEADER_INCLUDED__

#include <boost/shared_ptr.hpp>

namespace pdf {

// freetype wrappers
namespace ft
{
	struct Face ;
}

class Outline ;

///	brief description
/**	The Glyph class represents
*/
class Glyph
{
public :
	Glyph( unsigned idx, const ft::Face& face ) ;
	~Glyph( ) ;
	
	unsigned Width( ) const ;
	unsigned Height( ) const ;
	
	unsigned AdvanceX( ) const ;
	unsigned AdvanceY( ) const ;

	bool Decompose( Outline *outline ) const ;

private :
	struct Impl ;
	boost::shared_ptr<Impl>	m_impl ;
	
	struct OutlineDecomposer ;
} ;

} // end of namespace

#endif // GLYPH_HH_
