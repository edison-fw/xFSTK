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
MESSAGE("\n**** Configuring Baytrail Plugin ****")

# Find ui files.
file(GLOB GUI_UI ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/baytrail/*.ui)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
  INCLUDE(${QT_USE_FILE})
  QT4_WRAP_UI(UIC_SOURCES ${GUI_UI})
ELSE(NOT Qt5_FOUND)
  QT5_WRAP_UI(UIC_SOURCES ${GUI_UI})
ENDIF(NOT Qt5_FOUND)

SET(BAY     ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/baytrail)
SET(PLUG    ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader)
SET(API     ${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api)

# Show Cmake where the header files are.
INCLUDE_DIRECTORIES(
    ${BAY}
    ${PLUG}
    ${API}
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(XfstkDldrPluginBaytrail
    SHARED
        ${BAY}/xfstkdldrpluginuserinterface.cpp
        ${BAY}/xfstkdldrpluginoptionsinterface.cpp
        ${BAY}/xfstkdldrpluginbaytrail.cpp
        ${BAY}/xfstkdldrthread.cpp
        ${UIC_SOURCES})

target_link_libraries(XfstkDldrPluginBaytrail
    xfstk-command-line
    XfstkFactory
    xfstk-dldr-gui-api
    ${QT_LIBRARIES})

SET_TARGET_PROPERTIES(XfstkDldrPluginBaytrail
    PROPERTIES
        AUTOMOC true)

INSTALL(TARGETS XfstkDldrPluginBaytrail
    LIBRARY DESTINATION /usr/lib/xfstk/xfstkdldrplugins
        COMPONENT gui)
