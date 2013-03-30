/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: compatibility_Solaris.cpp
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
#include <procfs.h>
#include <fcntl.h>
#include <sys/types32.h>

using std::runtime_error;

namespace {
    
    const psinfo_t getProcessInfo();
    void readArguments( int &procFile, uintptr_t *arguments, psinfo_t &pinfo );
    const vector<string> collectArguments( int &procFile, uintptr_t *arguments, psinfo_t &pinfo );
    
    const psinfo_t getProcessInfo() {
        psinfo_t pinfo;
        int procFile = open( "/proc/self/psinfo", O_RDONLY );
        
        if ( procFile == -1 || read( procFile, &pinfo, sizeof( psinfo_t ) ) == -1 ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process information." );
        }
        
        close( procFile );
        return pinfo;
    }

    void readArguments( int &procFile, uintptr_t *arguments, psinfo_t &pinfo ) {
        procFile = open( "/proc/self/as", O_RDONLY );
        
        if ( procFile == -1 ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process address-space image." );
        }
        
        if ( pinfo.pr_dmodel == PR_MODEL_NATIVE ) {
            if ( pread( procFile, arguments, pinfo.pr_argc * sizeof( uintptr_t ), pinfo.pr_argv ) == -1 ) {
                throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process address-space image." );
            }
        }
        else {
            caddr32_t *Argvec32 = ( caddr32_t * )arguments;
            if ( pread( procFile, Argvec32, pinfo.pr_argc * sizeof( caddr32_t ), pinfo.pr_argv ) == -1 ) {
                throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process address-space image." );
            }
            
            for ( int index = pinfo.pr_argc - 1; index >= 0; --index ) {
                arguments[index] = Argvec32[index];
            }
        }
    }

    const vector<string> collectArguments( int &procFile, uintptr_t *arguments, psinfo_t &pinfo ) {
        vector<string> args;
        size_t argLength = 16;
        char *argument = ( char* )malloc( argLength + 1 );
        
        if ( argument != NULL ) {
            for ( int index = 1; index < pinfo.pr_argc; ++index ) {
                if ( pread( procFile, argument, argLength, arguments[index] ) == -1 ) {
                    throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't collect process arguments." );
                }
                
                if ( strlen( argument ) == argLength ) {
                    argument = ( char* )realloc( argument, ( argLength *= 2 ) + 1 );
                    if ( argument == NULL ) {
                        throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't collect process arguments." );
                    }
                    --index;
                    continue;
                }
                args.push_back( argument );
            }
            free( argument );
        }
        return args;
    }
}

namespace crosslib {
    
    const string getExecutablePath() {
        char path[PATH_MAX];
        if ( realpath( getexecname(), path ) ) {
            return path;
        }
        else {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + "Can't get process executable path." );
        }
    }
    
    const vector<string> getArguments() {
        int procFile;
        vector<string> args;
        psinfo_t pinfo = getProcessInfo();
        
        uintptr_t *arguments = ( uintptr_t* )malloc( pinfo.pr_argc * sizeof ( uintptr_t ) );
        readArguments( procFile, arguments, pinfo );
        args = collectArguments( procFile, arguments, pinfo );

        close( procFile );
        free( arguments );
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
