/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	BaseFont.cc
	\brief	implementation the BaseFont class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#include "BaseFont.hh"

#include <boost/bind.hpp>

namespace pdf {

BaseFont::BaseFont( )
{
}

boost::function<BaseFont* ()> BaseFont::Maker(
	Dictionary& 		obj,
	File 				*file,
	const ft::Library&	ft )
{
	return boost::bind( &CreateFont, boost::ref(obj), file, boost::ref(ft) ) ;
}

} // end of namespace
