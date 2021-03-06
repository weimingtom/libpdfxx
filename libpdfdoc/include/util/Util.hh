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
	\file	Util.hh
	\brief	Utility functions
	\date	Sun Mar 9 2008
	\author	Nestal Wan
	
	This file contains some utility functions that is not PDF specific.
*/

#ifndef __PDF_UTIL_HEADER_INCLUDED__
#define __PDF_UTIL_HEADER_INCLUDED__

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>

namespace pdf {

std::string Demangle( const char *name ) ;

std::ostream& PrintHex( std::ostream& os, const void *buf, std::size_t size ) ;

#ifdef WIN32
#define strcasecmp _stricmp
#endif

} // end of namespace

#endif
