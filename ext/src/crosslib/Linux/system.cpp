/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: compatibility_Linux.cpp
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

#include <system.h>

#include <stdexcept>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

namespace crosslib {
    
    using std::runtime_error;
    
    const string getExecutablePath() {
        char path[PATH_MAX];
        if ( realpath( program_invocation_name, path ) ) {
            return path;
        }
        else {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process executable path." );
        }
    }
    
    const vector<string> getArguments() {
        FILE *cmdline = fopen( "/proc/self/cmdline", "rb" );
        
        if ( cmdline == NULL ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );
        }
        
        char *arg = NULL;
        size_t size = 0;
        vector<string> args;
        
        while ( getdelim( &arg, &size, 0, cmdline ) != -1 ) {
            args.push_back( arg );
        }
        
        if ( arg != NULL ) {
            free( arg );
        }
        
        fclose( cmdline );
        args.erase( args.begin() );
        return args;
    }
    
    const string getCurrentDirectory() {
        char currentDir[PATH_MAX];
        
        if ( getcwd( currentDir, PATH_MAX ) != NULL ) {
            return currentDir;
        }
        else {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process current working directory." );
        }
    }
}