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
MESSAGE("\n**** Configuring downloader-ui ****")

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
  INCLUDE(${QT_USE_FILE})
  QT4_WRAP_UI(UIC_SOURCES
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkdldrgui.ui
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstksettingsdialog.ui
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkaboutdialog.ui)
  QT4_ADD_RESOURCES(RESOURCES ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkresources.qrc)
ELSE(NOT Qt5_FOUND)
  QT5_WRAP_UI(UIC_SOURCES
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkdldrgui.ui
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstksettingsdialog.ui
      ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkaboutdialog.ui)
  QT5_ADD_RESOURCES(RESOURCES ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkresources.qrc)
ENDIF(NOT Qt5_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -std=c++11")

SET(SRCS
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui
    ${CMAKE_SOURCE_DIR}/xfstk-sources/plugins/downloader)

INCLUDE_DIRECTORIES(${SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR})

ADD_EXECUTABLE(xfstk-dldr-gui
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstk.rc
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/main.cpp
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/main.cpp
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkdldrgui.cpp
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkdldrpluginabstractor.cpp
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstksettingsdialog.cpp
    ${CMAKE_SOURCE_DIR}/xfstk-sources/gui/downloader-ui/xfstkaboutdialog.cpp
    ${UIC_SOURCES}
    ${RESOURCES}
    ${SRCS})

TARGET_LINK_LIBRARIES(xfstk-dldr-gui ${QT_LIBRARIES})

SET_TARGET_PROPERTIES(xfstk-dldr-gui
    PROPERTIES
        AUTOMOC true)

INSTALL(TARGETS xfstk-dldr-gui
    RUNTIME DESTINATION /usr/bin
    COMPONENT gui)
