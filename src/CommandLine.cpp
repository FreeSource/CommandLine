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

#include <vector>
#include <algorithm>
#include <stdexcept>

#include <util.h>
#include <system.h>

namespace {
    const int OPTION_NOT_FOUND = -1;
}

namespace environs {
    
    using std::vector;
    using std::runtime_error;
    
    class CommandLine::CommandLineImpl {
        public:
            CommandLineImpl();
            
            const string getCommandLine() const;
            
            const string getApplicationName() const;
            const string getApplicationPath() const;
            const string getApplicationFullPath() const;
            
            const string getCurrentWorkingDirectory() const;
            
            const bool hasParameters() const;
            const bool hasParameter( const unsigned &position ) const;
            
            const int getParametersNumber() const;
            const string getAllParameters() const;
            const string getParameter( const unsigned &position ) const;
            
            void gotoFirstParameter();
            const bool gotoNextParameter();
            const int getCurrentPosition() const;
            
            const string getCurrentParameter() const;
            
            const string getFirstParameter() const;
            const string getLastParameter() const;
            
            void setOptionPrefix( const string &optionPrefix );
            void setOptionPostfix( const string &optionPostfix );
            
            const string getOptionPrefix() const;
            const string getOptionPostfix() const;
            
            const bool hasOption( const string &option ) const;
            
            const string getOptionValue( const string &option ) const;
            const string getOptionLongValue( const string &option ) const;
            
            void optionCaseSensitive();
            void optionCaseInsensitive();
            const bool isOptionCaseSensitive() const;
            
        private:
            string applicationFullPath;
            vector<string> parameters;
            vector<string> optionParameters;
            
            unsigned currentPosition;
            bool caseSensitiveMode;
            string optionPrefix;
            bool postfixed;
            
