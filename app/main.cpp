/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: main.cpp
    Version: 1.0.1
    Copyright: (C) 2012 by Enzo Roberto Verlato
    Contact: enzover@ig.com.br
    All rights reserved.
    
----------------------------------------------------------------------------
    This file is part of the CommandLine Class.
    
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

#include <CommandLine.h>

#include <iostream>
#include <cstdlib>
#include <stdexcept>

using std::string;
using std::cout;
using std::endl;
using std::runtime_error;

int main() {
    try {
        environs::CommandLine commandLine;
        string option;
        cout.precision( 9 );
        
        for ( int index = 0; index <= commandLine.getParametersNumber(); ++index )
        {
            option = commandLine.getParameter( index + 1 );
            
            if ( commandLine.getParameter( index ) == "getParametersNumber" ) {
                cout << commandLine.getParametersNumber() << endl;
            }
            else if ( commandLine.getParameter( index ) == "getApplicationName" ) {
                cout << commandLine.getApplicationName() << endl;
            }
            else if ( commandLine.getParameter( index ) == "getApplicationPath" ) {
                cout << commandLine.getApplicationPath() << endl;
            }
            else if ( commandLine.getParameter( index ) == "getCurrentWorkingDirectory" ) {
                cout << commandLine.getCurrentWorkingDirectory() << endl;
            }
            else if ( commandLine.getParameter( index ) == "getParameter" ) {
                cout << commandLine.getParameter( atoi( commandLine.getParameter( index + 1 ).c_str() ) ) << endl;
            }
            else if ( commandLine.getParameter( index ) == "setOptionPrefix" ) {
                commandLine.setOptionPrefix( commandLine.getParameter( index + 1 ) );
            }
            else if ( commandLine.getParameter( index ) == "setOptionPostfix" ) {
                commandLine.setOptionPostfix( commandLine.getParameter( index + 1 ) );
            }
            else if ( commandLine.getParameter( index ) == "hasOption" ) {
                commandLine.hasOption( option ) ? cout << "yes" << endl : cout << "no" << endl;
            }
            else if ( commandLine.getParameter( index ) == "getOptionValue" ) {
                cout << commandLine.getOptionValue( option ) << endl;
            }
            else if ( commandLine.getParameter( index ) == "getOptionLongValue" ) {
                cout << commandLine.getOptionLongValue( option ) << endl;
            }
            else if ( commandLine.getParameter( index ) == "optionCaseSensitive" ) {
                commandLine.optionCaseSensitive();
            }
            else if ( commandLine.getParameter( index ) == "optionCaseInsensitive" ) {
                commandLine.optionCaseInsensitive();
            }
        }
        return EXIT_SUCCESS;
    }
    catch ( runtime_error &error ) {
        // Custom user messages...
        
        // Log messages...
        cout << "Exception occurred at " << error.what() << endl;
        
        return EXIT_FAILURE;
    }
}
