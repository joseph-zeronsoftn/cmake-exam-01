cmake_minimum_required(VERSION 3.16)
project(test-application)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

message("Target System = ${CMAKE_SYSTEM_NAME}")
include(compiler-detects.cmake)

if (TARGET_IS_WINDOWS)
    set(CMAKE_C_FLAGS "-static ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
endif()

include(libraries.cmake)

set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sdlpp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sdlpp.cc
        )

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/font.o
        COMMAND ${CMAKE_OBJCOPY_EXECUTABLE}
        --input-target binary
        --output-target ${BUILD_OBJCOPY_TARGET_TYPE}
        --binary-architecture ${BUILD_OBJCOPY_BINARY_ARCH}
        font.ttf ${CMAKE_CURRENT_BINARY_DIR}/font.o
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res
)

if (TARGET_IS_WINDOWS)
    list(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/windows/app.rc)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/windows/app.rc PROPERTIES
            OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/windows/app.manifest.xml")
endif()

set(APP_NAME test-app)

add_executable(${APP_NAME} ${SRC_FILES} ${CMAKE_CURRENT_BINARY_DIR}/font.o)

target_link_options(${APP_NAME}
        PUBLIC
        -static
        )

if (TARGET_IS_WINDOWS)
    target_link_libraries(${APP_NAME}
            PUBLIC
            mingw32
            )
endif()
target_link_libraries(${APP_NAME}
        PUBLIC
        SDL2-static
        SDL2_ttf
        SDL2main
        )
if (VIDEO_X11)
    target_link_libraries(${APP_NAME}
            PUBLIC
            X11
            Xext
            xcb
            Xau
            Xdmcp
            )
endif()
