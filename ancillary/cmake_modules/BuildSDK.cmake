#	 Copyright (C) 2014  Intel Corporation
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

MESSAGE("\n**** Building Downloader SDK ****")

# Obtain api file location (a.k.a the shared library location)
set(api_file_location "$<TARGET_FILE:xfstk-dldr-api>")

add_custom_target(SDK ALL DEPENDS xfstk-dldr-api)

add_custom_command(TARGET SDK
  COMMAND ${CMAKE_COMMAND} -E make_directory '${CMAKE_BINARY_DIR}/SDK'
  COMMAND ${CMAKE_COMMAND} -E copy '${api_file_location}' '${CMAKE_BINARY_DIR}/SDK'  #copy the shared library
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api/xfstk-dldr-api_global.h' '${CMAKE_BINARY_DIR}/SDK'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api/xfstkdldrapi.h' '${CMAKE_BINARY_DIR}/SDK'

  # Populate sdk/xfstk-dldr-example-parallel
  COMMAND ${CMAKE_COMMAND} -E make_directory '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel/xfstk-dldr-api-test.pro' '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel/xfstk-dldr-example-parallel.h' '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel/xfstk-dldr-example-parallel.cpp' '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel'

  # Populate sdk/xfstk-dldr-example-serial
  COMMAND ${CMAKE_COMMAND} -E make_directory  '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-serial'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-serial/xfstk-dldr-api-test.pro' '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-serial'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-serial/xfstk-dldr-example-serial.cpp' '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-serial'

  # Populate sdk/xfstk-dldr-example-parallel-dynamic-load
  COMMAND ${CMAKE_COMMAND} -E make_directory  '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel-dynamic-load'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel-dynamic-load/xfstk-dldr-api-test.pro'
    '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel-dynamic-load'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel-dynamic-load/xfstk-dldr-example-parallel.h'
    '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel-dynamic-load'
  COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_SOURCE_DIR}/xfstk-sources/api/SDK/xfstk-dldr-example-parallel-dynamic-load/xfstk-dldr-example-parallel.cpp'
    '${CMAKE_BINARY_DIR}/SDK/xfstk-dldr-example-parallel-dynamic-load'

  # Tar it up  
  COMMAND ${CMAKE_COMMAND} -E tar czf xfstk-sdk-x86_64.tar.gz  '${CMAKE_BINARY_DIR}/SDK')

