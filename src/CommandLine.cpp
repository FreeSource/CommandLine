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

#include <CommandLine.h>
#include <CommandLineImpl.h>

namespace {
    environs::CommandLineImpl* commandLine;
}

namespace environs {
    
    CommandLine::CommandLine() {
        commandLine = new CommandLineImpl();
    }
    
    CommandLine::~CommandLine() {
        delete commandLine;
    }
    
    const string CommandLine::getCommandLine() const {
        return commandLine->getCommandLine();
    }
    
    const string CommandLine::getAllParameters() const {
        return commandLine->getAllParameters();
    }
    
    const string CommandLine::getApplicationName() const {
        return commandLine->getApplicationName();
    }
    
    const string CommandLine::getApplicationPath() const {
        return commandLine->getApplicationPath();
    }
    
    const string CommandLine::getApplicationFullPath() const {
        return commandLine->getApplicationFullPath();
    }
    
    const string CommandLine::getCurrentWorkingDirectory() const {
        return commandLine->getCurrentWorkingDirectory();
    }
    
    const bool CommandLine::hasParameters() const {
        return commandLine->hasParameters();
    }
    
    const bool CommandLine::hasParameter( const unsigned &position ) const {
        return commandLine->hasParameter( position );
    }
    
    const int CommandLine::getParametersNumber() const {
        return commandLine->getParametersNumber();
    }
    
    const string CommandLine::getParameterByPosition( const unsigned &position ) const {
        return commandLine->getParameterByPosition( position );
    }
    
    const int CommandLine::getParameterByPositionAsInteger( const unsigned &position ) const {
        return commandLine->getParameterByPositionAsInteger( position );
    }
    
    const double CommandLine::getParameterByPositionAsFloat( const unsigned &position ) const {
        return commandLine->getParameterByPositionAsFloat( position );
    }
    
    void CommandLine::gotoFirstParameter() const {
        return commandLine->gotoFirstParameter();
    }
    
    const bool CommandLine::gotoNextParameter() const {
        return commandLine->gotoNextParameter();
    }
    
    const int CommandLine::getCurrentPosition() const {
        return commandLine->getCurrentPosition();
    }
    
    const string CommandLine::getCurrentParameter() const {
        return commandLine->getCurrentParameter();
    }
    
    const int CommandLine::getCurrentParameterAsInteger() const {
        return commandLine->getCurrentParameterAsInteger();
    }
    
    const double CommandLine::getCurrentParameterAsFloat() const {
        return commandLine->getCurrentParameterAsFloat();
    }
    
    const string CommandLine::getFirstParameter() const {
        return commandLine->getFirstParameter();
    }
    
    const int CommandLine::getFirstParameterAsInteger() const {
        return commandLine->getFirstParameterAsInteger();
    }
    
    const double CommandLine::getFirstParameterAsFloat() const {
        return commandLine->getFirstParameterAsFloat();
    }
    
    const string CommandLine::getLastParameter() const {
        return commandLine->getLastParameter();
    }
    
    const int CommandLine::getLastParameterAsInteger() const {
        return commandLine->getLastParameterAsInteger();
    }
    
    const double CommandLine::getLastParameterAsFloat() const {
        return commandLine->getLastParameterAsFloat();
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) const {
        commandLine->setOptionPrefix( optionPrefix );
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) const {
        commandLine->setOptionPostfix( optionPostfix );
    }
    
    const string CommandLine::getOptionPrefix() const {
        return commandLine->getOptionPrefix();
    }
    
    const string CommandLine::getOptionPostfix() const {
        return commandLine->getOptionPostfix();
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
    
    void CommandLine::setOptionCaseSensitive() const {
        commandLine->setOptionCaseSensitive();
    }
    
    void CommandLine::setOptionCaseInsensitive() const {
        commandLine->setOptionCaseInsensitive();
    }
    
    const bool CommandLine::isOptionCaseSensitive() const {
        return commandLine->isOptionCaseSensitive();
    }
}
