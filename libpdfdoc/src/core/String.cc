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

/*!	\file	String.cc
	\brief	implementation the String class
	\date	Sun Mar 9 2008
	\author	Nestal Wan
*/

#include "String.hh"

#include "Token.hh"

#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <functional>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <sstream>

namespace pdf {

/// Construct a string object with an std::string
String::String( const std::string& str )
	: m_value( str )
{
}

/// Returns the underlying string as std::string
const std::string& String::Get( ) const
{
	return m_value ;
}

/// Automatic conversion to std::string
String::operator std::string( ) const
{
	return m_value ;
}

/// Read from std::istream.
/**	This function will create a TokenSrc for reading. It is recommended to use
	the extraction operator of TokenSrc instead.
*/
std::istream& operator>>( std::istream& src, String& obj )
{
	Token t ;
	if ( src >> t )
	{
		const std::string& str = t.Get() ;
	
		if ( !str.empty() )
		{
			if ( str[0] == '(' )
				obj.DecodeLiteralString( str ) ;
			
			else if ( str[0] == '<' )
				obj.DecodeHexString( src ) ;
			
			else
				src.setstate( std::ios::failbit ) ;
		}
	}
	return src ;
}

bool String::GetChar(
	char&							ch,
	std::string::const_iterator&	it,
	std::string::const_iterator		end )
{
	if ( it != end )
	{
		ch = *it ;
		++it ;
		return true ;
	}
	else
		return false ;
}

void String::DecodeLiteralString( const std::string& token )
{
	// assume the first char is '('
	int bracket_balance = 1 ;

	DecodeState state = done ;

	std::string::const_iterator it = token.begin() ;
	
	// skip the first bracket character
	PDF_ASSERT( it	!= token.end() ) ;
	PDF_ASSERT( *it	== '(' ) ;
	++it ;
	
	char ch ;
	while ( state == extra || GetChar( ch, it, token.end() ) )
	{
		state = done ;
	
		// escape character
		switch ( ch )
		{
			case '\\' :
				if ( !HandleEscapeCharacter( ch, it, token.end() ) )
				{
					state = HandleOctal( ch, it, token.end() ) ;
					if ( state == quit )
						break ;
					
					// PDF specs indicates for unknown character after escape
					// character '\' will ignore the escape character, i.e.
					// the character after '\' will still be counted.
					continue ;
				}
				break ;
			
			case '(' :
				bracket_balance++ ;
				break ;
			
			case ')' :
				bracket_balance-- ;
				if ( bracket_balance <= 0 )
					return ;
		}
		
		m_value.push_back( ch ) ;
	} 
}

//void String::HandleOctal( TokenSrc& is, char& ch, bool& quit, bool& used )
String::DecodeState String::HandleOctal(
	char&							ch,
	std::string::const_iterator&	it,
	std::string::const_iterator		end )
{
	DecodeState ret = extra ;
	
	// handle the \ddd octal digits here
	if ( ch >= '0' && ch < '8' )
	{
		char val = ch - '0' ;
		
		// at most 2 more octal digits
		for ( int i = 0 ; i < 2 ; i++ )
		{
			// no character in input, quit
			if ( !GetChar( ch, it, end ) )
				return quit ;
			
			else if ( ch >= '0' && ch < '8' )
			{
				val <<= 3 ;
				val |= (ch - '0') ;
				
				ret = done ;
			}
			
			else
			{
				ret = extra ;
				break ;
			}
		}
		
		m_value.push_back( val ) ;
	}
	return ret ;
}

bool String::HandleEscapeCharacter(
	char&							ch,
	std::string::const_iterator&	it,
	std::string::const_iterator		end )
{
	if ( GetChar( ch, it, end ) )
	{
		switch ( ch )
		{
			case 'n'  : ch = '\n' ;     break ;
			case 'r'  : ch = '\r' ;     break ;
			case 't'  : ch = '\t' ;     break ;
			case 'b'  : ch = '\b' ;     break ;
			case 'f'  : ch = '\f' ;     break ;
			case '('  : ch = '(' ;      break ;
			case ')'  : ch = ')' ;      break ;
			case '\\' : ch = '\\' ;     break ;
			
			// unknown character after escape is not an error.
			// it will be ignored.
			default :
				return false ;
		}
	}
	return true ;
}

std::istream& String::ReadXDigit( std::istream& is, char& digit )
{
	// skip space characters
	while ( is.get( digit ) && std::isspace( digit ) )
	{
	}
	return is ;
}

void String::DecodeHexString( std::istream& is )
{
	// the 3rd byte is never written to keep it null-terminated
	char ch[3] = {} ;
	
	while ( ReadXDigit( is, ch[0] ) )
	{
		if ( ch[0] == '>' )
			break ;
		
		if ( std::isxdigit( ch[0] ) )
		{
			if ( !ReadXDigit( is, ch[1] ) )
			{
				is.setstate( std::ios::failbit ) ;
				break ;
			}
			else
			{
				bool quit = false ;
				if ( ch[1] == '>' )
				{
					ch[1] = '0' ; // treat as 0 for missing second character
					quit = true ;
				}
				else if ( !std::isxdigit( ch[1] ) )
				{
					is.setstate( std::ios::failbit ) ;
					break ;
				}
				
				m_value.push_back( static_cast<char>(std::strtol( ch, 0, 16 )));
				
				if ( quit )
					break ;
			}
		}
		else
			is.setstate( std::ios::failbit ) ;
	}
}

namespace
{
	unsigned short ToInt( char ch )
	{
		return static_cast<unsigned char>( ch ) ;
	}

	bool IsPrint( char ch )
	{
		return
			std::isprint( static_cast<unsigned char>( ch ) ) != 0 ||
			ch == '\n' ;
	}
}

std::ostream& operator<<( std::ostream& os, const String& b )
{
	if ( b.IsHex() )
	{
		os << '<' ;
		for ( std::string::const_iterator i = b.m_value.begin() ;
			i != b.m_value.end() ; ++i )
		{
			os	<< std::hex << std::setw( 2 ) << std::setfill('0')
				<< static_cast<unsigned short>(static_cast<unsigned char>(*i)) ;
		}
		os << '>' << std::dec ;
	}
	else
	{
		os.put( '(' ) ;
		for ( std::string::const_iterator i = b.m_value.begin() ;
			i != b.m_value.end() ; ++i )
		{
			if ( *i == '(' || *i == ')' )
				os.put( '\\' ) ;
			os.put( *i ) ;
		}
		os.put( ')' ) ;
	}
	return os ;
}

bool String::operator==( const String& str ) const
{
	return m_value == str.m_value ;
}

bool String::IsHex( ) const
{
	return std::find_if(
		m_value.begin( ),
		m_value.end( ),
		!boost::bind( IsPrint, _1 ) ) != m_value.end( ) ;
}

void String::Swap( std::string& val )
{
	m_value.swap( val ) ;
}

void String::Swap( String& val )
{
	m_value.swap( val.m_value ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::String& s, std::string& val )
	{
		s.Swap( val ) ;
	}

	void swap( pdf::String& s1, pdf::String& s2 )
	{
		s1.Swap( s2 ) ;
	}
}
