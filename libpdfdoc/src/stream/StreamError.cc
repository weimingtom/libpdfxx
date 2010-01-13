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
	\file	StreamError.cc
	\brief	implementation of the StreamError class
	\date	Jan 8, 2010
	\author	Nestal Wan
*/

#include "StreamError.hh"

#include <boost/format.hpp>

namespace pdf {

/**	constructor
*/
StreamError::StreamError( const std::string& msg )
	: Exception( msg )
{
}

StreamError::StreamError( boost::format fmt )
	: Exception( fmt )
{
}

} // end of namespace