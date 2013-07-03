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

int main() {
    
    using std::string;
    using std::cout;
    using std::endl;
    using std::runtime_error;
    using environs::CommandLine;
    
    try {
        CommandLine commandLine;
        string option;
        
        for ( int position = 0; position <= commandLine.getParametersNumber(); ++position ) {
            
            option = commandLine.getParameter( position + 1 );
            
            if ( commandLine.getParameter( position ) == "getParametersNumber" ) {
                cout << commandLine.getParametersNumber() << endl;
                
            } else if ( commandLine.getParameter( position ) == "getApplicationName" ) {
                cout << commandLine.getApplicationName() << endl;
                
            } else if ( commandLine.getParameter( position ) == "getApplicationPath" ) {
                cout << commandLine.getApplicationPath() << endl;
                
            } else if ( commandLine.getParameter( position ) == "getCurrentWorkingDirectory" ) {
                cout << commandLine.getCurrentWorkingDirectory() << endl;
                
            } else if ( commandLine.getParameter( position ) == "getParameter" ) {
                cout << commandLine.getParameter( atoi( commandLine.getParameter( position + 1 ).c_str() ) ) << endl;
                
            } else if ( commandLine.getParameter( position ) == "setOptionPrefix" ) {
                commandLine.setOptionPrefix( commandLine.getParameter( position + 1 ) );
                
            } else if ( commandLine.getParameter( position ) == "setOptionPostfix" ) {
                commandLine.setOptionPostfix( commandLine.getParameter( position + 1 ) );
                
            } else if ( commandLine.getParameter( position ) == "hasOption" ) {
                commandLine.hasOption( option ) ? cout << "yes" << endl : cout << "no" << endl;
                
            } else if ( commandLine.getParameter( position ) == "getOptionValue" ) {
                cout << commandLine.getOptionValue( option ) << endl;
                
            } else if ( commandLine.getParameter( position ) == "getOptionLongValue" ) {
                cout << commandLine.getOptionLongValue( option ) << endl;
                
            } else if ( commandLine.getParameter( position ) == "optionCaseSensitive" ) {
                commandLine.optionCaseSensitive();
                
            } else if ( commandLine.getParameter( position ) == "optionCaseInsensitive" ) {
                commandLine.optionCaseInsensitive();
            }
        }
        return EXIT_SUCCESS;
        
    } catch ( runtime_error &error ) {
        // Custom user messages...
        
        // Log messages...
        cout << "Exception occurred at " << error.what() << endl;
        
        return EXIT_FAILURE;
    }
}
