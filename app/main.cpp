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
using environs::CommandLine;

int main() {
    try {
        CommandLine commandLine;
        string option;
        
        for ( int parameterPosition = 0; parameterPosition <= commandLine.getParametersNumber(); ++parameterPosition ) {
            
            option = commandLine.getParameter( parameterPosition + 1 );
            
            if ( commandLine.getParameter( parameterPosition ) == "getParametersNumber" ) {
                cout << commandLine.getParametersNumber() << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getApplicationName" ) {
                cout << commandLine.getApplicationName() << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getApplicationPath" ) {
                cout << commandLine.getApplicationPath() << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getCurrentWorkingDirectory" ) {
                cout << commandLine.getCurrentWorkingDirectory() << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getParameter" ) {
                cout << commandLine.getParameter( atoi( commandLine.getParameter( parameterPosition + 1 ).c_str() ) ) << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "setOptionPrefix" ) {
                commandLine.setOptionPrefix( commandLine.getParameter( parameterPosition + 1 ) );
            }
            else if ( commandLine.getParameter( parameterPosition ) == "setOptionPostfix" ) {
                commandLine.setOptionPostfix( commandLine.getParameter( parameterPosition + 1 ) );
            }
            else if ( commandLine.getParameter( parameterPosition ) == "hasOption" ) {
                commandLine.hasOption( option ) ? cout << "yes" << endl : cout << "no" << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getOptionValue" ) {
                cout << commandLine.getOptionValue( option ) << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "getOptionLongValue" ) {
                cout << commandLine.getOptionLongValue( option ) << endl;
            }
            else if ( commandLine.getParameter( parameterPosition ) == "optionCaseSensitive" ) {
                commandLine.optionCaseSensitive();
            }
            else if ( commandLine.getParameter( parameterPosition ) == "optionCaseInsensitive" ) {
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
