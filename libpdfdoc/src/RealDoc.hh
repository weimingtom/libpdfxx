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
	\file	RealDoc.hh
	\brief	header the PDF document class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_DOC_HEADER_INCLUDED__
#define __PDF_REAL_DOC_HEADER_INCLUDED__

#include "Doc.hh"

#include "file/Catalog.hh"
#include "page/RealPage.hh"
#include "font/StandardFont.hh"

// freetype library is optional
#ifdef HAVE_FREETYPE
#include "font/ftwrap/Library.hh"
#endif

#include <string>
#include <fstream>

namespace pdf {

class IndirectObj ;
class Font ;

class RealDoc : public Doc
{
public :
	RealDoc( ) ;
	~RealDoc( ) ;
	
	void Read( const std::string& filename ) ;
	void Write( const std::string& filename ) const ;

	RealPage* AppendPage( ) ;
	RealPage* AddPage( std::size_t index ) ;

	std::size_t PageCount( ) const ;
	RealPage* GetPage( std::size_t index ) ;

	StandardFont* CreateSimpleFont( const std::string& name ) ;

private :
	//! Catalog of the document
	Catalog				*m_catalog ;
	
	//! The file stream of the PDF file. It is only for reading.
	//! Some objects from the document will only be read on-demand.
	std::ifstream		m_readfs ;
	
#ifdef HAVE_FREETYPE
	//! Freetype library for loading the font files.
	ft::Library	m_ft_lib ;
#endif
} ;

} // end of namespace

#endif
