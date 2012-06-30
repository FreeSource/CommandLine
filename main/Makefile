# --------------------------------------------------------------------------
#    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#    
#    File: Makefile
#    Version: 1.0.0
#    Copyright: (C) 2012 by Enzo Roberto Verlato
#    Contact: enzover@ig.com.br
#    All rights reserved.
#    
# --------------------------------------------------------------------------
#    This file is part of the CommandLine Class Project
#    
#    This file may be used under the terms of the GNU General Public
#    License version 2.0 as published by the Free Software Foundation
#    and appearing in the file LICENSE.GPL2 included in the packaging of
#    this file.
#    
#    This file is provided "AS IS" in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF DESIGN;
#    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
#    PARTICULAR PURPOSE. See the GNU General Public License for more details.
#    
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software Foundation,
#    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
# --------------------------------------------------------------------------

CXX = g++
INCLUDES = ../include
VPATH = ../src
RES = ../rc/
OPTFLAGS = -s -Os
CFLAGS = -I$(INCLUDES) ${OPTFLAGS} -Wall -pedantic-errors -std=c++98 $(BITS)

EXEC   = myapp
OBJS = ../build/obj/
BIN  = ../build/bin/

ifeq ($(OS),Windows_NT)
    FIXPATH = $(subst /,\,$1)
    OSTYPE = WINDOWS
    LIB =
    RM = del /s /q
    
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        BITS = -m32
    else
        BITS = -m64
    endif
else
    FIXPATH = $1
    OSTYPE = $(shell uname -s)
    RM = rm -f
    
    ifeq ($(OSTYPE),Linux)
        OSTYPE = LINUX
        LIB =
        
        BITS = $(shell uname -m)
        ifeq ($(BITS),i686)
            BITS = -m32
        else
            BITS = -m64
        endif
    else
        ifeq ($(OSTYPE),FreeBSD)
                OSTYPE = FREEBSD
                LIB =
                
                BITS = $(shell uname -m)
                ifeq ($(BITS),i386)
                    BITS = -m32
                else
                    BITS = -m64
                endif
        else
            OSTYPE = SOLARIS
            LIB = -R/usr/local/lib:/usr/lib/64:/usr/local/lib/sparcv9
            
            BITS = $(shell isainfo -b)
            ifeq ($(BITS),32)
                BITS = -m32
            else
                BITS = -m64
            endif
        endif
    endif
endif

all : main CommandLine compatibility util
	@echo Linking...
	@$(CXX) -o $(BIN)$(EXEC) $(OBJS)*.o $(LIB) $(CFLAGS)

main : main.cpp
	@echo Compiling on $(OSTYPE) $(subst -m,,$(BITS))BIT...
	@echo main
	@$(CXX) -c -o $(OBJS)$@.o $^ $(CFLAGS)

CommandLine : CommandLine.cpp
	@echo CommandLine
	@$(CXX) -c -o $(OBJS)$@.o $(VPATH)/$@.cpp $(CFLAGS)

compatibility : compatibility.cpp
	@echo compatibility
	@$(CXX) -c -o $(OBJS)$@.o $(VPATH)/$@.cpp $(CFLAGS)

util : util.cpp
	@echo util
	@$(CXX) -c -o $(OBJS)$@.o $(VPATH)/$@.cpp $(CFLAGS)

.PHONY : clean
clean :
	@echo Cleaning...
	@-$(RM) $(call FIXPATH,$(BIN)*)
	@-$(RM) $(call FIXPATH,$(OBJS)*.o)