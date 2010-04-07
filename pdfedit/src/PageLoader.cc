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

/**	\file	PageLoader.cc
	\brief	implementation of the PageLoader class
	\date	Mar 26, 2010
	\author	Nestal Wan
*/

#include "PageLoader.hh"

// local headers
#include "TextObject.hh"
#include "PathObject.hh"

// libpdfdoc headers
#include <graphics/Path.hh>
#include <graphics/PathSegment.hh>
#include <graphics/Text.hh>
#include <util/Debug.hh>

// Qt headers
//#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QPainterPath>

// boost headers
#include <boost/bind.hpp>

namespace pdf {

/**	constructor
	
*/
PageLoader::PageLoader( QGraphicsScene *scene )
	: m_scene( scene )
{
	PDF_ASSERT( m_scene != 0 ) ;
}

void PageLoader::VisitText( Text *text )
{
	PDF_ASSERT( text != 0 ) ;

	std::for_each( text->begin(), text->end(),
		boost::bind( &PageLoader::LoadTextLine, this, _1 ) ) ;
}

void PageLoader::LoadTextLine( const TextLine& line )
{
	PDF_ASSERT( m_scene != 0 ) ;
	m_scene->addItem( new TextObject( line ) ) ;
}

void PageLoader::VisitGraphics( Graphics *gfx )
{
}

void PageLoader::VisitPath( Path *path )
{
	m_scene->addItem( new PathObject( path ) ) ;
}

} // end of namespace
