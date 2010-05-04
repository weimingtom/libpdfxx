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

/**	\file	BuildInEncoding.cc
	\brief	implementation of the BuildInEncoding class
	\date	May 4, 2010
	\author	Nestal Wan
*/

#include "BuildInEncoding.hh"

#include "CodeMap.hh"
#include "font/FontException.hh"

#include "core/Name.hh"
#include "core/Object.hh"

#include "util/Debug.hh"

namespace pdf {

namespace
{
	const char *winansi_map[256] =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"space",
		"exclam",
		"quotedbl",
		"numbersign",
		"dollar",
		"percent",
		"ampersand",
		"quotesingle",
		"parenleft",
		"parenright",
		"asterisk",
		"plus",
		"comma",
		"hyphen",
		"period",
		"slash",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"colon",
		"semicolon",
		"less",
		"equal",
		"greater",
		"question",
		"at",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"bracketleft",
		"backslash",
		"bracketright",
		"asciicircum",
		"underscore",
		"grave",
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"braceleft",
		"bar",
		"braceright",
		"asciitilde",
		"bullet",
		"Euro",
		"bullet",
		"quotesinglbase",
		"florin",
		"quotedblbase",
		"ellipsis",
		"dagger",
		"daggerdbl",
		"circumflex",
		"perthousand",
		"Scaron",
		"guilsinglleft",
		"OE",
		"bullet",
		"Zcaron",
		"bullet",
		"bullet",
		"quoteleft",
		"quoteright",
		"quotedblleft",
		"quotedblright",
		"bullet",
		"endash",
		"emdash",
		"tilde",
		"trademark",
		"scaron",
		"guilsinglright",
		"oe",
		"bullet",
		"zcaron",
		"Ydieresis",
		"space",
		"exclamdown",
		"cent",
		"sterling",
		"currency",
		"yen",
		"brokenbar",
		"section",
		"dieresis",
		"copyright",
		"ordfeminine",
		"guillemotleft",
		"logicalnot",
		"hyphen",
		"registered",
		"macron",
		"degree",
		"plusminus",
		"twosuperior",
		"threesuperior",
		"acute",
		"mu",
		"paragraph",
		"periodcentered",
		"cedilla",
		"onesuperior",
		"ordmasculine",
		"guillemotright",
		"onequarter",
		"onehalf",
		"threequarters",
		"questiondown",
		"Agrave",
		"Aacute",
		"Acircumflex",
		"Atilde",
		"Adieresis",
		"Aring",
		"AE",
		"Ccedilla",
		"Egrave",
		"Eacute",
		"Ecircumflex",
		"Edieresis",
		"Igrave",
		"Iacute",
		"Icircumflex",
		"Idieresis",
		"Eth",
		"Ntilde",
		"Ograve",
		"Oacute",
		"Ocircumflex",
		"Otilde",
		"Odieresis",
		"multiply",
		"Oslash",
		"Ugrave",
		"Uacute",
		"Ucircumflex",
		"Udieresis",
		"Yacute",
		"Thorn",
		"germandbls",
		"agrave",
		"aacute",
		"acircumflex",
		"atilde",
		"adieresis",
		"aring",
		"ae",
		"ccedilla",
		"egrave",
		"eacute",
		"ecircumflex",
		"edieresis",
		"igrave",
		"iacute",
		"icircumflex",
		"idieresis",
		"eth",
		"ntilde",
		"ograve",
		"oacute",
		"ocircumflex",
		"otilde",
		"odieresis",
		"divide",
		"oslash",
		"ugrave",
		"uacute",
		"ucircumflex",
		"udieresis",
		"yacute",
		"thorn",
		"ydieresis"
	} ;

