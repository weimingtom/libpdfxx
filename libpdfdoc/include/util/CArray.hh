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

/**	\file	CArray.hh
    \brief	definition the C array helper functions class
    \date	Jul 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_CARRAY_HH_EADER_INCLUDED__
#define __PDF_CARRAY_HH_EADER_INCLUDED__

#include <cstddef>

namespace pdf {

/*!	\brief	get the begin iterator from an array
	\internal
	
	This function returns the begin "iterator" of an array. It is useful to
	treat an array like an STL container.
	
	For example:
\code
int array[10] = { 1, 2, 3, 4, 5 } ;
std::vector<int> v ;
std::copy( Begin(array), End(array), std::back_inserter( v ) ;
\endcode

	\param	array	reference to the array
	\return	the begin iterator of the array. i.e. \a array itself
	\sa End(), Count()
*/
template <typename T, std::size_t n>
T* Begin( T (&array)[n] )
{
	return array ;
}

/*!	\brief	get the end iterator from an array
	\internal
	
	This function returns the end "iterator" of an array. It is useful to
	treat an array like an STL container.
	
	\param	array	reference to the array
	\return	the end iterator of the array. i.e. \a array+n
	\sa Begin(), Count()
*/
template <typename T, std::size_t n>
T* End( T (&array)[n] )
{
	return array + n ;
}

/*!	\brief	get the number of elements in the array
	\internal

	This function will return the number of elements in the array.
	\return	the number of elements in the array
	\sa Begin(), End()
*/
template <typename T, std::size_t n>
std::size_t Count( T (&array)[n] )
{
	return n ;
}

} // end of namespace

#endif // CARRAY_HH_
