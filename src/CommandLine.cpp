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

#include <algorithm>
#include <stdexcept>

#include <charseq.h>
#include <system.h>

namespace {
    
    using namespace util;
    using namespace crosslib;
    using std::runtime_error;
    
    const int OPTION_NOT_FOUND = -1;
    
    // PRIVATE:
    string applicationFullPath;
    vector<string> parameters;
    vector<string> optionParameters;
    
    unsigned currentPosition;
    bool caseSensitiveMode;
    string optionPrefix;
    bool postfixed;
    
    const string getAllParameters();
    void convertOptionPostfixToPrefix();
    const int findOptionPosition( string option );
    const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters );
    const vector<string> removeNullElement( vector<string> parameters );
    const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters );
    
    const string getAllParameters() {
        string parameters;
        for ( unsigned position = 0; position < ::parameters.size(); ++position ) {
            parameters.append( ::parameters.at( position ) );
            parameters.append( position + 1 == ::parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const int findOptionPosition( string option ) {
        if ( !option.empty() ) {
            if ( !caseSensitiveMode ) {
                transform( option.begin(), option.end(), option.begin(), ::tolower );
            }
            
            string parameter;
            for ( unsigned position = 0; position < optionParameters.size(); ++position ) {
                parameter = optionParameters.at( position );
                
                if ( !caseSensitiveMode ) {
                    transform( parameter.begin(), parameter.end(), parameter.begin(), ::tolower );
                }
                
                if ( optionPrefix + option == parameter ) {
                    return position;
                }
            }
        }
        return OPTION_NOT_FOUND;
    }
    
    void convertOptionPostfixToPrefix() {
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
    
    const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position + 1 < parameters.size() && parameters.at( position ) == optionPrefix && parameters.at( position + 1 ) == optionPrefix ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters ) {
        size_t found = parameters.find( optionPrefix, 0 );
        while ( found != string::npos ) {
            parameters.replace( parameters.find( optionPrefix, found ), optionPrefix.size(), " " + optionPrefix + " " );
            found += 2 + optionPrefix.size();
            found = parameters.find( optionPrefix, found );
        }
        return parameters;
    }
    
    const vector<string> removeNullElement( vector<string> parameters ) {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( parameters.at( position ).empty() ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
}

namespace environs {
    
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
        for ( unsigned index = 0; index < ::parameters.size(); ++index ) {
            parameters.append( ::parameters.at( index ) );
            parameters.append( index + 1 == ::parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const string CommandLine::getApplicationName() const {
        return applicationFullPath.substr( applicationFullPath.find_last_of( "/\\" ) + 1U );
    }
    
    const string CommandLine::getApplicationPath() const {
        return applicationFullPath.substr( 0, applicationFullPath.find_last_of( "/\\" ) );
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
    
    const string CommandLine::getFirstParameter() const {
        return getParameter( 1 );
    }
    
    const string CommandLine::getLastParameter() const {
        return getParameter( parameters.size() );
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) {
        postfixed = false;
        ::optionPrefix = optionPrefix;
        optionParameters = parameters;
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) {
        if ( !optionPostfix.empty() ) {
            postfixed = true;
            optionPrefix = optionPostfix;
            convertOptionPostfixToPrefix();
        }
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
    
    const string CommandLine::getOptionValue( const string &option ) const {
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
    
    const string CommandLine::getOptionLongValue( const string &option ) const {
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
        return trim( parameters );
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
