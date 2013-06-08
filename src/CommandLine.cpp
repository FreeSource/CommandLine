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
	
	struct self{
		string applicationFullPath;
		vector<string> parameters;
		vector<string> optionParameters;
		
		unsigned currentPosition;
		bool caseSensitiveMode;
		string optionPrefix;
		bool postfixed;
	} self;
	
	const string getAllParameters();
	void convertOptionPostfixToPrefix();
	const int findOptionPosition( string option );
	const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters );
	const vector<string> removeNullElement( vector<string> parameters );
	const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters );
	
	const string getAllParameters() {
		string parameters;
		for ( unsigned index = 0; index < self.parameters.size(); ++index ) {
			parameters.append( self.parameters.at( index ) );
			parameters.append( index + 1 == self.parameters.size() ? "" : " " );
		}
		return parameters;
	}
	
	const int findOptionPosition( string option ) {
		if ( !option.empty() ) {
			if ( !self.caseSensitiveMode ) {
				transform( option.begin(), option.end(), option.begin(), ::tolower );
			}
			
			string parameter;
			for ( unsigned index = 0; index < self.optionParameters.size(); ++index ) {
				parameter = self.optionParameters.at( index );
				
				if ( !self.caseSensitiveMode ) {
					transform( parameter.begin(), parameter.end(), parameter.begin(), ::tolower );
				}
				
				if ( self.optionPrefix + option == parameter ) {
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
			if ( position > 0 && parameters.at( position ) == self.optionPrefix ) {
				if ( parameters.at( position - 1 ).find( self.optionPrefix ) == string::npos ) {
					parameters.at( position - 1 ) = self.optionPrefix + parameters.at( position - 1 );
				}
				
				parameters.erase( parameters.begin() + position );
				--position;
			}
		}
		self.optionParameters = parameters;
	}
	
	const vector<string> removeOptionPostfixDuplicityBetweenOptionAndValue( vector<string> parameters ) {
		for ( unsigned position = 0; position < parameters.size(); ++position ) {
			if ( position + 1 < parameters.size() && parameters.at( position ) == self.optionPrefix && parameters.at( position + 1 ) == self.optionPrefix ) {
				parameters.erase( parameters.begin() + position );
				--position;
			}
		}
		return parameters;
	}
	
	const string prefixAndPostfixOptionPostfixWithWhitespace( string parameters ) {
		size_t found = parameters.find( self.optionPrefix, 0 );
		while ( found != string::npos ) {
			parameters.replace( parameters.find( self.optionPrefix, found ), self.optionPrefix.size(), " " + self.optionPrefix + " " );
			found += 2 + self.optionPrefix.size();
			found = parameters.find( self.optionPrefix, found );
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
			self.currentPosition = 0;
			self.caseSensitiveMode = true;
			
			self.applicationFullPath = getExecutablePath();
			self.parameters = getArguments();
			self.optionParameters = self.parameters;
		}
		catch ( runtime_error &error ) {
			// Clean up all previous efforts...
			self.applicationFullPath.clear();
			throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
		}
	}
	
	const string CommandLine::getApplicationName() const {
		return self.applicationFullPath.substr( self.applicationFullPath.find_last_of( "/\\" ) + 1U );
	}
	
	const string CommandLine::getApplicationPath() const {
		return self.applicationFullPath.substr( 0, self.applicationFullPath.find_last_of( "/\\" ) );
	}
	
	const string CommandLine::getCurrentWorkingDirectory() const {
		try {
			return getCurrentDirectory();
		}
		catch ( runtime_error &error ) {
			throw runtime_error( "FILE: " + string( __FILE__ ) + " FUNCTION: " + string( __PRETTY_FUNCTION__ ) + " -> " + error.what() );
		}
	}
	
	const int CommandLine::size() const {
		return self.parameters.size();
	}
	
	const string CommandLine::getParameter( const unsigned &position ) const {
		return position <= self.parameters.size() && position > 0 ? self.parameters.at( position - 1 ) : "";
	}
	
	void CommandLine::setOptionPrefix( const string &optionPrefix ) {
		self.postfixed = false;
		self.optionPrefix = optionPrefix;
		self.optionParameters = self.parameters;
	}
	
	void CommandLine::setOptionPostfix( const string &optionPostfix ) {
		if ( !optionPostfix.empty() ) {
			self.postfixed = true;
			self.optionPrefix = optionPostfix;
			convertOptionPostfixToPrefix();
		}
	}
	
	const bool CommandLine::hasOption( const string &option ) const {
		return findOptionPosition( option ) != OPTION_NOT_FOUND ? true : false;
	}
	
	const string CommandLine::getOptionValue( const string &option ) const {
		int index = findOptionPosition( option );
		if ( index != OPTION_NOT_FOUND && unsigned( ++index ) < self.optionParameters.size() ) {
			if ( self.optionPrefix.empty() ) {
				return self.optionParameters.at( index );
			}
			
			size_t found = self.optionParameters.at( index ).find( self.optionPrefix );
			
			if ( found == string::npos || found > 0 ) {
				return self.optionParameters.at( index );
			}
		}
		return "";
	}
	
	const string CommandLine::getOptionLongValue( const string &option ) const {
		int index = findOptionPosition( option );
		string parameters;
		if ( index != OPTION_NOT_FOUND ) {
			size_t found;
			for ( ++index; unsigned( index ) < self.optionParameters.size(); ++index ) {
				found = self.optionPrefix.empty() || self.optionParameters.at( index ) == self.optionPrefix ? string::npos : self.optionParameters.at( index ).find( self.optionPrefix );
				
				if ( found == string::npos || found > 0 ) {
					parameters += self.optionParameters.at( index );
				}
				else {
					break;
				}
				
				if ( unsigned( index + 1 ) < self.optionParameters.size() ) {
					parameters += " ";
				}
			}
		}
		return trim( parameters );
	}
	
	void CommandLine::optionCaseSensitive() {
		self.caseSensitiveMode = true;
	}
	
	void CommandLine::optionCaseInsensitive() {
		self.caseSensitiveMode = false;
	}
}
