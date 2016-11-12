#   
#   Copyright (c) 2015-2016 CASM Organization
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/casm-lang/libcasm-ir
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
#   along with libcasm-ir. If not, see <http://www.gnu.org/licenses/>.
#   

.PHONY:

default: debug

help:
	@echo "TODO"

REPO = libcasm-ir

TARGET = $(REPO).a

TEST_TARGET = test-$(REPO)


CP  = $(shell find src -name '*.cpp' | cut -d'.' -f1)
CO  = $(CP:%=obj/%.o)

CI += -I ./
CI += -I ./src
CI += -I ./src/analyze
CI += -I ./src/transform

CI += -I ../stdhl
CI += -I ../pass
CI += -I ../casm-fe

CL  =

CC  =
CF  =

  %-gcc: CC = gcc
%-clang: CC = clang

  debug-%: CF += -O0 -g
release-%: CF += -O3 -DNDEBUG

linux%:  CF += -Wall -std=c++11
linux%:  XF += -Wall -std=c11
linux3%: CF += -m32
linux6%: CF += -m64


build: config $(TARGET)
check: build $(TEST_TARGET)

linux32-build: build
linux64-build: build

linux32-check: check
linux64-check: check


  debug-build-linux32-gcc:   linux32-build
  debug-check-linux32-gcc:   linux32-check
release-build-linux32-gcc:   linux32-build
release-check-linux32-gcc:   linux32-check

  debug-build-linux64-gcc:   linux64-build
  debug-check-linux64-gcc:   linux64-check
release-build-linux64-gcc:   linux64-build
release-check-linux64-gcc:   linux64-check

  debug-build-linux32-clang: linux32-build
  debug-check-linux32-clang: linux32-check
release-build-linux32-clang: linux32-build
release-check-linux32-clang: linux32-check

  debug-build-linux64-clang: linux64-build
  debug-check-linux64-clang: linux64-check
release-build-linux64-clang: linux64-build
release-check-linux64-clang: linux64-check


  debug:   debug-build-linux64-clang
release: clean release-build-linux64-clang

test:           debug-check-linux64-clang
test-release: release-check-linux64-clang


config: CFG=CC="$(CC)" CF="$(CF)"
config:
	@echo "CFG  $(CFG)"


obj/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

obj/%.o: %.c
	@mkdir -p `dirname $@`
	@echo "C   " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

$(TARGET): $(CO) $(CL)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)
	@echo "RM  " $(TEST_TARGET)
	@rm -f $(TEST_TARGET)


#TEST_TARGET = $(TARGET:%.a=%-test.a)

TF   = $(shell find uts -name '*.cpp' | cut -d'.' -f1)
TO = $(TF:%=obj/%.o)

TI  = -I ../gtest/googletest/include
TI += -I ../gtest/googletest

TL  = -lstdc++
TL += -lm
TL += -lpthread

obj/uts/%.o: uts/%.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(TI) $(CI) -c $< -o $@

$(TEST_TARGET): $(TO) $(CO) $(TARGET)
	@echo "LD " $@
	@$(CC) \
	  $(CF) \
	  $(TI) \
	  $(CI) \
	  $(TL) \
	  -o $@ \
	  $(TO) \
	  $(TARGET) \
	  ../gtest/googletest/src/gtest-all.cc \
	  ../gtest/googletest/src/gtest_main.cc 
	@echo "RUN " $@
	@./$@