	const char *mac_roman_map[256] =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"space",
		"exclam",
		"quotedbl",
		"numbersign",
		"dollar",
		"percent",
		"ampersand",
		"quotesingle",
		"parenleft",
		"parenright",
		"asterisk",
		"plus",
		"comma",
		"hyphen",
		"period",
		"slash",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"colon",
		"semicolon",
		"less",
		"equal",
		"greater",
		"question",
		"at",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"bracketleft",
		"backslash",
		"bracketright",
		"asciicircum",
		"underscore",
		"grave",
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"braceleft",
		"bar",
		"braceright",
		"asciitilde",
		NULL,
		"Adieresis",
		"Aring",
		"Ccedilla",
		"Eacute",
		"Ntilde",
		"Odieresis",
		"Udieresis",
		"aacute",
		"agrave",
		"acircumflex",
		"adieresis",
		"atilde",
		"aring",
		"ccedilla",
		"eacute",
		"egrave",
		"ecircumflex",
		"edieresis",
		"iacute",
		"igrave",
		"icircumflex",
		"idieresis",
		"ntilde",
		"oacute",
		"ograve",
		"ocircumflex",
		"odieresis",
		"otilde",
		"uacute",
		"ugrave",
		"ucircumflex",
		"udieresis",
		"dagger",
		"degree",
		"cent",
		"sterling",
		"section",
		"bullet",
		"paragraph",
		"germandbls",
		"registered",
		"copyright",
		"trademark",
		"acute",
		"dieresis",
		"notequal",
		"AE",
		"Oslash",
		"infinity",
		"plusminus",
		"lessequal",
		"greaterequal",
		"yen",
		"mu",
		"partialdiff",
		"summation",
		"product",
		"pi",
		"integral",
		"ordfeminine",
		"ordmasculine",
		"Omega",
		"ae",
		"oslash",
		"questiondown",
		"exclamdown",
		"logicalnot",
		"radical",
		"florin",
		"approxequal",
		"Delta",
		"guillemotleft",
		"guillemotright",
		"ellipsis",
		"space",
		"Agrave",
		"Atilde",
		"Otilde",
		"OE",
		"oe",
		"endash",
		"emdash",
		"quotedblleft",
		"quotedblright",
		"quoteleft",
		"quoteright",
		"divide",
		"lozenge",
		"ydieresis",
		"Ydieresis",
		"fraction",
		"currency",
		"guilsinglleft",
		"guilsinglright",
		"fi",
		"fl",
		"daggerdbl",
		"periodcentered",
		"quotesinglbase",
		"quotedblbase",
		"perthousand",
		"Acircumflex",
		"Ecircumflex",
		"Aacute",
		"Edieresis",
		"Egrave",
		"Iacute",
		"Icircumflex",
		"Idieresis",
		"Igrave",
		"Oacute",
		"Ocircumflex",
		"apple",
		"Ograve",
		"Uacute",
		"Ucircumflex",
		"Ugrave",
		"dotlessi",
		"circumflex",
		"tilde",
		"macron",
		"breve",
		"dotaccent",
		"ring",
		"cedilla",
		"hungarumlaut",
		"ogonek",
		"caron"
	};

	const char *mac_expert_map[256] =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"space",
		"exclamsmall",
		"Hungarumlautsmall",
		"centoldstyle",
		"dollaroldstyle",
		"dollarsuperior",
		"ampersandsmall",
		"Acutesmall",
		"parenleftsuperior",
		"parenrightsuperior",
		"twodotenleader",
		"onedotenleader",
		"comma",
		"hyphen",
		"period",
		"fraction",
		"zerooldstyle",
		"oneoldstyle",
		"twooldstyle",
		"threeoldstyle",
		"fouroldstyle",
		"fiveoldstyle",
		"sixoldstyle",
		"sevenoldstyle",
		"eightoldstyle",
		"nineoldstyle",
		"colon",
		"semicolon",
		NULL,
		"threequartersemdash",
		NULL,
		"questionsmall",
		NULL,
		NULL,
		NULL,
		NULL,
		"Ethsmall",
		NULL,
		NULL,
		"onequarter",
		"onehalf",
		"threequarters",
		"oneeighth",
		"threeeighths",
		"fiveeighths",
		"seveneighths",
		"onethird",
		"twothirds",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"ff",
		"fi",
		"fl",
		"ffi",
		"ffl",
		"parenleftinferior",
		NULL,
		"parenrightinferior",
		"Circumflexsmall",
		"hypheninferior",
		"Gravesmall",
		"Asmall",
		"Bsmall",
		"Csmall",
		"Dsmall",
		"Esmall",
		"Fsmall",
		"Gsmall",
		"Hsmall",
		"Ismall",
		"Jsmall",
		"Ksmall",
		"Lsmall",
		"Msmall",
		"Nsmall",
		"Osmall",
		"Psmall",
		"Qsmall",
		"Rsmall",
		"Ssmall",
		"Tsmall",
		"Usmall",
		"Vsmall",
		"Wsmall",
		"Xsmall",
		"Ysmall",
		"Zsmall",
		"colonmonetary",
		"onefitted",
		"rupiah",
		"Tildesmall",
		NULL,
		NULL,
		"asuperior",
		"centsuperior",
		NULL,
		NULL,
		NULL,
		NULL,
		"Aacutesmall",
		"Agravesmall",
		"Acircumflexsmall",
		"Adieresissmall",
		"Atildesmall",
		"Aringsmall",
		"Ccedillasmall",
		"Eacutesmall",
		"Egravesmall",
		"Ecircumflexsmall",
		"Edieresissmall",
		"Iacutesmall",
		"Igravesmall",
		"Icircumflexsmall",
		"Idieresissmall",
		"Ntildesmall",
		"Oacutesmall",
		"Ogravesmall",
		"Ocircumflexsmall",
		"Odieresissmall",
		"Otildesmall",
		"Uacutesmall",
		"Ugravesmall",
		"Ucircumflexsmall",
		"Udieresissmall",
		NULL,
		"eightsuperior",
		"fourinferior",
		"threeinferior",
		"sixinferior",
		"eightinferior",
		"seveninferior",
		"Scaronsmall",
		NULL,
		"centinferior",
		"twoinferior",
		NULL,
		"Dieresissmall",
		NULL,
		"Caronsmall",
		"osuperior",
		"fiveinferior",
		NULL,
		"commainferior",
		"periodinferior",
		"Yacutesmall",
		NULL,
		"dollarinferior",
		NULL,
		NULL,
		"Thornsmall",
		NULL,
		"nineinferior",
		"zeroinferior",
		"Zcaronsmall",
		"AEsmall",
		"Oslashsmall",
		"questiondownsmall",
		"oneinferior",
		"Lslashsmall",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"Cedillasmall",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"OEsmall",
		"figuredash",
		"hyphensuperior",
		NULL,
		NULL,
		NULL,
		NULL,
		"exclamdownsmall",
		NULL,
		"Ydieresissmall",
		NULL,
		"onesuperior",
		"twosuperior",
		"threesuperior",
		"foursuperior",
		"fivesuperior",
		"sixsuperior",
		"sevensuperior",
		"ninesuperior",
		"zerosuperior",
		NULL,
		"esuperior",
		"rsuperior",
		"tsuperior",
		NULL,
		NULL,
		"isuperior",
		"ssuperior",
		"dsuperior",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"lsuperior",
		"Ogoneksmall",
		"Brevesmall",
		"Macronsmall",
		"bsuperior",
		"nsuperior",
		"msuperior",
		"commasuperior",
		"periodsuperior",
		"Dotaccentsmall",
		"Ringsmall",
		NULL,
		NULL,
		NULL,
		NULL
	};

	const char *standard_map[256] =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"space",
		"exclam",
		"quotedbl",
		"numbersign",
		"dollar",
		"percent",
		"ampersand",
		"quoteright",
		"parenleft",
		"parenright",
		"asterisk",
		"plus",
		"comma",
		"hyphen",
		"period",
		"slash",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"colon",
		"semicolon",
		"less",
		"equal",
		"greater",
		"question",
		"at",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"bracketleft",
		"backslash",
		"bracketright",
		"asciicircum",
		"underscore",
		"quoteleft",
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"braceleft",
		"bar",
		"braceright",
		"asciitilde",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"exclamdown",
		"cent",
		"sterling",
		"fraction",
		"yen",
		"florin",
		"section",
		"currency",
		"quotesingle",
		"quotedblleft",
		"guillemotleft",
		"guilsinglleft",
		"guilsinglright",
		"fi",
		"fl",
		NULL,
		"endash",
		"dagger",
		"daggerdbl",
		"periodcentered",
		NULL,
		"paragraph",
		"bullet",
		"quotesinglbase",
		"quotedblbase",
		"quotedblright",
		"guillemotright",
		"ellipsis",
		"perthousand",
		NULL,
		"questiondown",
		NULL,
		"grave",
		"acute",
		"circumflex",
		"tilde",
		"macron",
		"breve",
		"dotaccent",
		"dieresis",
		NULL,
		"ring",
		"cedilla",
		NULL,
		"hungarumlaut",
		"ogonek",
		"caron",
		"emdash",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"AE",
		NULL,
		"ordfeminine",
		NULL,
		NULL,
		NULL,
		NULL,
		"Lslash",
		"Oslash",
		"OE",
		"ordmasculine",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"ae",
		NULL,
		NULL,
		NULL,
		"dotlessi",
		NULL,
		NULL,
		"lslash",
		"oslash",
		"oe",
		"germandbls",
		NULL,
		NULL,
		NULL,
		NULL
	};
}

