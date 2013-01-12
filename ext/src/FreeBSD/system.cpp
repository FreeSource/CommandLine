/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: compatibility_FreeBSD.cpp
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
#include <sys/sysctl.h>

using std::runtime_error;

const string getExecutablePath() {
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char buffer[PATH_MAX];
    size_t cb = sizeof( buffer );
    
    if( sysctl( mib, 4, buffer, &cb, NULL, 0 ) < 0 ) {
        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process executable path." );
    }
    return buffer;
}

const vector<string> getArguments() {
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_ARGS;
    mib[3] = getpid();
    
    char arg[ARG_MAX];
    size_t len;
    len = sizeof( arg );
    
    if ( sysctl( mib, 4, arg, &len, NULL, 0 ) < 0 ) {
        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );;
    }
    
    char *cp = arg;
    cp += strlen( cp ) + 1;
    vector<string> args;
    while ( cp < arg + len ) {
        args.push_back( cp );
        cp += strlen( cp ) + 1;
    }
    
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
