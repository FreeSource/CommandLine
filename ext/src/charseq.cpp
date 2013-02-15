/*--------------------------------------------------------------------------
   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: util.cpp
    Version: 1.0.0
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

#include <charseq.h>

#include <sstream>

namespace util {
    
    using std::istringstream;
    
    const string removeDuplicates( string text, const string &duplicate ) {
        while ( text.find( duplicate + duplicate ) != string::npos ) {
            text.replace( text.find( duplicate + duplicate ), duplicate.length() * 2, duplicate );
        }
        return text;
    }
    
    const string convertDelimiterToPipe( string text, const string &delimiter ) {
        size_t pos = 0;
        pos = text.find( delimiter, pos );
        
        while ( pos != string::npos ) {
            text.replace( pos, delimiter.size(), "|" );
            ++pos;
            pos = text.find( delimiter, pos );
        }
        return text;
    }
    
    const vector<string> split( string text, const string &delimiter ) {
        vector<string> stringArray;
        if ( text.find( delimiter ) != string::npos ) {
            text = removeDuplicates( text, delimiter );
            text = convertDelimiterToPipe( text, delimiter );
            
            string buffer;
            istringstream str( text );
            while ( getline( str, buffer, '|' ) ) {
                stringArray.push_back( buffer );
            }
        }
        else {
            stringArray.push_back( text );
        }
        return stringArray;
    }
    
    const string trim( string text ) {
        static const char whitespace[] = " \n\t\v\r\f";
        text.erase( 0, text.find_first_not_of( whitespace ) );
        return text.erase( text.find_last_not_of( whitespace ) + 1U );
    }
}
