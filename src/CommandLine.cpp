/*--------------------------------------------------------------------------
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
    
    File: CommandLine.cpp
    Version: 1.0
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

#include <algorithm>
#include <stdexcept>

#include <util.h>
#include <compatibility.h>

namespace {
    const int OPTION_NOT_FOUND = -1;
}

namespace util {
    
    using std::runtime_error;
    
    CommandLine::CommandLine() {
        try {
            currentPosition = 0;
            caseSensitiveMode = true;
            
            applicationFullPath = getExecutablePath();
            parameters = getArguments();
            optionParameters = parameters;
            
        } catch ( runtime_error &error ) {
            // Clean up all previous efforts...
            applicationFullPath.clear();
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const string CommandLine::getCommandLine() const {
        return applicationFullPath + " " + getAllParameters();
    }
    
    const string CommandLine::getAllParameters() const {
        string parameters;
        for ( unsigned index = 0; index < this->parameters.size(); ++index ) {
            parameters.append( this->parameters.at( index ) );
            parameters.append( index + 1 == this->parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const string CommandLine::getApplicationName() const {
        return applicationFullPath.substr( applicationFullPath.rfind( PATH_SEPARATOR ) + 1U );
    }
    
    const string CommandLine::getApplicationPath() const {
        return applicationFullPath.substr( 0, applicationFullPath.rfind( PATH_SEPARATOR ) );
    }
    
    const string CommandLine::getApplicationFullPath() const {
        return applicationFullPath;
    }
    
    const string CommandLine::getCurrentWorkingDirectory() const {
        try {
            return getCurrentDirectory();
        } catch ( runtime_error &error ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const bool CommandLine::hasParameters() const {
        return parameters.empty() ? false : true;
    }
    
    const bool CommandLine::hasParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? true : false;
    }
    
    const int CommandLine::getParametersNumber() const {
        return parameters.size();
    }
    
    const string CommandLine::getParameter( const unsigned &position ) const {
        return position <= parameters.size() && position > 0 ? parameters.at( position - 1 ) : "";
    }
    
    const int CommandLine::getParameterAsInteger( const unsigned &position ) const {
        return atoi( getParameter( position ).c_str() );
    }
    
    const double CommandLine::getParameterAsFloat( const unsigned &position ) const {
        return atof( getParameter( position ).c_str() );
    }
    
    void CommandLine::gotoFirstParameter() {
        currentPosition = 0;
    }
    
    const bool CommandLine::gotoNextParameter() {
        if ( currentPosition + 1 < parameters.size() ) {
            ++currentPosition;
            return true;
        }
        return false;
    }
    
    const int CommandLine::getCurrentPosition() const {
        return parameters.size() ? currentPosition + 1 : 0;
    }
    
    const string CommandLine::getCurrentParameter() const {
        return getParameter( currentPosition + 1 );
    }
    
    const int CommandLine::getCurrentParameterAsInteger() const {
        return getParameterAsInteger( currentPosition + 1 );
    }
    
    const double CommandLine::getCurrentParameterAsFloat() const {
        return getParameterAsFloat( currentPosition + 1 );
    }
    
    const string CommandLine::getFirstParameter() const {
        return getParameter( 1 );
    }
    
    const int CommandLine::getFirstParameterAsInteger() const {
        return getParameterAsInteger( 1 );
    }
    
    const double CommandLine::getFirstParameterAsFloat() const {
        return getParameterAsFloat( 1 );
    }
    
    const string CommandLine::getLastParameter() const {
        return getParameter( parameters.size() );
    }
    
    const int CommandLine::getLastParameterAsInteger() const {
        return getParameterAsInteger( parameters.size() );
    }
    
    const double CommandLine::getLastParameterAsFloat() const {
        return getParameterAsFloat( parameters.size() );
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) {
        postfixed = false;
        this->optionPrefix = optionPrefix;
        optionParameters = parameters;
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) {
        if ( !optionPostfix.empty() ) {
            postfixed = true;
            this->optionPrefix = optionPostfix;
            convertOptionPostfixToPrefix();
        }
    }
    
    void CommandLine::convertOptionPostfixToPrefix() {
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
    
    const string CommandLine::prefixAndPostfixOptionPostfixWhithWhitespace( string parameters ) const {
        size_t found = parameters.find( optionPrefix, 0 );
        while ( found != string::npos ) {
            parameters.replace( parameters.find( optionPrefix, found ), optionPrefix.size(), " " + optionPrefix + " " );
            found += 2 + optionPrefix.size();
            found = parameters.find( optionPrefix, found );
        }
        return parameters;
    }
    
    const vector<string> CommandLine::removeNullElement( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( parameters.at( position ).empty() ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const vector<string> CommandLine::removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) const {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position + 1 < parameters.size() && parameters.at( position ) == optionPrefix && parameters.at( position + 1 ) == optionPrefix ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const string CommandLine::getOptionPrefix() const {
        return postfixed ? "" : optionPrefix;
    }
    
    const string CommandLine::getOptionPostfix() const {
        return postfixed ? optionPrefix : "";
    }
    
    const bool CommandLine::hasOption( const string &option ) const {
        return findOptionPosition( option ) != OPTION_NOT_FOUND ? true : false;
    }
    
    const int CommandLine::getOptionValueAsInteger( const string &option ) const {
        return atoi( getOptionValue( option ).c_str() );
    }
    
    const double CommandLine::getOptionValueAsFloat( const string &option ) const {
        return atof( getOptionValue( option ).c_str() );
    }
    
    const string CommandLine::getOptionValue( const string &option ) const {
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
    
    const string CommandLine::getOptionLongValue( const string &option ) const {
        int index = findOptionPosition( option );
        string parameters;
        if ( index != OPTION_NOT_FOUND ) {
            size_t found;
            for ( ++index; unsigned( index ) < optionParameters.size(); ++index ) {
                found = optionPrefix.empty() || optionParameters.at( index ) == optionPrefix ? string::npos : optionParameters.at( index ).find( optionPrefix );
                
                if ( found == string::npos || found > 0 ) {
                    parameters += optionParameters.at( index );
                } else {
                    break;
                }
                
                if ( unsigned( index + 1 ) < optionParameters.size() ) {
                    parameters += " ";
                }
            }
        }
        return trim( parameters );
    }
    
    const int CommandLine::findOptionPosition( string option ) const {
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
    
    void CommandLine::optionCaseSensitive() {
        caseSensitiveMode = true;
    }
    
    void CommandLine::optionCaseInsensitive() {
        caseSensitiveMode = false;
    }
    
    const bool CommandLine::isOptionCaseSensitive() const {
        return caseSensitiveMode;
    }
}
