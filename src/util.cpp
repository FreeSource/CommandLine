/*--------------------------------------------------------------------------
   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: util.cpp
    Version: 1.0
    Copyright: (C) 2012 by Enzo Roberto Verlato
    Contact: enzover@ig.com.br
    All rights reserved.
    
----------------------------------------------------------------------------
    This file is part of the util library.
    
    This file may be used under the terms of the GNU General Public
    License version 2.0 as published by the Free Software Foundation
    and appearing in the file LICENSE.GPL2 included in the packaging of
    this file.
    
    This file is provided "AS IS" in the hope that it will be useful,
    but WITHOUT ANY WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF DESIGN;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
--------------------------------------------------------------------------*/

#include <util.h>

#include <sstream>

using std::istringstream;

const vector<string> split( string text, const string &token ) {
    vector<string> fields;
    
    if ( text.find( token ) != string::npos ) {
        while ( text.find( token ) != string::npos ) {
            text.replace( text.find( token ), token.length(), "|" );
        }
        
        string buffer;
        istringstream str( text );
        
        while ( getline( str, buffer, '|' ) ) {
            fields.push_back( buffer );
        }
    }
   
   return fields;
}

const string trim( string text ) {
    static const char whitespace[] = " \n\t\v\r\f";
    text.erase( 0, text.find_first_not_of( whitespace ) );
    return text.erase( text.find_last_not_of( whitespace ) + 1U );
}
