/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	RealPage.hh
	\brief	definition the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_PAGE_HEADER_INCLUDED__
#define __PDF_REAL_PAGE_HEADER_INCLUDED__

// base class headers
#include "page/Page.hh"
#include "PageTree.hh"
#include "PageInfo.hh"

// other libpdfdoc headers
#include "stream/Stream.hh"

#include <iosfwd>

namespace pdf {

class Dictionary ;
class RealResources ;
class File ;
class GraphicsState ;

/*!	\brief	real implementation of a page
	
	This class represents a page in the PDF document. It is an implementation
	of the Page interface.
*/
class RealPage : public Page, public PageNode
{
public :
	explicit RealPage( PageTree *parent ) ;
	~RealPage( ) ;

	void Read( DictReader& dict ) ;
	void Write(
		const Ref& 				link,
		File 					*file,
		const Ref& 				parent,
		const FontSubsetInfo	*ss ) const ;

	Rect MediaBox( ) const ;
	Rect CropBox( ) const ;
	
	void Clear( ) ;
	
	std::size_t Count( ) const ;
	PageTree* Parent( ) ;
	
	RealPage* GetLeaf( std::size_t index ) ;
	
	RealResources* Resource( ) ;
	const RealResources* Resource( ) const ;

	int Rotation( ) const ;
	
	void AddInlineImage( std::size_t width, std::size_t height,
		const std::string& file ) ;
	
	void VisitGraphics( GraphicsVisitor *visitor ) const ;
	void SetContent( const std::vector<Graphics*>& gfx ) ;

	void GetRawContent( std::vector<unsigned char>& out ) const ;

	void SetRawContent( const unsigned char *data, std::size_t size ) ;

private :
	void ReadContent( const Object& str_obj, File *file ) ;
	Object WriteContent( File *file ) const ; 

	void WriteGraphic(
		const Graphics	*gfx,
		std::ostream& 	os,
		GraphicsState& 	gs,
		RealResources	*res ) ;

private :
	PageInfo			m_pinfo ;

	std::vector<Stream>	m_cstrs ;
} ;

} // end of namespace

#endif