            void convertOptionPostfixToPrefix();
            const int findOptionPosition( string option ) const;
            const string prefixAndPostfixOptionPostfixWhithWhitespace( string parameters ) const;
            const vector<string> removeNullElement( vector<string> parameters ) const;
            const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) const;
    };
    
    CommandLine::CommandLineImpl::CommandLineImpl() {
        try {
            currentPosition = 0;
            caseSensitiveMode = true;
            
            applicationFullPath = getExecutablePath();
            parameters = getArguments();
            optionParameters = parameters;
        }
        catch ( runtime_error &error ) {
            // Clean up all previous efforts...
            applicationFullPath.clear();
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const string CommandLine::CommandLineImpl::getCommandLine() const {
        return applicationFullPath + " " + getAllParameters();
    }
    
    const string CommandLine::CommandLineImpl::getAllParameters() const {
        string parameters;
        for ( unsigned index = 0; index < this->parameters.size(); ++index ) {
            parameters.append( this->parameters.at( index ) );
            parameters.append( index + 1 == this->parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const string CommandLine::CommandLineImpl::getApplicationName() const {
        return applicationFullPath.substr( applicationFullPath.find_last_of( "/\\" ) + 1U );
    }
    
    const string CommandLine::CommandLineImpl::getApplicationPath() const {
        return applicationFullPath.substr( 0, applicationFullPath.find_last_of( "/\\" ) );
    }
    
    const string CommandLine::CommandLineImpl::getApplicationFullPath() const {
        return applicationFullPath;
    }
    
    const string CommandLine::CommandLineImpl::getCurrentWorkingDirectory() const {
        try {
            return getCurrentDirectory();
        }
        catch ( runtime_error &error ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const bool CommandLine::CommandLineImpl::hasParameters() const {
        return parameters.empty() ? false : true;
    }
    
    const bool CommandLine::CommandLineImpl::hasParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? true : false;
    }
    
    const int CommandLine::CommandLineImpl::getParametersNumber() const {
        return parameters.size();
    }
    
    const string CommandLine::CommandLineImpl::getParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? parameters.at( position - 1 ) : "";
    }
    
    void CommandLine::CommandLineImpl::gotoFirstParameter() {
        currentPosition = 0;
    }
    
    const bool CommandLine::CommandLineImpl::gotoNextParameter() {
        if ( currentPosition + 1 < parameters.size() ) {
            ++currentPosition;
            return true;
        }
        return false;
    }
    
    const int CommandLine::CommandLineImpl::getCurrentPosition() const {
        return parameters.size() ? currentPosition + 1 : 0;
    }
    
    const string CommandLine::CommandLineImpl::getCurrentParameter() const {
        return getParameter( currentPosition + 1 );
    }
    
    const string CommandLine::CommandLineImpl::getFirstParameter() const {
        return getParameter( 1 );
    }
    
    const string CommandLine::CommandLineImpl::getLastParameter() const {
        return getParameter( parameters.size() );
    }
    
    void CommandLine::CommandLineImpl::setOptionPrefix( const string &optionPrefix ) {
        postfixed = false;
        this->optionPrefix = optionPrefix;
        optionParameters = parameters;
    }
    
    void CommandLine::CommandLineImpl::setOptionPostfix( const string &optionPostfix ) {
        if ( !optionPostfix.empty() ) {
            postfixed = true;
            this->optionPrefix = optionPostfix;
            convertOptionPostfixToPrefix();
        }
    }
    
    void CommandLine::CommandLineImpl::convertOptionPostfixToPrefix() {
        vector<string> parameters = split( prefixAndPostfixOptionPostfixWhithWhitespace( getAllParameters() ), " " );
        parameters = removeNullElement( parameters );
        parameters = removeOptionPostfixDuplicityBetweenOptionAndValue( parameters );
        
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position > 0 && parameters.at( position ) == optionPrefix ) {
                if ( parameters.at( position - 1 ).find( optionPrefix ) == string::npos ) {
                    parameters.at( position - 1 ) = optionPrefix + parameters.at( position - 1 );
                }
                
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        optionParameters = parameters;
    }
    
    const string CommandLine::CommandLineImpl::prefixAndPostfixOptionPostfixWhithWhitespace( string parameters ) const {
        size_t found = parameters.find( optionPrefix, 0 );
        while ( found != string::npos ) {
            parameters.replace( parameters.find( optionPrefix, found ), optionPrefix.size(), " " + optionPrefix + " " );
            found += 2 + optionPrefix.size();
            found = parameters.find( optionPrefix, found );
        }
        return parameters;
    }
    
    const vector<string> CommandLine::CommandLineImpl::removeNullElement( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( parameters.at( position ).empty() ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const vector<string> CommandLine::CommandLineImpl::removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position + 1 < parameters.size() && parameters.at( position ) == optionPrefix && parameters.at( position + 1 ) == optionPrefix ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const string CommandLine::CommandLineImpl::getOptionPrefix() const {
        return postfixed ? "" : optionPrefix;
    }
    
    const string CommandLine::CommandLineImpl::getOptionPostfix() const {
        return postfixed ? optionPrefix : "";
    }
    
    const bool CommandLine::CommandLineImpl::hasOption( const string &option ) const {
        return findOptionPosition( option ) != OPTION_NOT_FOUND ? true : false;
    }
    
    const string CommandLine::CommandLineImpl::getOptionValue( const string &option ) const {
        int index = findOptionPosition( option );
        if ( index != OPTION_NOT_FOUND && unsigned( ++index ) < optionParameters.size() ) {
            if ( optionPrefix.empty() ) {
                return optionParameters.at( index );
            }
            
            size_t found = optionParameters.at( index ).find( optionPrefix );
            
            if ( found == string::npos || found > 0 ) {
                return optionParameters.at( index );
            }
        }
        return "";
    }
    
    const string CommandLine::CommandLineImpl::getOptionLongValue( const string &option ) const {
        int index = findOptionPosition( option );
        string parameters;
        if ( index != OPTION_NOT_FOUND ) {
            size_t found;
            for ( ++index; unsigned( index ) < optionParameters.size(); ++index ) {
                found = optionPrefix.empty() || optionParameters.at( index ) == optionPrefix ? string::npos : optionParameters.at( index ).find( optionPrefix );
                
                if ( found == string::npos || found > 0 ) {
                    parameters += optionParameters.at( index );
                }
                else {
                    break;
                }
                
                if ( unsigned( index + 1 ) < optionParameters.size() ) {
                    parameters += " ";
                }
            }
        }
        return trim( parameters );
    }
    
    const int CommandLine::CommandLineImpl::findOptionPosition( string option ) const {
        if ( !option.empty() ) {
            if ( !caseSensitiveMode ) {
                transform( option.begin(), option.end(), option.begin(), ::tolower );
            }
            
            string parameter;
            for ( unsigned index = 0; index < optionParameters.size(); ++index ) {
                parameter = optionParameters.at( index );
                
                if ( !caseSensitiveMode ) {
                    transform( parameter.begin(), parameter.end(), parameter.begin(), ::tolower );
                }
                
                if ( optionPrefix + option == parameter ) {
                    return index;
                }
            }
        }
        return OPTION_NOT_FOUND;
    }
    
    void CommandLine::CommandLineImpl::optionCaseSensitive() {
        caseSensitiveMode = true;
    }
    
    void CommandLine::CommandLineImpl::optionCaseInsensitive() {
        caseSensitiveMode = false;
    }
    
    const bool CommandLine::CommandLineImpl::isOptionCaseSensitive() const {
        return caseSensitiveMode;
    }
    
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
    
    const string CommandLine::getParameter( const unsigned &position ) const {
        return commandLine->getParameter( position );
    }
    
    void CommandLine::gotoFirstParameter() {
        commandLine->gotoFirstParameter();
    }
    
    const bool CommandLine::gotoNextParameter() {
        return commandLine->gotoNextParameter();
    }
    
    const int CommandLine::getCurrentPosition() const {
        return commandLine->getCurrentPosition();
    }
    
    const string CommandLine::getCurrentParameter() const {
        return commandLine->getCurrentParameter();
    }
    
    const string CommandLine::getFirstParameter() const {
        return commandLine->getFirstParameter();
    }
    
    const string CommandLine::getLastParameter() const {
        return commandLine->getLastParameter();
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) {
        commandLine->setOptionPrefix( optionPrefix );
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) {
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
    
    void CommandLine::optionCaseSensitive() {
        commandLine->optionCaseSensitive();
    }
    
    void CommandLine::optionCaseInsensitive() {
        commandLine->optionCaseInsensitive();
    }
    
    const bool CommandLine::isOptionCaseSensitive() const {
        return commandLine->isOptionCaseSensitive();
    }
}
