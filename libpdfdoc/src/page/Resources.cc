/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/*!
	\file	Resources.cc
	\brief	implementation the Resources class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#include "Resources.hh"

#include "XObject.hh"

#include "core/Array.hh"
#include "file/ElementReader.hh"
#include "file/IElementDest.hh"
#include "file/ElementList.hh"
#include "font/BaseFont.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace pdf {

Resources::Resources( )
	: m_proc_set( 1, Name( "PDF" ) )
{
    m_proc_set.push_back( Name( "Text" ) ) ;
}

Resources::Resources( const Dictionary& dict, ElementReader *repo )
	: m_proc_set( 1, Name( "PDF" ) )
{
	assert( repo != 0 ) ;
	
	m_self = dict ;
	OnRead( repo ) ;
}

void Resources::Init( Object& obj, ElementReader *repo )
{
	assert( repo != 0 ) ;

	std::swap( m_self, obj.As<Dictionary>() ) ;
	OnRead( repo ) ;
}

void Resources::OnRead( ElementReader *repo )
{
	Array proc_set ;
	repo->Detach( m_self, "ExtGState",	m_ext_gstate ) ;
	repo->Detach( m_self, "ProcSet",	proc_set ) ;
	m_proc_set.assign( proc_set.begin( ), proc_set.end( ) ) ;
	
	ReadSubDict( "Font",    repo, m_fonts ) ;
	ReadSubDict( "XObject", repo, m_xobjs ) ;
}

void Resources::Write( const Ref& link, IElementDest *repo ) const
{
    Dictionary dict( m_self ) ;
	dict["ProcSet"]	= Array( m_proc_set.begin( ), m_proc_set.end( ) ) ;
    dict["Font"]	= WriteSubDict( m_fonts, repo ) ;
    dict["XObject"]	= WriteSubDict( m_xobjs, repo ) ;
    
    return repo->WriteObj( dict, link ) ;
}

template <typename T>
Dictionary Resources::WriteSubDict( const std::map<Name, T*>& input,
                                    IElementDest *file ) const
{
	Dictionary dict ;
	
	typedef typename std::map<Name, T*>::const_iterator FontIt ;
	for ( FontIt i = input.begin( ) ; i != input.end( ) ; ++i )
		dict.insert( std::make_pair( i->first, file->Write( i->second ) ) ) ;

	return dict ;
}

template <typename T>
void Resources::ReadSubDict( const Name& name, ElementReader *file,
					         std::map<Name, T*>& output )
{
	assert( file != 0 ) ;
	
	Dictionary dict ;
	if ( file->Detach( m_self, name, dict ) )
	{	
		for ( Dictionary::const_iterator i  = dict.begin( ) ;
										 i != dict.end( ) ; ++i )
			output.insert( std::make_pair( i->first,
										file->Read<T>( i->second ) ) ) ;
	}
}

XObject* Resources::ReadXObj( const Ref& link )
{
	// XObjects are made from streams, so we read them as streams
	return 0 ;
}

Name Resources::AddFont( BaseFont *font )
{
	// first, see if the font is already added
	using boost::lambda::bind ;
	using boost::lambda::_1 ;
	FontMap::iterator it = std::find_if( m_fonts.begin( ), m_fonts.end( ),
		bind( &FontMap::value_type::second, _1 ) == font ) ;
	if ( it != m_fonts.end( ) )
		return it->first ;
	
	// create a new name
	Name name ;
	do
	{
		std::ostringstream oss ;
		oss << "F" << m_fonts.size( ) ;
		name = Name( oss.str() ) ;
	} while ( m_fonts.find( name ) != m_fonts.end( ) ) ;
	
	m_fonts.insert( std::make_pair( name, font ) ) ;
	return name ;
}

ElementList Resources::GetChildren( ) const
{
	ElementList e ;
	std::transform( m_fonts.begin( ), m_fonts.end( ), std::back_inserter( e ),
	                SelectSecond( ) ) ;
	std::transform( m_xobjs.begin( ), m_xobjs.end( ), std::back_inserter( e ),
	                SelectSecond( ) ) ;
	return e ;
}

} // end of namespace
