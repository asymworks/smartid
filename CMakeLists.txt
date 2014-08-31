#------------------------------------------------------------------------------
# Master CMake File for the Smart-I Daemon Application
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
project(smartid)

# Application Version Information
set(SMARTID_MAJOR_VERSION 1)
set(SMARTID_MINOR_VERSION 0)
set(SMARTID_PATCH_VERSION 2)
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
SET(SMARTID_PIDFILE		"/var/run/${SMARTID_APP_NAME}.pid")

# Run Subdirectories
add_subdirectory(src)
