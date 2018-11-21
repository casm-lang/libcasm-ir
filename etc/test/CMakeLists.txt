#
#   Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
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

include_directories(
  ${PROJECT_BINARY_DIR}/src
  ${LIBGTEST_INCLUDE_DIR}
  ${LIBTPTP_INCLUDE_DIR}
  ${LIBSTDHL_INCLUDE_DIR}
  ${LIBPASS_INCLUDE_DIR}
  )

add_library( ${PROJECT}-test OBJECT
  agent.cpp
  annotation.cpp
  enumeration.cpp
  isa.cpp
  main.cpp
  property.cpp
  value.cpp

  constant/binary.cpp
  constant.cpp
  constant/integer.cpp
  constant/list.cpp
  constant/range.cpp
  constant/rational.cpp
  constant/string.cpp
  constant/undef.cpp

  instruction/add.cpp
  instruction/imp.cpp
  instruction/inv.cpp
  instruction/pow.cpp

  operation/abort.cpp
  operation/add/integer.cpp
  operation/add/string.cpp
  operation/and.cpp
  operation/as/boolean/binary.cpp
  operation/as/boolean/boolean.cpp
  operation/as/boolean/integer.cpp
  operation/as/integer/binary.cpp
  operation/as/integer/boolean.cpp
  operation/as/integer/integer.cpp
  operation/assert.cpp
  operation/as/string/binary.cpp
  operation/as/string/boolean.cpp
  operation/as/string/decimal.cpp
  operation/as/string/enumeration.cpp
  operation/as/string/integer.cpp
  operation/as/string/rational.cpp
  operation/as/string/string.cpp
  operation/at/list.cpp
  operation/equ.cpp
  operation/geq/integer.cpp
  operation/gth/integer.cpp
  operation/imp.cpp
  operation/inv/integer.cpp
  operation/leq/integer.cpp
  operation/lth/integer.cpp
  operation/not.cpp
  operation/or.cpp
  operation/print.cpp
  operation/println.cpp
  operation/size/enumeration.cpp
  operation/size/list.cpp
  operation/size/range.cpp
  operation/xor.cpp

  transform/BranchEliminationPass.cpp

  type/binary.cpp
  type/boolean.cpp
  type.cpp
  type/decimal.cpp
  type/enum.cpp
  type/file.cpp
  type/id.cpp
  type/integer.cpp
  type/list.cpp
  type/port.cpp
  type/range.cpp
  type/ranged_integer.cpp
  type/rational.cpp
  type/record.cpp
  type/relation.cpp
  type/tuple.cpp
  type/void.cpp
  type/feature.cpp
)
