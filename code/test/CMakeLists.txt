# Copyright 2024 GlitchyByte
# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.26)

set(IS_MACOS CMAKE_SYSTEM_NAME STREQUAL "Darwin")
set(IS_LINUX CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(IS_WINDOWS CMAKE_SYSTEM_NAME STREQUAL "Windows")

if (NOT CMAKE_BUILD_TYPE)
    # Force MinSizeRel if no build type specified.
    set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Type of build." FORCE)
endif ()

# Main binary to build.
set(APP "gbtest")

#file(STRINGS "../../version" APP_VERSION LIMIT_COUNT 1)
project(${APP} VERSION "${APP_VERSION}")
message(STATUS "Project: ${APP} v${APP_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

# Set C++ standard and binary output dir.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Set flags.
if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    if (${IS_LINUX} OR ${IS_MACOS})
        # Linux and macOS.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -ffunction-sections -fdata-sections")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffunction-sections -fdata-sections")
    elseif (${IS_WINDOWS})
        # Windows has not been tested.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GL")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LTCG")
    else ()
        message(WARNING "Unknown OS.")
    endif ()
endif ()

# Test framework.
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/9d43b27f7a873596496a2ea70721b3f9eb82df01.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Add all cpp files in src dir.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "../src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "/src/main.cpp$")
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cc")
add_executable(${APP} ${SOURCES} ${TEST_SOURCES})

if (${IS_MACOS})
    target_link_libraries(${APP}
            PUBLIC "-framework CoreFoundation"
            PUBLIC "-framework CoreServices"
    )
endif ()

# Add all h files in include dir.
target_include_directories(${APP} PRIVATE "../include")

# Add GoogleTest libraries.
target_link_libraries(${APP} PUBLIC GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${APP})

# Minimize binary size.
if ((CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel") AND
(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    # Strip all remaining symbols and relocation information.
    add_custom_command(
            TARGET ${APP}
            POST_BUILD
            COMMAND strip $<TARGET_FILE:${APP}>
    )
endif ()
