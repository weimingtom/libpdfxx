/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
	\file	Backtrace.cc
	\brief	implementation the Backtrace class
	\date	Mon Mar 30 2009
	\author	Nestal Wan
*/

#include "Backtrace.hh"

#include "SymbolInfo.hh"
#include "Util.hh"

#include <ostream>

namespace pdf {

Backtrace::Backtrace( )
	: m_count( SymbolInfo::Instance()->Backtrace(m_stack, Count(m_stack) ))
{
}

/*!	\brief	operator<< for printing backtraces

	This function will call SymbolInfo::Backtrace() to print out a backtrace
	to the stream. It will use the SymbolInfo::Instance() singleton to get
	the symbol info.
	\param	os	the output stream
	\sa SymbolInfo::Backtrace(), SymbolInfo::Instance()
*/
std::ostream& operator<<( std::ostream& os, const pdf::Backtrace& b )
{
	// the 1st function in the stack is SymbolInfo::Backtrace()
	// the 2nd one is the Backtrace() constructor
	// both are not interesting to user
	for ( std::size_t i = 2 ; i < b.m_count ; i++ )
		SymbolInfo::Instance()->PrintTrace( b.m_stack[i], os, i - 2 ) ;

	return os ;
}

} // end of namespace
