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
MESSAGE("\n**** Configuring Plugin: Medfield ****")

# Find ui files.
FILE(GLOB GUI_UI ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/medfield/*.ui)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
  INCLUDE(${QT_USE_FILE})
  QT4_WRAP_UI(UIC_SOURCES ${GUI_UI})
ELSE(NOT Qt5_FOUND)
  QT5_WRAP_UI(UIC_SOURCES ${GUI_UI})
ENDIF(NOT Qt5_FOUND)

SET(MED ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/medfield)
SET(DLDR ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader)
SET(DAPI ${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api)

# Show Cmake where the header files are.
INCLUDE_DIRECTORIES(
    ${MED}
    ${DLDR}
    ${DAPI}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR})

# Tell Cmake which sources to compile.
SET (SRCS
    ${MED}/xfstkdldrpluginuserinterface.cpp
    ${MED}/xfstkdldrpluginoptionsinterface.cpp
    ${MED}/xfstkdldrpluginmedfield.cpp
    ${MED}/xfstkdldrthread.cpp)

ADD_LIBRARY(XfstkDldrPluginMedfield
    SHARED
        ${MED}/xfstkdldrpluginuserinterface.cpp
        ${MED}/xfstkdldrpluginoptionsinterface.cpp
        ${MED}/xfstkdldrpluginmedfield.cpp
        ${MED}/xfstkdldrthread.cpp
        ${UIC_SOURCES})

target_link_libraries(XfstkDldrPluginMedfield
    xfstk-command-line
    XfstkFactory
    ${QT_LIBRARIES}
    xfstk-dldr-gui-api)

SET_TARGET_PROPERTIES(XfstkDldrPluginMedfield
    PROPERTIES
        AUTOMOC true)
                            
INSTALL(TARGETS XfstkDldrPluginMedfield
    LIBRARY DESTINATION /usr/lib/xfstk/xfstkdldrplugins
        COMPONENT gui)
