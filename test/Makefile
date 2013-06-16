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

EXEC   = ../build/bin/myapp.exe
OUTPUT = result.txt

all :
	@echo Autotest...
	@"$(EXEC)" Mercury Venus Earth Mars Jupiter Saturn Uranus Neptune getParametersNumber > $(OUTPUT)
	@"$(EXEC)" Mercury Venus Earth Mars Jupiter Saturn Uranus Neptune getApplicationName >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus Earth Mars Jupiter Saturn Uranus Neptune getApplicationPath >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus Earth Mars Jupiter Saturn Uranus Neptune getCurrentWorkingDirectory >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus Earth Mars 3.14159265 Jupiter Saturn Uranus Neptune getParameter 4 >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus --planet Earth Mars Jupiter Saturn Uranus Neptune setOptionPrefix -- getOptionValue planet >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus Earth Mars planet = Jupiter Saturn Uranus Neptune setOptionPostfix = getOptionValue planet >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus --planet Earth Mars Jupiter Saturn Uranus Neptune setOptionPrefix -- hasOption planet >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus --planet Earth is the third planet from the Sun --next Mars Jupiter Saturn Uranus Neptune setOptionPrefix -- getOptionLongValue planet >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus --planet Earth is the third planet from the Sun --next Mars Jupiter Saturn Uranus Neptune setOptionPrefix -- optionCaseSensitive getOptionLongValue pLaNeT >> $(OUTPUT)
	@"$(EXEC)" Mercury Venus planet Earth is the third planet = from the Sun next = Mars Jupiter Saturn Uranus Neptune setOptionPostfix = optionCaseInsensitive getOptionLongValue pLaNeT >> $(OUTPUT)