/**	constructor
	
*/
BuildInEncoding::BuildInEncoding( const Name& name )
	: m_enc( Parse( name ) )
{
	static const char **map[] =
	{
		winansi_map,
		mac_roman_map,
		mac_expert_map,
		standard_map
	} ;
	
	if ( m_enc >= win && m_enc <= standard )
	{
		Build( map[m_enc] ) ;
	}
}

void BuildInEncoding::Build( const char **map )
{
	for ( unsigned short i = 0 ; i < 256U ; ++i )
	{
		wchar_t unicode = 0 ;
		if ( map[i] != 0 && NameToUnicode( map[i], unicode ) )
			Add( i, unicode ) ;
	}
}

BuildInEncoding::Encoding BuildInEncoding::Parse( const Name& name )
{
	if ( name == "WinAnsiEncoding" )
		return win ;
	else if ( name == "MacRomanEncoding" )
		return mac_roman ;
	else if ( name == "MacExpertEncoding" )
		return mac_expert ;
	else if ( name == "StandardEncoding" )
		return standard ;
	else
		throw FontException( "invalid encoding" ) ;
}

Object BuildInEncoding::Write( File * ) const
{
	PDF_ASSERT( m_enc >= win && m_enc <= standard ) ;
	static const Name names[] =
	{
		"WinAnsiEncoding",
		"MacRomanEncoding",
		"MacExpertEncoding",
		"StandardEncoding"
	} ;
	return names[m_enc] ;
}

} // end of namespace
