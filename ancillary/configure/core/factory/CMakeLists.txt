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
MESSAGE("\n**** Configuring Factory ****")

SET(INC ${CMAKE_SOURCE_DIR}/xfstk-sources/core/factory)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
SET(QT_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
IF(NOT Qt5_FOUND)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtWidgets)
  INCLUDE(${QT_USE_FILE})
ENDIF(NOT Qt5_FOUND)

FIND_PACKAGE(USB01)

INCLUDE_DIRECTORIES(${INC}
    ${INC}/platforms/cloverviewplus
    ${INC}/platforms/cloverview
    ${INC}/platforms/emmc
    ${INC}/platforms/hashverify
    ${INC}/platforms/medfield
    ${INC}/platforms/merrifield
    ${INC}/platforms/softfuses
    ${INC}/interfaces
    ${INC}/common
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${USB_INCLUDE_DIRS})

SET(SRCS
    ${INC}/xfstkfactory.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusdevice.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusdownloader.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusfw.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusoptions.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusutils.cpp
    ${INC}/platforms/cloverviewplus/cloverviewplusos.cpp
    ${INC}/platforms/cloverviewplus/clvpdldrstate.cpp
    ${INC}/platforms/cloverview/cloverviewutils.cpp
    ${INC}/platforms/cloverview/cloverviewdownloader.cpp
    ${INC}/platforms/cloverview/cloverviewdevice.cpp
    ${INC}/platforms/cloverview/cloverviewfw.cpp
    ${INC}/platforms/cloverview/cloverviewos.cpp
    ${INC}/platforms/cloverview/cloverviewoptions.cpp
    ${INC}/platforms/cloverview/clvdldrstate.cpp
    ${INC}/platforms/medfield/medfieldutils.cpp
    ${INC}/platforms/medfield/medfieldoptions.cpp
    ${INC}/platforms/medfield/medfielddownloader.cpp
    ${INC}/platforms/medfield/medfielddevice.cpp
    ${INC}/platforms/medfield/medfieldfw.cpp
    ${INC}/platforms/medfield/dldrstate.cpp
    ${INC}/platforms/medfield/medfieldos.cpp
    ${INC}/platforms/merrifield/merrifieldutils.cpp
    ${INC}/platforms/merrifield/merrifieldoptions.cpp
    ${INC}/platforms/merrifield/merrifielddownloader.cpp
    ${INC}/platforms/merrifield/merrifielddevice.cpp
    ${INC}/platforms/merrifield/merrifieldfw.cpp
    ${INC}/platforms/merrifield/mrfdldrstate.cpp
    ${INC}/platforms/merrifield/merrifieldos.cpp
    ${INC}/platforms/hashverify/hashverifyoptions.cpp
    ${INC}/platforms/hashverify/hashverifydownloader.cpp
    ${INC}/platforms/hashverify/hashverifyutils.cpp
	${INC}/platforms/baytrail/baytraildevice.cpp
	${INC}/platforms/baytrail/baytraildownloader.cpp
	${INC}/platforms/baytrail/baytrailfw.cpp
	${INC}/platforms/baytrail/baytrailoptions.cpp
	${INC}/platforms/baytrail/baytrailutils.cpp		
    ${INC}/platforms/softfuses/softfusesfw.cpp
    ${INC}/platforms/emmc/emmcoptions.cpp
    ${INC}/platforms/emmc/emmcstate.cpp
    ${INC}/platforms/emmc/emmcdownloader.cpp
    ${INC}/platforms/emmc/emmcutils.cpp
    ${INC}/platforms/emmc/emmcfw.cpp)

ADD_LIBRARY(XfstkFactory 
     STATIC
        ${SRCS})

TARGET_LINK_LIBRARIES(XfstkFactory
    xfstk-command-line
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    ${USB_LIBRARIES})
			
SET_TARGET_PROPERTIES(XfstkFactory
    PROPERTIES
        AUTOMOC true)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86_64 AND CMAKE_COMPILER_IS_GNUCXX )
    SET_TARGET_PROPERTIES(XfstkFactory
        PROPERTIES
            COMPILE_FLAGS "-fPIC")
endif ()

 
