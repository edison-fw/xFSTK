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
MESSAGE("\n**** Configuring API ****")

FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options)
FIND_PACKAGE(USB01)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
ENDIF(NOT Qt5_FOUND)

SET(INCLUDES ${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/xfstk-sources/core/factory
    ${INCLUDES}
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/ancillary/configure/core/command-line-helper
    ${CMAKE_BINARY_DIR}/ancillary/configure/core/factory)

ADD_LIBRARY(xfstk-dldr-gui-api
    SHARED
        ${INCLUDES}/xfstkdldrapi.cpp
        ${INCLUDES}/xfstkdldrfactory.cpp
        ${INCLUDES}/xfstkdldrthread.cpp)

TARGET_LINK_LIBRARIES(xfstk-dldr-gui-api
    XfstkFactory
    ${USB_LIBRARIES}
    ${Boost_LIBRARIES})

SET_TARGET_PROPERTIES(xfstk-dldr-gui-api
    PROPERTIES
        AUTOMOC true)
		
		
		
ADD_LIBRARY(xfstk-dldr-api
    SHARED
        ${INCLUDES}/xfstkdldrapi.cpp
        ${INCLUDES}/xfstkdldrfactory.cpp
        ${INCLUDES}/xfstkdldrthread.cpp)

TARGET_LINK_LIBRARIES(xfstk-dldr-api
    XfstkFactory
    ${USB_LIBRARIES}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES})

SET_TARGET_PROPERTIES(xfstk-dldr-api
    PROPERTIES
        AUTOMOC true)


INSTALL(TARGETS xfstk-dldr-gui-api
    LIBRARY
        DESTINATION /usr/lib
        NAMELINK_SKIP
        COMPONENT lib)
        
