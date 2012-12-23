/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: compatibility_Windows.cpp
    Version: 2.0.0
    Copyright: (C) 2012 by Enzo Roberto Verlato
    Contact: enzover@ig.com.br
    All rights reserved.
    
----------------------------------------------------------------------------
    This file is part of the compatibility library.
    
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

#include <compatibility.h>

#include <stdexcept>
#include <windows.h>

using std::runtime_error;
using std::wstring;

const string getExecutablePath() {
    char exepath[MAX_PATH];
    int bytes = GetModuleFileName( NULL, exepath, MAX_PATH );
    
    if ( bytes > 0 ) {
        return exepath;
    }
    else {
        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process executable path." );
    }
}

const vector<string> getArguments() {
    LPWSTR *parameterList;
    int parametersNumber = 0;
    parameterList = CommandLineToArgvW( GetCommandLineW(), &parametersNumber );
    
    if ( parameterList == NULL ) {
        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );
    }
    
    wstring parameter;
    vector<string> args;
    for ( int index = 1; index < parametersNumber; ++index ) {
        parameter = parameterList[index];
        args.push_back( string( parameter.begin(), parameter.end() ) );
    }
    LocalFree( parameterList );
    return args;
}

const string getCurrentDirectory() {
    char currentDir[MAX_PATH];
    if ( GetCurrentDirectory( MAX_PATH, currentDir ) > 0 ) {
        return currentDir;
    }
    else {
        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process current working directory." );
    }
}
