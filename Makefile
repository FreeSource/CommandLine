# --------------------------------------------------------------------------
#    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#    
#    File: Makefile
#    Version: 3.0.0
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
APPPATH = app
OBJS = build/obj/
BIN  = build/bin/
INCLUDES = include
VPATH = src
RES = rc
OPTFLAGS = -Os
CFLAGS = -I$(INCLUDES) ${OPTFLAGS} -Wall -pedantic-errors -std=c++98 $(BITS)
OSTYPE = $(shell gcc -dumpmachine)
EXEC = myapp.exe

ifneq (,$(findstring $(firstword $(subst -, ,$(shell gcc -dumpmachine))),mingw32 i686 i586 i386))
    BITS = -m32
else 
    BITS = -m64
endif

ifneq (,$(findstring mingw,$(OSTYPE)))
    FIXPATH = $(subst /,\,$1)
    OSTYPE = Windows
    LIB =
    RM = del /s /q
else
    FIXPATH = $1
    RM = rm -f
    
    ifneq (,$(findstring linux,$(OSTYPE)))
        OSTYPE = Linux
        LIB =
    else ifneq (,$(findstring freebsd,$(OSTYPE)))
        OSTYPE = FreeBSD
        LIB =
    else ifneq (,$(findstring solaris,$(OSTYPE)))
        OSTYPE = Solaris
        LIB = -R/usr/local/lib:/usr/lib/64:/usr/local/lib/sparcv9
    else ifneq (,$(findstring darwin,$(OSTYPE)))
        OSTYPE = MacOSX
        LIB =
    else
        $(error Operating System not found)
    endif
endif

define compile
    @echo $(subst _$(OSTYPE),,$1)
    @$(CXX) -c -o $(OBJS)$(subst _$(OSTYPE),,$1).o $(VPATH)/$1.cpp $(CFLAGS)
endef

all: main CommandLine compatibility util
	@echo Linking...
	@$(CXX) -o $(BIN)$(EXEC) $(OBJS)*.o $(LIB) $(CFLAGS)
	@strip $(BIN)$(EXEC)

main:
	@echo Compiling on $(OSTYPE) $(subst -m,,$(BITS))BIT...
	@echo $@
	@$(CXX) -c -o $(OBJS)$@.o $(APPPATH)/$@.cpp $(CFLAGS)

CommandLine:
	$(call compile,$@)

compatibility:
	$(call compile,$@_$(OSTYPE))

util:
	$(call compile,$@)

.PHONY: clean
clean:
	@echo Cleaning...
	@-$(RM) $(call FIXPATH,$(BIN)*)
	@-$(RM) $(call FIXPATH,$(OBJS)*.o)