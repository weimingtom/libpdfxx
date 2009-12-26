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
	\file	XObject.hh
	\brief	definition the XObject class
	\date	Sat May 10 2008
	\author	Nestal Wan
*/

#ifndef __PDF_XOBJECT_HEADER_INCLUDED__
#define __PDF_XOBJECT_HEADER_INCLUDED__

#include "stream/Stream.hh"

namespace pdf {

class IFile ;

/*!	\brief	PDF external objects
	
	According to the PDF specification: "An external object (commonly called an
	XObject) is a graphics object whose contents are defined by a self-contained
	content stream."
*/
class XObject
{
public :
	XObject( ) ;

	void Init( Object& link, IFile *repo ) ;

private :
	Stream	m_str ;
} ;

} // end of namespace

#endif
