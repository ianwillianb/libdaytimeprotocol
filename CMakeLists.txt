cmake_minimum_required(VERSION 3.5)
project(daytimemessageprotocol)

set(CMAKE_CXX_STANDARD 17)

# Find and use pkg-config
find_package(PkgConfig REQUIRED)

# Use pkg-config to find and set the appropriate flags for libtftp-client
pkg_check_modules(TFTP_CLIENT REQUIRED IMPORTED_TARGET tftp-client)

# Source files
set(SRC_FILES
    DaytimeMessageProtocolClient.cpp
    DaytimeMessageProtocolServer.cpp
)

# Header files
set(HEADER_FILES
    DaytimeMessageProtocolClient.h
    DaytimeMessageProtocolServer.h
)

# Shared library
add_library(daytimemessageprotocol SHARED ${SRC_FILES})
target_include_directories(daytimemessageprotocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#Link libtftp-client
target_link_libraries(daytimemessageprotocol PkgConfig::TFTP_CLIENT)

# Static library
add_library(daytimemessageprotocol_static STATIC ${SRC_FILES})
target_include_directories(daytimemessageprotocol_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(daytimemessageprotocol_static PkgConfig::TFTP_CLIENT)

# Option to build examples
option(BUILD_EXAMPLES "Build example executables" OFF)

if(BUILD_EXAMPLES)
    # Example executable
    add_executable(NIST-daytime examples/NIST-daytime.cpp)
    # Link the NIST-daytime executable with tftp-client
    target_link_libraries(NIST-daytime PRIVATE daytimemessageprotocol_static)
    # Set the output directory for the NIST-daytime executable
    set_target_properties(NIST-daytime PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/built-examples)
    
    add_executable(daytime-server examples/daytime-server.cpp)
    # Link the daytime-server executable with tftp-client
    target_link_libraries(daytime-server PRIVATE daytimemessageprotocol_static)
    # Set the output directory for the NIST-daytime executable
    set_target_properties(daytime-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/built-examples)
endif()

# Installation
install(TARGETS daytimemessageprotocol daytimemessageprotocol_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/daytimemessageprotocol)


# Create and install the pkg-config file
configure_file(DaytimeMessageProtocol.pc.in DaytimeMessageProtocol.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/DaytimeMessageProtocol.pc
        DESTINATION lib/pkgconfig)

