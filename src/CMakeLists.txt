#------------------------------------------------------------------------------
# <proj_root>/src Subdirectory
#------------------------------------------------------------------------------

# C/C++ Source Files
set(SMARTID_SRCS
    base64.c
    irda.c
    smartid_cmd.c
    smartid_conn.c
    smartid_device.c
    smartid_logging.c
    smartid_main.c
    smartid_server.c
)

# Use C99
add_definitions( -std=c99 )

# Linux needs _XOPEN_SOURCE and POSIX 200809 defined
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
add_definitions( -D_XOPEN_SOURCE -D_POSIX_C_SOURCE=200809L )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Find LibEvent
find_package( Event REQUIRED )

# Configure Header
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/smartid_version.h.in 
    ${CMAKE_CURRENT_BINARY_DIR}/smartid_version.h 
    @ONLY)

# Setup Include Directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	${EVENT_INCLUDE_DIRS}
)

# Build the Application
add_executable(${SMARTID_APP_NAME}
	${SMARTID_SRCS}
)

# Link the Application
target_link_libraries(${SMARTID_APP_NAME}
    ${EVENT_CORE_LIBRARIES}
)

# Install the Application
install(
    TARGETS ${SMARTID_APP_NAME}
    DESTINATION ${SMARTID_SBINDIR}
)
