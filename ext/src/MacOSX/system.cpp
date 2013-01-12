/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: compatibility_MacOSX.cpp
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
#include <libproc.h>

#include <util.h>

using std::runtime_error;

namespace {
    
    const int getMaxArgumentSize();
    void readArguments( char *procargs );
    
    const int getMaxArgumentSize() {
        int mib[3];
        mib[0] = CTL_KERN;
        mib[1] = KERN_ARGMAX;
        
        int argmax;
        size_t size = sizeof( argmax );
        if ( sysctl( mib, 2, &argmax, &size, NULL, 0 ) == -1 ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );
        }
        return argmax;
    }
    
    void readArguments( char *procargs ) {
        if ( procargs == NULL ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );
        }
        int mib[3];
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROCARGS2;
        mib[2] = getpid();
        
        size_t size = ( size_t )getMaxArgumentSize();
        if ( sysctl(mib, 3, procargs, &size, NULL, 0) == -1 ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process arguments." );
        }
    }
}

const string getExecutablePath() {
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    return proc_pidpath( getpid(), pathbuf, sizeof( pathbuf ) ) ? pathbuf : "";
}

const vector<string> getArguments() {
    char *procargs = ( char * )malloc( getMaxArgumentSize() );
    readArguments( procargs );
    
    int nargs;
    size_t size = ( size_t )getMaxArgumentSize();
    memcpy( &nargs, procargs, sizeof( nargs ) );
    char *ptArgs;
    ptArgs = procargs + sizeof( nargs );
    
    bool flg = false;
    for ( ; ptArgs < &procargs[size]; ptArgs++ ) {
        if ( flg == true && *ptArgs != '\0' ) {
            break;
        }
        else if ( flg == false && *ptArgs == '\0' ) {
            flg = true;
        }
    }
    
    string concatArgs;
    for ( int cArgs = 0; cArgs < nargs && ptArgs < &procargs[size]; ptArgs++ ) {
        if ( *ptArgs == '\0' ) {
            cArgs++;
            concatArgs += ' ';
        }
        else {
            concatArgs += *ptArgs;
        }
    }
    
    vector<string> args;
    args = split( concatArgs, " " );
    if ( args.size() ) {
        args.erase( args.begin() );
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
