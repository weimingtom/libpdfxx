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

/**	\file	PageLoader.hh
    \brief	definition the PageLoader class
    \date	Mar 26, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PAGELOADER_HH_EADER_INCLUDED__
#define __PDF_PAGELOADER_HH_EADER_INCLUDED__

#include <graphics/GraphicsVisitor.hh>

class QGraphicsScene ;
class QGraphicsItem ;

namespace pdf {

class TextLine ;

///	brief description
/**	\internal
	The PageLoader class represents
*/
class PageLoader : public GraphicsVisitor
{
public :
	PageLoader( QGraphicsScene *scene ) ;

	void VisitText( Text *text ) ;
	void VisitGraphics( Graphics *gfx ) ;
	void VisitPath( Path *path ) ;
	void VisitRenderedObject( GraphicsLink<Image> *img ) ;

private :
	void LoadTextLine( QGraphicsItem *parent, const TextLine& line ) ;

private :
	QGraphicsScene *m_scene ;
} ;

} // end of namespace

#endif // PAGELOADER_HH_
