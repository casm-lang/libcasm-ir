#
#   Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 <https://github.com/casm-lang/libcasm-ir>
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
#   Additional permission under GNU GPL version 3 section 7
#
#   libcasm-ir is distributed under the terms of the GNU General Public License
#   with the following clarification and special exception: Linking libcasm-ir
#   statically or dynamically with other modules is making a combined work
#   based on libcasm-ir. Thus, the terms and conditions of the GNU General
#   Public License cover the whole combination. As a special exception,
#   the copyright holders of libcasm-ir give you permission to link libcasm-ir
#   with independent modules to produce an executable, regardless of the
#   license terms of these independent modules, and to copy and distribute
#   the resulting executable under terms of your choice, provided that you
#   also meet, for each linked independent module, the terms and conditions
#   of the license of that module. An independent module is a module which
#   is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
#   may extend this exception to your version of the library, but you are
#   not obliged to do so. If you do not wish to do so, delete this exception
#   statement from your version.
#

ifndef TARGET
  $(error TARGET not defined!)
endif

OBJ = obj
BIN = install
.PHONY: $(OBJ)
.NOTPARALLEL: $(OBJ)

CLANG := $(shell clang --version 2> /dev/null)
ifdef CLANG
  CC=clang
  CXX=clang++
endif

ifdef ENV_CC
  CC=$(ENV_CC)
endif

ifdef ENV_CXX
  CXX=$(ENV_CXX)
endif


default: debug

help:
	@echo "TODO"


$(OBJ):
	@mkdir -p $(OBJ)

clean:
ifneq ("$(wildcard $(OBJ)/CMakeCache.txt)","")
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) clean
endif

clean-all:
	@echo "-- Removing build directory" $(OBJ)
	@rm -rf $(OBJ)

TYPES = debug sanitize release

SYNCS = $(TYPES:%=%-sync)
TESTS = $(TYPES:%=%-test)
BENCH = $(TYPES:%=%-benchmark)
INSTA = $(TYPES:%=%-install)
ANALY = $(TYPES:%=%-analyze)
ALL   = $(TYPES:%=%-all)


$(OBJ)/Makefile: $(OBJ)
ifeq ("$(wildcard $(OBJ)/CMakeCache.txt)","")
	@(\
	cd $(OBJ); \
	cmake \
	-D CMAKE_INSTALL_PREFIX=$(BIN) \
	-D CMAKE_BUILD_TYPE=$(TYPE) \
	-D CMAKE_C_COMPILER=$(CC) \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	.. \
	)
else
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) rebuild_cache
endif


sync: debug-sync

sync-all: $(TYPES:%=%-sync)

$(SYNCS):%-sync: $(OBJ)
	@$(MAKE) $(MFLAGS) --no-print-directory TYPE=$(patsubst %-sync,%,$@) $(OBJ)/Makefile


$(TYPES):%: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) ${TARGET}

all: debug-all

$(ALL):%-all: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ)


test: debug-test

test-all: $(TYPES:%=%-test)

$(TESTS):%-test: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-check
	@echo "-- Running unit test"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-check --gtest_output=xml:obj/report.xml $(ENV_ARGS)


benchmark: debug-benchmark

benchmark-all: $(TYPES:%=%-benchmark)

$(BENCH):%-benchmark: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-run
	@echo "-- Running benchmark"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-run -o console -o json:obj/report.json $(ENV_ARGS)


install: debug-install

install-all: $(TYPES:%=%-install)

$(INSTA):%-install: %
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) install


format: $(FORMAT:%=%-format-cpp)

%-format-cpp:
	@echo "-- Formatting Code C++: $(patsubst %-format-cpp,%,$@)"
	@clang-format -i \
	`ls $(patsubst %-format-cpp,%,$@)/*.h 2> /dev/null | grep -e .h` 2> /dev/null
	@clang-format -i \
	`ls $(patsubst %-format-cpp,%,$@)/*.cpp 2> /dev/null | grep -e .cpp` 2> /dev/null


update: $(UPDATE_FILE:%=%-update)

%-update:
	@echo "-- Updating: $(patsubst %-update,%,$@)"
	@for i in $(UPDATE_PATH); \
	  do \
	    cp -v \
	    $(UPDATE_ROOT)/$(patsubst %-update,%,$@) \
	    $$i/$(patsubst %-update,%,$@); \
	  done


license: $(UPDATE_ROOT:%=%-license) $(UPDATE_PATH:%=%-license)

%-license:
	@echo "-- License: $(patsubst %-update,%,$@)"
	@(cd $(patsubst %-update,%,$@); \
	  python2 $(UPDATE_ROOT)/src/py/Licenser.py \
	)


analyze: debug-analyze

analyze-all: $(TYPES:%=%-analyze)

$(ANALY):%-analyze: %
	@echo "-- Running analysis tools"
	$(MAKE) $(MFLAGS) $@-cppcheck
	$(MAKE) $(MFLAGS) $@-iwyu
	$(MAKE) $(MFLAGS) $@-scan-build


analyze-cppcheck: debug-analyze-cppcheck

CPPCHECK_REPORT = ./$(OBJ)/.cppcheck.xml

%-analyze-cppcheck:
	@echo "-- Running 'cppcheck' $(patsubst %-analyze-cppcheck,%,$@)"
	@echo -n "" > $(CPPCHECK_REPORT)
	cppcheck \
	-v \
	--template=gcc \
	--force \
	--report-progress \
	--enable=all \
	-I . \
	./src/c**

	cppcheck \
	-v \
	--template=gcc \
	--errorlist \
	--xml-version=2 \
	--force \
	--enable=all \
	-I . \
	./src/c** > $(CPPCHECK_REPORT)


analyze-iwyu: debug-analyze-iwyu

IWYU_REPORT = ./$(OBJ)/.iwyu.txt

%-analyze-iwyu:
	@echo "-- Running 'iwyu' $(patsubst %-analyze-iwyu,%,$@)"
	@echo -n "" > $(IWYU_REPORT)
	@for i in `find ./c*`; do include-what-you-use $$i; done
	@for i in `find ./c*`; do include-what-you-use $$i >> $(IWYU_REPORT); done


analyze-scan-build: debug-analyze-scan-build

SCAN_BUILD_REPORT = ./$(OBJ)/.scan-build
SCAN_BUILD_REPORT_ATTIC = $(SCAN_BUILD_REPORT).attic

%-analyze-scan-build: clean
	@echo "-- Running 'scan-build' $(patsubst %-analyze-scan-build,%,$@)"
	@mkdir -p $(SCAN_BUILD_REPORT_ATTIC)

	scan-build \
	-v \
	-o $(SCAN_BUILD_REPORT).attic \
	-stats \
	-plist-html \
	-analyzer-config stable-report-filename=true \
	-enable-checker llvm.Conventions \
	--force-analyze-debug-code \
	--keep-going \
	--keep-empty \
	$(MAKE) $(MFLAGS) $(patsubst %-analyze-scan-build,%,$@)

	@ln -f -s \
	$(SCAN_BUILD_REPORT_ATTIC)/`ls -t $(SCAN_BUILD_REPORT_ATTIC) | head -1` \
	$(SCAN_BUILD_REPORT)
