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

/**	\file	ExtGraphicsLink.hh
    \brief	definition the Image class
    \date	May 11, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_EXT_GRAPHICS_LINK_HEADER_INCLUDED__
#define __PDF_EXT_GRAPHICS_LINK_HEADER_INCLUDED__

#include "graphics/ObjectAppearance.hh"
#include "GraphicsImpl.hh"

#include "graphics/GraphicsState.hh"
#include "util/Matrix.hh"

namespace pdf {

///	A link to a graphics object external to the page content.
/**	\ingroup graphics
	In a PDF document, there may be some graphics objects that are not stored
	in the page content. These objects includes images and PDF forms. The
	ExtGraphicsLink class is a link to these external object in the page.
	Multiple links to these external objects can appear in the PDF document
	or even in the same page, but only one copy will be save to file.
	
	Currently, only images are supported.
*/
template <typename T>
class ExtGraphicsLink : public ObjectAppearance<T>, public GraphicsImpl
{
public :
	ExtGraphicsLink( const GraphicsState& gs, const Matrix& ctm, const T *t ) ;
	~ExtGraphicsLink( ) ;

	void OnCommand( ContentOp& op, const ResourcesDict *res ) ;

	Matrix Transform( ) const ;
	void Transform( const Matrix& mat ) ;
	void Print(
		std::ostream&	os,
		ResourcesDict	*res,
		GraphicsState&	gs ) const ;
	void Visit( GraphicsVisitor *visitor ) ;
	
	GraphicsState GetState( ) const ;

	const T* Get( ) const ;

	GraphicsImpl* QueryImplementation( ) ;
	const GraphicsImpl* QueryImplementation( ) const ;
	Graphics* Interface( ) ;

private :
	Matrix			m_transform ;
	GraphicsState	m_gs ;
	
	const T	*m_obj ;
} ;

} // end of namespace

#endif // IMAGE_HH_
