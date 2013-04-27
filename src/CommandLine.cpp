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
    string m_applicationFullPath;
    vector<string> m_parameters;
    vector<string> m_optionParameters;
    
    unsigned m_currentPosition;
    bool m_caseSensitiveMode;
    string m_optionPrefix;
    bool m_postfixed;
    
    const string getAllParameters();
    void convertOptionPostfixToPrefix();
    const int findOptionPosition( string option );
    const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters );
    const vector<string> removeNullElement( vector<string> parameters );
    const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters );
    
    const string getAllParameters() {
        string parameters;
        for ( unsigned index = 0; index < m_parameters.size(); ++index ) {
            parameters.append( m_parameters.at( index ) );
            parameters.append( index + 1 == m_parameters.size() ? "" : " " );
        }
        return parameters;
    }
    
    const int findOptionPosition( string option ) {
        if ( !option.empty() ) {
            if ( !m_caseSensitiveMode ) {
                transform( option.begin(), option.end(), option.begin(), ::tolower );
            }
            
            string parameter;
            for ( unsigned index = 0; index < m_optionParameters.size(); ++index ) {
                parameter = m_optionParameters.at( index );
                
                if ( !m_caseSensitiveMode ) {
                    transform( parameter.begin(), parameter.end(), parameter.begin(), ::tolower );
                }
                
                if ( m_optionPrefix + option == parameter ) {
                    return index;
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
            if ( position > 0 && parameters.at( position ) == m_optionPrefix ) {
                if ( parameters.at( position - 1 ).find( m_optionPrefix ) == string::npos ) {
                    parameters.at( position - 1 ) = m_optionPrefix + parameters.at( position - 1 );
                }
                
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        m_optionParameters = parameters;
    }
    
    const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) {
        for ( unsigned position = 0; position < parameters.size(); ++position ) {
            if ( position + 1 < parameters.size() && parameters.at( position ) == m_optionPrefix && parameters.at( position + 1 ) == m_optionPrefix ) {
                parameters.erase( parameters.begin() + position );
                --position;
            }
        }
        return parameters;
    }
    
    const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters ) {
        size_t found = parameters.find( m_optionPrefix, 0 );
        while ( found != string::npos ) {
            parameters.replace( parameters.find( m_optionPrefix, found ), m_optionPrefix.size(), " " + m_optionPrefix + " " );
            found += 2 + m_optionPrefix.size();
            found = parameters.find( m_optionPrefix, found );
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
            m_currentPosition = 0;
            m_caseSensitiveMode = true;
            
            m_applicationFullPath = getExecutablePath();
            m_parameters = getArguments();
            m_optionParameters = m_parameters;
        }
        catch ( runtime_error &error ) {
            // Clean up all previous efforts...
            m_applicationFullPath.clear();
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const string CommandLine::getApplicationName() const {
        return m_applicationFullPath.substr( m_applicationFullPath.find_last_of( "/\\" ) + 1U );
    }
    
    const string CommandLine::getApplicationPath() const {
        return m_applicationFullPath.substr( 0, m_applicationFullPath.find_last_of( "/\\" ) );
    }
    
    const string CommandLine::getCurrentWorkingDirectory() const {
        try {
            return getCurrentDirectory();
        }
        catch ( runtime_error &error ) {
            throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
        }
    }
    
    const int CommandLine::getParametersNumber() const {
        return m_parameters.size();
    }
    
    const string CommandLine::getParameter( const unsigned &position ) const {
        return position <= m_parameters.size() && position > 0 ? m_parameters.at( position - 1 ) : "";
    }
    
    void CommandLine::setOptionPrefix( const string &optionPrefix ) {
        m_postfixed = false;
        m_optionPrefix = optionPrefix;
        m_optionParameters = m_parameters;
    }
    
    void CommandLine::setOptionPostfix( const string &optionPostfix ) {
        if ( !optionPostfix.empty() ) {
            m_postfixed = true;
            m_optionPrefix = optionPostfix;
            convertOptionPostfixToPrefix();
        }
    }
    
    const bool CommandLine::hasOption( const string &option ) const {
        return findOptionPosition( option ) != OPTION_NOT_FOUND ? true : false;
    }
    
    const string CommandLine::getOptionValue( const string &option ) const {
        int index = findOptionPosition( option );
        if ( index != OPTION_NOT_FOUND && unsigned( ++index ) < m_optionParameters.size() ) {
            if ( m_optionPrefix.empty() ) {
                return m_optionParameters.at( index );
            }
            
            size_t found = m_optionParameters.at( index ).find( m_optionPrefix );
            
            if ( found == string::npos || found > 0 ) {
                return m_optionParameters.at( index );
            }
        }
        return "";
    }
    
    const string CommandLine::getOptionLongValue( const string &option ) const {
        int index = findOptionPosition( option );
        string parameters;
        if ( index != OPTION_NOT_FOUND ) {
            size_t found;
            for ( ++index; unsigned( index ) < m_optionParameters.size(); ++index ) {
                found = m_optionPrefix.empty() || m_optionParameters.at( index ) == m_optionPrefix ? string::npos : m_optionParameters.at( index ).find( m_optionPrefix );
                
                if ( found == string::npos || found > 0 ) {
                    parameters += m_optionParameters.at( index );
                }
                else {
                    break;
                }
                
                if ( unsigned( index + 1 ) < m_optionParameters.size() ) {
                    parameters += " ";
                }
            }
        }
        return trim( parameters );
    }
    
    void CommandLine::optionCaseSensitive() {
        m_caseSensitiveMode = true;
    }
    
    void CommandLine::optionCaseInsensitive() {
        m_caseSensitiveMode = false;
    }
}
