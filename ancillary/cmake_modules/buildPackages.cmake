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

include(GNUInstallDirs)

MESSAGE("\n**** Begin Packaging Configuration ****")

SET(CPACK_PACKAGE_CONTACT		"Intel Corporation <xfstk@intel.com>")
SET(CPACK_PACKAGE_VENDOR		"Intel Corp")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY	"Used to provision firmware to Intel phones.")
SET(CPACK_RPM_PACKAGE_DESCRIPTION	"xFSTK Downloader is a cross platform tool that provides a simple, consistent interface "
					  "to perform firmware and OS downloads to an Intel SoC device.")
 
SET(CPACK_PACKAGE_VERSION $ENV{BUILD_VERSION})

# NOTE http://cmake.3232098.n2.nabble.com/Better-handling-of-library-dependencies-for-CPack-td6296476.html
# NOTE: cmake --help-module CPackRPM or cmake --help-module CPackDeb
IF (${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86_64)
        SET(ARCH "amd64")
ELSE()
        SET(ARCH "i386")
ENDIF ()

IF (DEFINED ENV{INTERNAL_BUILD})
        SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-$ENV{BUILD_VERSION}.$ENV{DISTRIBUTION_NAME}_internal-${ARCH}")
ELSE()
        SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-$ENV{BUILD_VERSION}.$ENV{DISTRIBUTION_NAME}-${ARCH}")
ENDIF()



SET(SRC "xfstk-sources/installer/linux/xfstk-downloader-0.0.0")
SET(DEST "${CMAKE_BINARY_DIR}/_CPack_Packages/Linux/DEB/${CPACK_PACKAGE_FILE_NAME}")

# Copy udev files
LIST(APPEND CPACK_INSTALL_COMMANDS
	"${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${SRC}/lib ${DEST}/lib")
# END copy udev file

# Need special permissions on Intel.ini so every user can edit it. This should then go in the user's home
# directory but we'll try to set open permssions on it for now.
INSTALL(FILES "${SRC}/usr/lib/xfstk/xfstkdldrplugins/Intel.ini"
	DESTINATION  "${CMAKE_INSTALL_LIBDIR}/xfstk/xfstkdldrplugins"
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE GROUP_WRITE WORLD_WRITE
  COMPONENT gui)

INSTALL(FILES "${SRC}/usr/share/applications/xfstk-downloader.desktop"
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/applications"
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE GROUP_WRITE WORLD_WRITE
  COMPONENT gui)

INSTALL(FILES "${SRC}/usr/bin/xfstk-ui-launcher.sh"
  	DESTINATION  "${CMAKE_INSTALL_BINDIR}"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  COMPONENT gui)
	
INSTALL(FILES "${SRC}/usr/share/pixmaps/xfstklogo.png"
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/pixmaps"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  COMPONENT gui)
	
INSTALL(FILES "${PROJECT_BINARY_DIR}/xfstk-sdk-x86_64.tar.gz"
	DESTINATION  "${CMAKE_INSTALL_LIBDIR}/xfstk"
  COMPONENT gui)
	
# and make sure the docs are in the package.
INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/docs/html"
	DESTINATION  "${CMAKE_INSTALL_DOCDIR}/xfstk-downloader"
  COMPONENT lib)

SET(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST
  /lib
  /usr
  /usr/bin
  /usr/lib
  /usr/share
  /usr/share/applications
  /usr/share/doc
  /usr/share/pixmaps)
  


# Produce packages only for the distro we are building on. So, Fedora will build fedora packages and
# Ubuntu will build Ubuntu packages.
IF (NOT DEFINED ENV{DISTRIBUTION_NAME})
  # Set up dependencies per OS. The version numbers are weird. I wonder how ubuntu3 will evaluate to a number.
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.46.1 (>= 1.46.1-7ubuntu3), libusb-0.1-4 (>= 2:0.1.12-20)") 
    #add ubuntu 12.04 deps rules here.
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
ELSE()
  MESSAGE("+++ Creating package for $ENV{DISTRIBUTION_NAME}")

  IF ($ENV{DISTRIBUTION_NAME} MATCHES "ubuntu12.04")
    # Set up dependencies per OS. The version numbers are weird. I wonder how ubuntu3 will evaluate to a number.
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.46.1 (>= 1.46.1-7ubuntu3), libusb-0.1-4 (>= 2:0.1.12-20)") 
    #add ubuntu 12.04 deps rules here.
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSEIF ($ENV{DISTRIBUTION_NAME} MATCHES "ubuntu13.10")
    # Set up dependencies per OS. The version numbers are weird. I wonder how ubuntu3 will evaluate to a number.
    # SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.49.0 (>= 1.49.0-4), libusb-0.1-4 (>= 2:0.1.12-20), libusb-dev, libusb-0.1-4, qt4-default, qt4-dev, libboost-program-options-dev")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.53.0")
    #add ubuntu 12.04 deps rules here.
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSEIF ($ENV{DISTRIBUTION_NAME} MATCHES "ubuntu14.04")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.55.0, libusb-0.1-4")
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSEIF ($ENV{DISTRIBUTION_NAME} MATCHES "ubuntu16.04")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.58.0, libusb-0.1-4")
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSEIF ($ENV{DISTRIBUTION_NAME} MATCHES "ubuntu20.04")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.71.0, libusb-0.1-4")
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSEIF ($ENV{DISTRIBUTION_NAME} MATCHES "debian")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.74.0, libusb-0.1-4")
    SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
  ELSE()
     MESSAGE(FATAL_ERROR "ERROR: Environment variable: DISTRIBUTION_NAME ($ENV{DISTRIBUTION_NAME}) doesn't match anything.")
  ENDIF()
ENDIF()


INCLUDE(CPack)


