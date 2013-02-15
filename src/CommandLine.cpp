/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: CommandLine.cpp
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

#include <CommandLineImpl.h>

namespace environs {
    
    CommandLine::CommandLine() {
        commandLine = new CommandLineImpl();
    }
    
    CommandLine::~CommandLine() {
        delete commandLine;
    }
    
    const string CommandLine::getApplicationName() const {
        return commandLine->getApplicationName();
    }
    
    const string CommandLine::getApplicationPath() const {
        return commandLine->getApplicationPath();
    }
    
    const string CommandLine::getCurrentWorkingDirectory() const {
        return commandLine->getCurrentWorkingDirectory();
    }
    
    const int CommandLine::getParametersNumber() const {
        return commandLine->getParametersNumber();
    }
    
    const string CommandLine::getParameter( const unsigned &position ) const {
        return commandLine->getParameter( position );
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) {
        commandLine->setOptionPrefix( optionPrefix );
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) {
        commandLine->setOptionPostfix( optionPostfix );
    }
    
    const bool CommandLine::hasOption( const string &option ) const {
        return commandLine->hasOption( option );
    }
    
    const string CommandLine::getOptionValue( const string &option ) const {
        return commandLine->getOptionValue( option );
    }
    
    const string CommandLine::getOptionLongValue( const string &option ) const {
        return commandLine->getOptionLongValue( option );
    }
    
    void CommandLine::optionCaseSensitive() {
        commandLine->optionCaseSensitive();
    }
    
    void CommandLine::optionCaseInsensitive() {
        commandLine->optionCaseInsensitive();
    }
}
