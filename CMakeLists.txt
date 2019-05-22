# Top-level CMakeLists.txt file for Downloader.
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

# Cmake 2.8 provides built-in automoc capabilities so let's use that version.
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11 FATAL_ERROR)

MESSAGE("\n**** Starting Downloader Build Configuration ****")

# The project can be referred to by ${PROJECT_NAME}.
PROJECT("xfstk-downloader" "CXX")

# CMake code can be separated into different files called modules. CMake provided
# modules are at CMAKE_MODULE_PATH. Our modules are in cmake_modules.
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/ancillary/cmake_modules)

# BEGIN C++11 feature checks. These are supported on GCC >= 4.5 and Visual Studio 10.
INCLUDE(CheckCXX11Features)
MESSAGE("-- The following C++ features are required:")

MACRO(require FEATURE_NAME MESSAGE_STRING)
	IF (NOT ${FEATURE_NAME})
		MESSAGE(FATAL_ERROR "\t-- ${MESSAGE_STRING} fail!")
	ELSE()
		MESSAGE("\t-- ${MESSAGE_STRING} ok")
	ENDIF()
ENDMACRO(require)

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

if ("${LIB64}" STREQUAL "TRUE")
    set(LIBSUFFIX 64)
else()
    set(LIBSUFFIX "")
endif()

# ** When a C++11 feature is introduced into the code, uncomment the equiviliant line so ensure compatability. **
# require(HAS_CXX11_LONG_LONG "C++11 lambdas\t\t\t")
# require(HAS_CXX11_AUTO "C++11 auto support:\t\t\t")
# require(HAS_CXX11_VARIADIC_TEMPLATES "C++11 variadic templates\t\t")
# require(HAS_CXX11_NULLPTR "C++11 nullptr support:\t\t")
# require(HAS_CXX11_RVALUE_REFERENCES "C++11 rvalue reference support\t")
# require(HAS_CXX11_CSTDINT_H "C++11 stdint support\t\t\t")
# require(HAS_CXX11_RVALUE_REFERENCES "C++11 rvalue references\t\t")
# require(HAS_CXX11_LONG_LONG "C++11 long long\t\t\t")
# END C++11 feature checks

# BEGIN Check build version number. This number gets set in the code and package name.
IF (DEFINED ENV{BUILD_VERSION})
	MESSAGE("++ Build version is $ENV{BUILD_VERSION} ++")
	ADD_DEFINITIONS("-DDOWNLOADER_VERSION=\"$ENV{BUILD_VERSION}\"")
ELSE()
	MESSAGE(FATAL_ERROR "-- BUILD_VERSION is not set!")
ENDIF()
# END Check build version number.

# BEGIN Default compiler args
IF (NOT CMAKE_BUILD_TYPE)
	MESSAGE("-- Defaulting to release build (use -DCMAKE_BUILD_TYPE:STRING=Debug for debug build)")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

INCLUDE(FindCCache)

# Append stuff to CMAKE_CXX_FLAGS
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXFSTK_MULTIDEVICE")
SET(CMAKE_CXX_FLAGS_DEBUG		"-O0 -g")
SET(CMAKE_CXX_FLAGS_MINSIZEREL		"-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"-O2 -g")
# END Default compiler args

# BEGIN the actual project configure.
# Make a convenience variable to shorten the path name.
SET(CONFIGDIR ${CMAKE_SOURCE_DIR}/ancillary/configure)
ADD_SUBDIRECTORY(${CONFIGDIR}/core/command-line-helper)
ADD_SUBDIRECTORY(${CONFIGDIR}/core/factory)
ADD_SUBDIRECTORY(${CONFIGDIR}/core/downloader-solo)
ADD_SUBDIRECTORY(${CONFIGDIR}/api/downloader-api)
ADD_SUBDIRECTORY(${CONFIGDIR}/gui/downloader-ui)
ADD_SUBDIRECTORY(${CONFIGDIR}/plugins/downloader/cloverview)
ADD_SUBDIRECTORY(${CONFIGDIR}/plugins/downloader/cloverviewplus)
ADD_SUBDIRECTORY(${CONFIGDIR}/plugins/downloader/medfield)
ADD_SUBDIRECTORY(${CONFIGDIR}/plugins/downloader/merrifield)
ADD_SUBDIRECTORY(${CONFIGDIR}/plugins/downloader/baytrail)
# END the actual project configure.

# Generate documentation.
INCLUDE(GenerateDocumentation) 

# Assemble the SDK
INCLUDE(BuildSDK)

# Package the software.
INCLUDE(buildPackages)

