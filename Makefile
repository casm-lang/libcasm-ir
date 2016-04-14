#   
#   Copyright (c) 2015-2016 CASM Organization
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libcasm-ir
#   
#   This file is part of libcasm-ir.
#   
#   libcasm-ir is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   libcasm-ir is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.
#   

AR=ar

CC=clang
CCFLAG += -std=c11
CCFLAG += -g -O0
CCFLAG += -Wall

CPP=clang
CPPFLAG += -std=c++11
CPPFLAG += -g -O0
CPPFLAG += -Wall
#CPPFLAG += -Wextra

TARGET += libcasm-ir.a

CPPOBJECTS += obj/Value.o
CPPOBJECTS += obj/Block.o
CPPOBJECTS += obj/Instruction.o
CPPOBJECTS += obj/Type.o
CPPOBJECTS += obj/User.o
CPPOBJECTS += obj/Rule.o
CPPOBJECTS += obj/Statement.o
CPPOBJECTS += obj/Constant.o
CPPOBJECTS += obj/Derived.o
CPPOBJECTS += obj/Function.o
CPPOBJECTS += obj/Builtin.o
CPPOBJECTS += obj/Agent.o
CPPOBJECTS += obj/Visitor.o
CPPOBJECTS += obj/Specification.o

CPPOBJECTS += obj/CasmIRDumpPass.o

CPPOBJECTS += obj/AstDumpPass.o
CPPOBJECTS += obj/TypeCheckPass.o
CPPOBJECTS += obj/SourceToAstPass.o
CPPOBJECTS += obj/AstToCasmIRPass.o

INCLUDE += -I ./src
INCLUDE += -I ./src/analyze
INCLUDE += -I ./src/transform
INCLUDE += -I ../
INCLUDE += -I ../casm-fe/src
INCLUDE += -I ../casm-fe/obj/src
INCLUDE += -I ../pass/src

LIBRARY += ../casm-fe/obj/libfrontend.a

default: $(LIBRARY) obj $(TARGET)

../casm-fe/obj/libfrontend.a:
	$(MAKE) -C ../casm-fe

obj:
	mkdir -p obj

obj/%.o: src/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

obj/%.o: src/analyze/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

obj/%.o: src/transform/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

libcasm-ir.a: $(CPPOBJECTS) $(LIBRARY)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
#	@$(AR) -rcT $@ $@.a $(filter %.a,$^)
	@ranlib $@
#	@rm -f $@.a

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)
	$(MAKE) clean -C ../casm-fe/
