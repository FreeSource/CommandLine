/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: CommandLineImpl.cpp
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

#include <algorithm>
#include <stdexcept>

#include <charseq.h>
#include <system.h>

namespace {
    const int OPTION_NOT_FOUND = -1;
}

namespace environs {
    
    using namespace util;
    using std::runtime_error;
    
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
    
    const string CommandLine::CommandLineImpl::getCurrentWorkingDirectory() const {
        try {
            return getCurrentDirectory();
        }
        catch ( runtime_error &error ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const int CommandLine::CommandLineImpl::getParametersNumber() const {
        return parameters.size();
    }
    
    const string CommandLine::CommandLineImpl::getParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? parameters.at( position - 1 ) : "";
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
}
