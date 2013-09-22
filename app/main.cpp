/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: main.cpp
    Version: 2.0.0
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

int main() {
    
    using namespace std;
    using namespace environs;
    
    try {
        CommandLine commandLine;
        string option;
        cout.precision( 9 );
        
        do {
            option = commandLine.getParameter( commandLine.getCurrentPosition() + 1 );
            
            if ( commandLine.getCurrentParameter() == "getParametersNumber" ) {
                cout << commandLine.getParametersNumber() << endl;

            } else if ( commandLine.getCurrentParameter() == "getCurrentPosition" ) {
                cout << commandLine.getCurrentPosition() << endl;

            } else if ( commandLine.getCurrentParameter() == "getCommandLine" ) {
                cout << commandLine.getCommandLine() << endl;

            } else if ( commandLine.getCurrentParameter() == "getApplicationName" ) {
                cout << commandLine.getApplicationName() << endl;

            } else if ( commandLine.getCurrentParameter() == "getApplicationPath" ) {
                cout << commandLine.getApplicationPath() << endl;

            } else if ( commandLine.getCurrentParameter() == "getApplicationFullPath" ) {
                cout << commandLine.getApplicationFullPath() << endl;

            } else if ( commandLine.getCurrentParameter() == "getCurrentWorkingDirectory" ) {
                cout << commandLine.getCurrentWorkingDirectory() << endl;

            } else if ( commandLine.getCurrentParameter() == "hasParameters" ) {
                commandLine.hasParameters() ? cout << "yes" << endl : cout << "no" << endl;

            } else if ( commandLine.getCurrentParameter() == "hasParameter" ) {
                commandLine.hasParameter( atoi( commandLine.getParameter( commandLine.getCurrentPosition() + 1 ).c_str() ) ) ? cout << "yes" << endl : cout << "no" << endl;

            } else if ( commandLine.getCurrentParameter() == "getParameter" ) {
                cout << commandLine.getParameter( atoi( commandLine.getParameter( commandLine.getCurrentPosition() + 1 ).c_str() ) ) << endl;

            } else if ( commandLine.getCurrentParameter() == "gotoFirstParameter" ) {
                cout << "Tested Yet..." << endl;

            } else if ( commandLine.getCurrentParameter() == "gotoNextParameter" ) {
                cout << "Tested Yet..." << endl;

            } else if ( commandLine.getCurrentParameter() == "getAllParameters" ) {
                cout << commandLine.getAllParameters() << endl;

            } else if ( commandLine.getCurrentParameter() == "getCurrentParameter" ) {
                cout << commandLine.getCurrentParameter() << endl;

            } else if ( commandLine.getCurrentParameter() == "getFirstParameter" ) {
                cout << commandLine.getFirstParameter() << endl;

            } else if ( commandLine.getCurrentParameter() == "getLastParameter" ) {
                cout << commandLine.getLastParameter() << endl;

            } else if ( commandLine.getCurrentParameter() == "setOptionPrefix" ) {
                commandLine.setOptionPrefix( commandLine.getParameter( commandLine.getCurrentPosition() + 1 ) );

            } else if ( commandLine.getCurrentParameter() == "setOptionPostfix" ) {
                commandLine.setOptionPostfix( commandLine.getParameter( commandLine.getCurrentPosition() + 1 ) );

            } else if ( commandLine.getCurrentParameter() == "hasOption" ) {
                commandLine.hasOption( option ) ? cout << "yes" << endl : cout << "no" << endl;

            } else if ( commandLine.getCurrentParameter() == "getOptionPrefix" ) {
                cout << commandLine.getOptionPrefix() << endl;

            } else if ( commandLine.getCurrentParameter() == "getOptionPostfix" ) {
                cout << commandLine.getOptionPostfix() << endl;

            } else if ( commandLine.getCurrentParameter() == "getOptionValue" ) {
                cout << commandLine.getOptionValue( option ) << endl;

            } else if ( commandLine.getCurrentParameter() == "getOptionLongValue" ) {
                cout << commandLine.getOptionLongValue( option ) << endl;

            } else if ( commandLine.getCurrentParameter() == "optionCaseSensitive" ) {
                commandLine.optionCaseSensitive();

            } else if ( commandLine.getCurrentParameter() == "optionCaseInsensitive" ) {
                commandLine.optionCaseInsensitive();

            } else if ( commandLine.getCurrentParameter() == "isOptionCaseSensitive" ) {
                commandLine.isOptionCaseSensitive() ? cout << "yes" << endl : cout << "no" << endl;

            }
        } while ( commandLine.gotoNextParameter() );

        return EXIT_SUCCESS;

    } catch ( runtime_error &error ) {
        // Custom user messages...
        
        // Log messages...
        cout << "Exception occurred at " << error.what() << endl;
        
        return EXIT_FAILURE;
    }
}
