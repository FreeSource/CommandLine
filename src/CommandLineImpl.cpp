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
#include <stdlib.h>

#include <charseq.h>
#include <system.h>

namespace {
    using namespace util;
    using namespace crosslib;
    using std::runtime_error;
    
    const int OPTION_NOT_FOUND = -1;
}

namespace environs {
    
    CommandLineImpl::CommandLineImpl() {
        try {
            currentPosition = 0;
            caseSensitiveMode = true;
            convert = &CommandLineImpl::noConvert;
            
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
    
    const string CommandLineImpl::getCommandLine() const {
        return applicationFullPath + " " + getAllParameters();
    }
    
    const string CommandLineImpl::getAllParameters() const {
        string parameters;
        for ( unsigned index = 0; index < this->parameters.size(); ++index ) {
            parameters.append( this->parameters.at( index ) );
            parameters.append( index + 1 == this->parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const string CommandLineImpl::getApplicationName() const {
        return applicationFullPath.substr( applicationFullPath.find_last_of( "/\\" ) + 1U );
    }
    
    const string CommandLineImpl::getApplicationPath() const {
        return applicationFullPath.substr( 0, applicationFullPath.find_last_of( "/\\" ) );
    }
    
    const string CommandLineImpl::getApplicationFullPath() const {
        return applicationFullPath;
    }
    
    const string CommandLineImpl::getCurrentWorkingDirectory() const {
        try {
            return getCurrentDirectory();
        }
        catch ( runtime_error &error ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const bool CommandLineImpl::hasParameters() const {
        return parameters.empty() ? false : true;
    }
    
    const bool CommandLineImpl::hasParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? true : false;
    }
    
    const int CommandLineImpl::getParametersNumber() const {
        return parameters.size();
    }
    
    const string CommandLineImpl::getParameterByPosition( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? parameters.at( position - 1 ) : "";
    }
    
    const int CommandLineImpl::getParameterByPositionAsInteger( const unsigned &position ) const {
        return atoi( getParameterByPosition( position ).c_str() );
    }
    
    const double CommandLineImpl::getParameterByPositionAsFloat( const unsigned &position ) const {
        return atof( getParameterByPosition( position ).c_str() );
    }
    
    void CommandLineImpl::gotoFirstParameter() {
        currentPosition = 0;
    }
    
    const bool CommandLineImpl::gotoNextParameter() {
        if ( currentPosition + 1 < parameters.size() ) {
            ++currentPosition;
            return true;
        }
        return false;
    }
    
    const int CommandLineImpl::getCurrentPosition() const {
        return parameters.size() ? currentPosition + 1 : 0;
    }
    
    const string CommandLineImpl::getCurrentParameter() const {
        return getParameterByPosition( currentPosition + 1 );
    }
    
    const int CommandLineImpl::getCurrentParameterAsInteger() const {
        return getParameterByPositionAsInteger( currentPosition + 1 );
    }
    
    const double CommandLineImpl::getCurrentParameterAsFloat() const {
        return getParameterByPositionAsFloat( currentPosition + 1 );
    }
    
    const string CommandLineImpl::getFirstParameter() const {
        return getParameterByPosition( 1 );
    }
    
    const int CommandLineImpl::getFirstParameterAsInteger() const {
        return getParameterByPositionAsInteger( 1 );
    }
    
    const double CommandLineImpl::getFirstParameterAsFloat() const {
        return getParameterByPositionAsFloat( 1 );
    }
    
    const string CommandLineImpl::getLastParameter() const {
        return getParameterByPosition( parameters.size() );
    }
    
    const int CommandLineImpl::getLastParameterAsInteger() const {
        return getParameterByPositionAsInteger( parameters.size() );
    }
    
    const double CommandLineImpl::getLastParameterAsFloat() const {
        return getParameterByPositionAsFloat( parameters.size() );
    }
    
    void CommandLineImpl::setOptionPrefix( const string &optionPrefix ) {
        postfixed = false;
        this->optionPrefix = optionPrefix;
        optionParameters = parameters;
    }
    
    void CommandLineImpl::setOptionPostfix( const string &optionPostfix ) {
        if ( !optionPostfix.empty() ) {
            postfixed = true;
            this->optionPrefix = optionPostfix;
            convertOptionPostfixToPrefix();
        }
    }
    
    void CommandLineImpl::convertOptionPostfixToPrefix() {
        vector<string> parameters = split( prefixAndPostfixOptionPostfixWithWhitespace( getAllParameters() ), " " );
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
    
    const string CommandLineImpl::prefixAndPostfixOptionPostfixWithWhitespace( string parameters ) const {
        size_t found = parameters.find( optionPrefix, 0 );
        while ( found != string::npos ) {
            parameters.replace( parameters.find( optionPrefix, found ), optionPrefix.size(), " " + optionPrefix + " " );
            found += 2 + optionPrefix.size();
            found = parameters.find( optionPrefix, found );
        }
        return parameters;
    }
    
    const vector<string> CommandLineImpl::removeNullElement( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( parameters.at( position ).empty() ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const vector<string> CommandLineImpl::removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position + 1 < parameters.size() && parameters.at( position ) == optionPrefix && parameters.at( position + 1 ) == optionPrefix ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const string CommandLineImpl::getOptionPrefix() const {
        return postfixed ? "" : optionPrefix;
    }
    
    const string CommandLineImpl::getOptionPostfix() const {
        return postfixed ? optionPrefix : "";
    }
    
    const bool CommandLineImpl::hasOption( const string &option ) {
        return findOptionPosition( option ) != OPTION_NOT_FOUND ? true : false;
    }
    
    const int CommandLineImpl::getOptionValueAsInteger( const string &option ) {
        return atoi( getOptionValue( option ).c_str() );
    }
    
    const double CommandLineImpl::getOptionValueAsFloat( const string &option ) {
        return atof( getOptionValue( option ).c_str() );
    }
    
    const string CommandLineImpl::getOptionValue( const string &option ) {
        int position = findOptionPosition( option );
        if ( position != OPTION_NOT_FOUND && unsigned( ++position ) < optionParameters.size() ) {
            if ( optionPrefix.empty() ) {
                return optionParameters.at( position );
            }
            
            size_t found = optionParameters.at( position ).find( optionPrefix );
            
            if ( found == string::npos || found > 0 ) {
                return optionParameters.at( position );
            }
        }
        return "";
    }
    
    const string CommandLineImpl::getOptionLongValue( const string &option ) {
        int position = findOptionPosition( option );
        string parameters;
        if ( position != OPTION_NOT_FOUND ) {
            size_t found;
            for ( ++position; unsigned( position ) < optionParameters.size(); ++position ) {
                found = optionPrefix.empty() || optionParameters.at( position ) == optionPrefix ? string::npos : optionParameters.at( position ).find( optionPrefix );
                
                if ( found == string::npos || found > 0 ) {
                    parameters += optionParameters.at( position );
                } else {
                    break;
                }
                
                if ( unsigned( position + 1 ) < optionParameters.size() ) {
                    parameters += " ";
                }
            }
        }
        trim( parameters );
        return parameters;
    }
    
    const int CommandLineImpl::findOptionPosition( string option ) {
        if ( !option.empty() ) {
            (this->*convert)( option );
            
            string parameter;
            for ( unsigned position = 0; position < optionParameters.size(); ++position ) {
                parameter = optionParameters.at( position );
                
                (this->*convert)( parameter );
                if ( optionPrefix + option == parameter ) {
                    return position;
                }
            }
        }
        return OPTION_NOT_FOUND;
    }
    
    void CommandLineImpl::setOptionCaseSensitive() {
        caseSensitiveMode = true;
        convert = &CommandLineImpl::noConvert;
    }
    
    void CommandLineImpl::noConvert( string &text ) {}
    
    void CommandLineImpl::setOptionCaseInsensitive() {
        caseSensitiveMode = false;
        convert = &CommandLineImpl::tolower;
    }
    
    void CommandLineImpl::tolower( string &text ) {
        transform( text.begin(), text.end(), text.begin(), ::tolower );
    }
    
    const bool CommandLineImpl::isOptionCaseSensitive() const {
        return caseSensitiveMode;
    }
}
