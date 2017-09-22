#
#   Copyright (c) 2015-2017 CASM Organization
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

set( PROJECT libcasm-ir )
cmake_minimum_required( VERSION ${CMAKE_VERSION} )

set( CMAKE_VERSION 3.2 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( CMAKE_CXX_STANDARD 11 )
project( ${PROJECT} LANGUAGES CXX )

find_package( ECM REQUIRED CONFIG QUIET )
set( CMAKE_MODULE_PATH
  ${ECM_MODULE_PATH}
  ${${PROJECT}_SOURCE_DIR}/.cmake
  )

include( ECMGenerateHeaders )
include( FeatureSummary )
include( GenerateExportHeader )
include( CPack )
include( LibPackage )

package_git( ${PROJECT} )

package_print_path( CMAKE_MODULE_PATH )
package_print_path( CMAKE_INSTALL_PREFIX )

package_find_git( libstdhl master REQUIRED "../stdhl;lib/stdhl" obj )
package_find_git( libpass  master REQUIRED "../pass; lib/pass"  obj )
package_find_git( GTest    1.8    REQUIRED "../gtest;lib/gtest" build )
package_find_git( Hayai    1.0.1  REQUIRED "../hayai;lib/hayai" build )

add_subdirectory( src )
add_subdirectory( etc )


#
# library
#

add_library( ${PROJECT} STATIC
  $<TARGET_OBJECTS:${PROJECT}-cpp>
  )

set( ${PROJECT}_VERSION ${GIT_REVTAG} )
set_property( TARGET ${PROJECT} PROPERTY VERSION   ${PROJECT}_VERSION )
set_property( TARGET ${PROJECT} PROPERTY PREFIX "" )

install(
  TARGETS  ${PROJECT}
  EXPORT   ${PROJECT}Targets
  LIBRARY  DESTINATION lib
  ARCHIVE  DESTINATION lib
  RUNTIME  DESTINATION bin
  INCLUDES DESTINATION include
  )

generate_export_header( ${PROJECT} )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}_export.h"
  DESTINATION
  "include/${PROJECT}"
  )

#
# test
# 

add_executable( ${PROJECT}-check
  $<TARGET_OBJECTS:${PROJECT}-test>
  )

target_link_libraries( ${PROJECT}-check
  ${PROJECT}
  ${LIBPASS_LIBRARY}
  ${LIBSTDHL_LIBRARY}
  GTest::GTest
  GTest::Main
  )


#
# benchmark
#

add_executable( ${PROJECT}-run
  $<TARGET_OBJECTS:${PROJECT}-benchmark>
  )

target_link_libraries( ${PROJECT}-run
  ${PROJECT}
  ${LIBPASS_LIBRARY}
  ${LIBSTDHL_LIBRARY}
  ${HAYAI_LIBRARY}
  )


#
# cmake
#

install(
  FILES
    .cmake/Find${PROJECT}.cmake
  DESTINATION
    "share/cmake/Module/${PROJECT}"
  )

#
# build features and checks
#

include( LibCompile )
compile_flags( CXX )
compile_check()
feature_summary( WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES )
