#------------------------------------------------------------------------------
# Master CMake File for the Smart-I Daemon Application
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
project(smartid)

# Application Version Information
set(SMARTID_MAJOR_VERSION 1)
set(SMARTID_MINOR_VERSION 0)
set(SMARTID_PATCH_VERSION 3)
set(SMARTID_VERSION
	${SMARTID_MAJOR_VERSION}.${SMARTID_MINOR_VERSION}.${SMARTID_PATCH_VERSION})

set(SMARTID_VERSION_STRING "${SMARTID_VERSION}")

# Application Name/Copyright Info
set(SMARTID_APP_NAME "smartid")
set(SMARTID_APP_TITLE "Smart-I Daemon")
set(SMARTID_COPYRIGHT "Copyright (c) 2011-2014 Asymworks, LLC.  All Rights Reserved.")

# Application Host Processor Info
execute_process(COMMAND uname -m OUTPUT_VARIABLE SMARTID_MACHINE OUTPUT_STRIP_TRAILING_WHITESPACE)

# Default Listening Port
set(SMARTID_DEFAULT_PORT 6740)

# Setup Local CMake Modules
set(CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH} 
	${CMAKE_SOURCE_DIR}/cmake/modules/
)

# Setup Application Paths
SET(SMARTID_DATADIR		    "${CMAKE_INSTALL_PREFIX}/share/${SMARTID_APP_NAME}")
SET(SMARTID_MANDIR		    "${CMAKE_INSTALL_PREFIX}/share/man")
SET(SMARTID_BINDIR          "${CMAKE_INSTALL_PREFIX}/bin")
SET(SMARTID_SBINDIR         "${CMAKE_INSTALL_PREFIX}/sbin")

SET(SMARTID_PIDFILE		    "/var/run/${SMARTID_APP_NAME}.pid")

# Setup SysV Init Script
configure_file(
    ${CMAKE_SOURCE_DIR}/dist/sysvinit-smartid.in 
    ${CMAKE_BINARY_DIR}/sysvinit-smartid 
    @ONLY)

# Run Subdirectories
add_subdirectory(src)

# Install top-level files
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${SMARTID_DATADIR}/)
install(FILES ${CMAKE_SOURCE_DIR}/README DESTINATION ${SMARTID_DATADIR}/)

# Install SysV init script
install(
    FILES ${CMAKE_BINARY_DIR}/sysvinit-smartid 
    DESTINATION /etc/init.d/${SMARTID_APP_NAME}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_EXECUTE             GROUP_READ
                WORLD_EXECUTE             WORLD_READ
)