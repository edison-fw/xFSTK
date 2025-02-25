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
MESSAGE("\n**** Configuring Plugin: Cloverview ****")

set(UIFILES
    ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/cloverview/xfstkdldrpluginuserinterface.ui
    ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/cloverview/xfstkdldrpluginoptionsinterface.ui)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
  INCLUDE(${QT_USE_FILE})
  QT4_WRAP_UI(UIC_SOURCES ${UIFILES})
ELSE(NOT Qt5_FOUND)
  QT5_WRAP_UI(UIC_SOURCES ${UIFILES})
ENDIF(NOT Qt5_FOUND)

SET(CV      ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader/cloverview)
SET(DLDR    ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader)
SET(DA      ${CMAKE_SOURCE_DIR}/xfstk-sources/api/downloader-api)

# Show Cmake where the header files are.
INCLUDE_DIRECTORIES(
    ${CV}
    ${DLDR}
    ${DA}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR})

ADD_LIBRARY(XfstkDldrPluginCloverview
    SHARED
        ${CV}/xfstkdldrpluginuserinterface.cpp
        ${CV}/xfstkdldrpluginoptionsinterface.cpp
        ${CV}/xfstkdldrplugincloverview.cpp
        ${CV}/xfstkdldrthread.cpp
        ${UIC_SOURCES})

TARGET_LINK_LIBRARIES(XfstkDldrPluginCloverview
    xfstk-command-line
    XfstkFactory
    ${QT_LIBRARIES}
    xfstk-dldr-gui-api)

SET_TARGET_PROPERTIES(XfstkDldrPluginCloverview
    PROPERTIES
        AUTOMOC true)

INSTALL(TARGETS XfstkDldrPluginCloverview
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/xfstk/xfstkdldrplugins
        COMPONENT gui)
