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

/**	\file	RealText.cc
	\brief	implementation of the RealRealText class
	\date	Jan 13, 2010
	\author	Nestal Wan
*/

#include "RealText.hh"

#include "graphics/GraphicsVisitor.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/TextState.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"
#include "font/Font.hh"
#include "font/FontEncoding.hh"
#include "page/ContentOp.hh"
#include "util/Debug.hh"
#include "util/CArray.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <set>

namespace pdf {

struct RealText::HandlerMap
{
	/// command handler
	typedef void (RealText::*Handler)( ContentOp& , const ResourcesDict* ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const RealText::HandlerMap::Map::value_type	RealText::HandlerMap::m_val[] =
{
	// text positioning commands
	std::make_pair( "Td",	&RealText::OnTd ),
	std::make_pair( "TD",	&RealText::OnTD ),
	std::make_pair( "Tm",	&RealText::OnTm ),
	std::make_pair( "T*",	&RealText::OnTstar ),
	
	// text showing commands
	std::make_pair( "Tj",	&RealText::OnTj ),
	std::make_pair( "TJ",	&RealText::OnTJ ),
	std::make_pair( "\'",	&RealText::OnSingleQuote ),
	std::make_pair( "\"",	&RealText::OnDoubleQuote ),
} ;

const RealText::HandlerMap::Map RealText::HandlerMap::m_map(
    Begin( RealText::HandlerMap::m_val ),
    End( RealText::HandlerMap::m_val ) ) ;

/**	constructor
*/
RealText::RealText( const GraphicsState& gs, const Matrix& ctm )
	: m_lines( 1, RealTextLine( gs ) )
	, m_state( gs )
	, m_offset( 0 )
	, m_transform( ctm )
{
}

const RealTextLine* RealText::At( std::size_t idx ) const
{
	return &m_lines[idx] ;
}

RealTextLine* RealText::At( std::size_t idx )
{
	return &m_lines[idx] ;
}

RealTextLine& RealText::front()
{
	return m_lines.front() ;
}

RealTextLine& RealText::back()
{
	return m_lines.back() ;
}

const RealTextLine& RealText::front() const
{
	return m_lines.front() ;
}

const RealTextLine& RealText::back() const
{
	return m_lines.back() ;
}

void RealText::OnCommand( ContentOp& op, const ResourcesDict *res )
{
	PDF_ASSERT( !m_lines.empty() ) ;

	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
	if ( i != HandlerMap::m_map.end() )
		(this->*(i->second))( op, res ) ;
	
	else if ( GraphicsState::IsGSCommand( op.Operator() ) )
	{
		RealTextLine& current = m_lines.back() ;
		
		// state changed
		bool is_changed = m_state.OnCommand( op, res ) ;
					
		if ( current.IsEmpty() || is_changed )
		{
			if ( current.IsEmpty() )
				current.SetFormat( m_state ) ;
			else
			{
				Matrix temp = m_text_mat ;
				m_lines.push_back( RealTextLine(
					m_state, temp.Translate( m_offset, 0 ) ) ) ;
			}
		}
	}
}

void RealText::AddLine( const TextLine *line )
{
	AddLine( dynamic_cast<const RealTextLine&>(*line) ) ;
}

void RealText::AddLine( const RealTextLine& line )
{
	PDF_ASSERT( !m_lines.empty() ) ;

	// remove empty lines first
	if ( m_lines.back().IsEmpty() )
		m_lines.pop_back() ;
	
	m_lines.push_back( line ) ;
}

void RealText::AddLine( double x, double y, const std::wstring& text )
{
	Matrix temp = m_text_mat ;
	RealTextLine line( m_state, temp.Translate(x,y) ) ;
	line.AppendText( text ) ;
	return AddLine( line ) ;
}

void RealText::Visit( GraphicsVisitor *visitor )
{
	PDF_ASSERT( visitor != 0 ) ;
	
	visitor->VisitText( this ) ;
}

std::size_t RealText::Count( ) const
{
	return m_lines.size( ) ;
}

void RealText::Print(
	std::ostream&	os,
	ResourcesDict	*res,
	GraphicsState&	gs ) const
{
	os << "BT\n" ;
	
	Matrix			mat ;
	
	using namespace boost ;
	std::for_each(
		m_lines.begin(),
		m_lines.end(),
		bind( &RealTextLine::Print, _1, ref(os), ref(mat), ref(gs), res ) ) ;

	os << "ET\n" ;
}

/// For debug purpose only. Prints XML.
std::ostream& operator<<( std::ostream& os, const RealText& t )
{
	os << "<RealText>\n" ;
	std::copy(
		t.m_lines.begin(),
		t.m_lines.end(),
		std::ostream_iterator<RealTextLine>(os, "\n" ) ) ;
	os << "</RealText>\n" ;
	return os ;
}

void RealText::OnTd( ContentOp& op, const ResourcesDict * )
{
	if ( op.Count() >= 2 )
	{
		m_text_mat.Translate( op[0], op[1] ) ;
		m_offset = 0 ;

		AddLine( RealTextLine( m_state, m_text_mat ) ) ;
	}
}

void RealText::OnTD( ContentOp& op, const ResourcesDict * )
{
	if ( op.Count() >= 2 )
	{
		double	ty	= op[1] ;
		m_state.Text().Leading( -ty ) ;

		m_text_mat.Translate( op[0], ty ) ; 
		m_offset = 0 ;
		
		AddLine( RealTextLine( m_state, m_text_mat ) ) ;
	}
}

void RealText::OnTm( ContentOp& op, const ResourcesDict * )
{
	if ( op.Count() >= 6 )
	{
		// unlike Td and TD, the Tm command will replace the current
		// matrix.
		m_text_mat = Matrix( op[0], op[1], op[2], op[3], op[4], op[5] ) ;
		
		m_offset = 0.0 ;
		
		AddLine( RealTextLine( m_state, m_text_mat ) ) ;
	}
}

void RealText::OnTstar( ContentOp& , const ResourcesDict * )
{
	m_text_mat.Translate( 0, -m_state.Text().Leading() ) ;
	m_offset = 0 ;
	
	AddLine( RealTextLine( m_state, m_text_mat ) ) ;
}

///	Shows a Text string
void RealText::OnTj( ContentOp& op, const ResourcesDict * )
{
	PDF_ASSERT( !m_lines.empty() ) ;
	
	if ( op.Count() >= 1 )
	{
		RealTextLine& current = m_lines.back() ;
		
		// must set the font properly before showing text
		Font *font = current.Format().FontFace() ;
		if ( font != 0 )
		{
			std::wstring ws = DecodeString( op[0].As<std::string>(), font ) ;
			current.AppendText( ws ) ;
			m_offset += m_state.Text().Width( ws ) ;
		}
	}
}

std::wstring RealText::DecodeString( const std::string& s, Font *font )
{
	PDF_ASSERT( font != 0 ) ;

	std::wstring ws ;
	if ( font->Encoding() == 0 )
	{
		for ( unsigned i = 0 ; i < s.size() ; ++i )
			ws.push_back( static_cast<wchar_t>(
				static_cast<unsigned char>( s[i] ) ) ) ;
	}
	else
		ws = font->Encoding()->Decode( s ) ;
	
	return ws ;
}

///	\a array TJ
/**	Show one or more text strings, allowing individual glyph positioning. Each
	element of array can be a string or a number. If the element is a string,
	this operator shows the string. If it is a number, the operator adjusts the
	text position by that amount; that is, it translates the text matrix, Tm.
	The number is expressed in thousandths of a unit of text space. This amount
	is subtracted from the current horizontal or vertical coordinate, depending
	on the writing mode. In the default coordinate system, a positive adjustment
	has the effect of moving the next glyph painted either to the left or down
	by the given amount. 
*/
void RealText::OnTJ( ContentOp& op, const ResourcesDict * )
{
	PDF_ASSERT( !m_lines.empty() ) ;
	
	// current line
	RealTextLine& current = m_lines.back() ;

	double offset = 0.0 ;

	Font *font = current.Format().FontFace() ;

	if ( op.Count() > 0 && font != 0 )
	{
		const Array& a = op[0].As<Array>() ;
		for ( Array::const_iterator i = a.begin() ; i != a.end() ; ++i )
		{
			if ( i->Is<std::string>() )
			{
				std::wstring ws = DecodeString(i->As<std::string>(), font) ;
				offset += m_state.Text().Width( ws ) ;
				current.AppendText( ws ) ;
			}
			else if ( i->IsNumber() )
			{
				double disp = i->To<double>() / 1000.0 *
					m_state.Text().FontSize() ;
				
				offset -= disp ;
				current.AppendSpace( disp ) ;
			}
		}
	}

	m_offset += offset ;
}

void RealText::OnSingleQuote( ContentOp& op, const ResourcesDict *res )
{
	OnTstar( op, res ) ;
	OnTj( op, res ) ;
}

void RealText::OnDoubleQuote( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() >= 2 )
	{
//		OnTw( ContentOp( Token("Tw"), op.begin(), op.begin()+1 ), res ) ;
	}
}

bool RealText::operator==( const RealText& rhs ) const
{
	// no need to compare current text state
	return m_lines		== rhs.m_lines ;
}

bool RealText::operator!=( const RealText& rhs ) const
{
	return !operator==( rhs ) ;
}

GraphicsState RealText::GetState() const
{
	return m_state ;
}

Matrix RealText::Transform() const
{
	return m_transform ;
}

void RealText::Transform( const Matrix& mat )
{
	m_transform = mat ;
}

GraphicsImpl* RealText::QueryImplementation( )
{
	return this ;
}

const GraphicsImpl* RealText::QueryImplementation( ) const
{
	return this ;
}

Graphics* RealText::Interface( )
{
	return this ;
}

} // end of namespace
