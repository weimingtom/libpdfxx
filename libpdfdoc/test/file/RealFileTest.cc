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
	\file	FileTest.cc
	\brief	implementation the FileTest class
	\date	Thu Apr 10 2008
	\author	Nestal Wan
*/

#include "RealFileTest.hh"

#include "file/RealFile.hh"

#include "core/Ref.hh"
#include "core/Dictionary.hh"
#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"
#include "util/CArray.hh"
#include "stream/Stream.hh"

#include "mock/Assert.hh"

namespace pdfut {

using namespace pdf ;

RealFileTest::RealFileTest( )
{
	m_catalog = m_doc_info = m_res = m_font = m_page = 0 ;
	m_content = 0 ;
}

void RealFileTest::setUp( )
{
	m_page = new Dictionary ;
	m_page->insert( "Type",		Name( "Page" ) ) ;
	m_page->insert( "Parent",	Ref( 1, 0 ) ) ;
	m_page->insert( "Resources",Ref( 2, 0 ) ) ;
	m_page->insert( "Contents",	Ref( 4, 0 ) ) ;
	
	m_content = new Stream(
		"2 J\n0.57 w\nBT /F1 16.00 Tf ET\n"
        "BT 31.19 794.57 Td (Hello World!) Tj ET" ) ;

	Ref pages[] = { Ref( 3, 0 ) } ;
	double mbox[] = { 0, 0, 595.28, 841.89 } ;
	m_page_tree = new Dictionary ;
	m_page_tree->insert( "Type",		Name( "Pages" ) ) ;
	m_page_tree->insert( "Kids",		Array( Begin(pages), End(pages) ) ) ;
	m_page_tree->insert( "Count",		1 ) ;
	m_page_tree->insert( "MediaBox",	Array( Begin(mbox), End(mbox) ) ) ;

	m_font = new Dictionary ;
	m_font->insert( "Type",		Name( "Font" ) ) ;
	m_font->insert( "BaseFont",	Name( "Helvetica-Bold" ) ) ;
	m_font->insert( "Subtype",	Name( "Type1" ) ) ;
	m_font->insert( "Encoding",	Name( "WinAnsiEncoding" ) ) ;

	m_res = new Dictionary ;
	Name pset[] = { Name("PDF"), Name("Text") } ;
	
	// resources
	m_res->insert( "ProcSet", Array( pset, pset+2 ) ) ;
	Dictionary font ;
	font.insert( "F1",		Ref( 5, 0 ) ) ;
	m_res->insert( "Font",	font ) ;

	// document info
	m_doc_info = new Dictionary ;
	m_doc_info->insert( "Producer", "nestal" ) ;
	m_doc_info->insert( "Creator", "D:20080410074227" ) ;
	
	// catalog
	m_catalog = new Dictionary ;
	m_catalog->insert( "Type", Name( "Catalog" ) ) ;
	m_catalog->insert( "Pages", Ref( 1, 0 ) ) ;
}

void RealFileTest::tearDown( )
{
	delete m_catalog ;
	delete m_doc_info ;
	delete m_res ;
	delete m_font ;
	delete m_content ;
	delete m_page ;
	
	m_catalog = m_doc_info = m_res = m_font = m_page = 0 ;
	m_content = 0 ;
}

void RealFileTest::TestSimple( )
{
	std::ostringstream file ;
// 	std::ofstream file( "a.pdf", std::ios::out | std::ios::binary ) ;
	RealFile f( &file ) ;
	
	Ref link[] =
	{
		Ref( ),
		f.AllocLink( ),
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
		f.AllocLink( ), 
	} ; 
	
	f.WriteObj( *m_page,		link[3] ) ;
	f.WriteObj( *m_content,		link[4] ) ;
	f.WriteObj( *m_page_tree,	link[1] ) ;
	f.WriteObj( *m_font,		link[5] ) ;
	f.WriteObj( *m_res,			link[2] ) ;
	f.WriteObj( *m_doc_info,	link[6] ) ;
	f.WriteObj( *m_catalog,		link[7] ) ;
	
	f.WriteTrailer( link[7], link[6] ) ;
	
	// open expected file to compare and verify
	std::ifstream exp( (std::string(TEST_DATA_DIR) +
	                   "FileTestSimple.pdf").c_str( ),
	                   std::ios::in | std::ios::binary ) ;
	CPPUNIT_ASSERT( exp ) ;
	
	std::string file_str = file.str() ;
	std::string exp_str(
		(std::istreambuf_iterator<char>( exp ) ),
		(std::istreambuf_iterator<char>( ) ) ) ;
	
	PDFUT_ASSERT_EQUAL( file_str, exp_str ) ;
}

void RealFileTest::TestReadStream( )
{
	// open expected file to compare and verify
	std::ifstream file( (std::string(TEST_DATA_DIR) +
	                    "FileTestSimple.pdf").c_str( ),
	                    std::ios::in | std::ios::binary ) ;
	RealFile f( &file ) ;
	
	pdf::Object obj = f.ReadObj( pdf::Ref( 4, 0 ) ) ;
	CPPUNIT_ASSERT( obj.Is<pdf::Stream>() ) ;

	std::stringstream output ;
	std::size_t count = obj.As<pdf::Stream>().CopyData( output.rdbuf() ) ;
	
	std::stringstream exp ;
	std::size_t exp_size = m_content->CopyData( exp.rdbuf() ) ;
	
	PDFUT_ASSERT_EQUAL( count, exp_size ) ;
	PDFUT_ASSERT_EQUAL( output.str(), exp.str() ) ;
}

void RealFileTest::TestReadType( )
{
	// open expected file to compare and verify
	std::ifstream file( (std::string(TEST_DATA_DIR) +
	                    "FileTestSimple.pdf").c_str( ),
	                    std::ios::in | std::ios::binary ) ;

}

} // end of namespace
