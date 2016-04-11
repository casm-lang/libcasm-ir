#   
#   Copyright (c) 2016 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libcasm-ir
#   
#   This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
#   You either find the license text below or in the root directory of this 
#   project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
#   please refer to https://opensource.org/licenses/OSL-3.0.
#   
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#   CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
#   WITH THE SOFTWARE.
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
INCLUDE += -I ../casm-fe/build/src
INCLUDE += -I ../pass/src

LIBRARY += ../casm-fe/build/libfrontend.a

default: $(LIBRARY) obj $(TARGET)

../casm-fe/build/libfrontend.a:
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
