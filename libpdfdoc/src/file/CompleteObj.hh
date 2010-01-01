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

/**
    \file	CompleteObj.hh
    \brief	definition the CompleteObj class
    \date	Jan 1, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COMPLETEOBJ_HH_EADER_INCLUDED__
#define __PDF_COMPLETEOBJ_HH_EADER_INCLUDED__

#include "core/Object.hh"
#include "core/ObjWrapper.hh"

#include <map>

namespace pdf {

class IFile ;
class Dictionary ;
class Array ;

/**	\brief	brief description

	The CompleteObj class represent a PDF object, as well as a mapping from
	the indirect references it contains to the real objects. The real objects
	referred by the references will stored inside RefCounterWrapper. They are
	shared between other CompleteObj's.
*/
class CompleteObj
{
public :
	CompleteObj( ) ;
	
	void Read( Object& dict, IFile *file ) ;
	Ref Write( IFile *file ) const ;

	Object& Get( ) ;
	const Object& Get( ) const ;

private :
	void ReplaceReference( Object& obj, IFile *file ) const ;

private :
	Object	m_obj ;
	
	typedef std::map<Ref, ObjWrapper*> ObjMap ;
	ObjMap	m_refs ;
} ;

} // end of namespace

#endif // COMPLETEOBJ_HH_
