cmake_minimum_required(VERSION 3.1)
project(OpenGLTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmakes")
include(Common)

set(CMAKE_SUPPRESS_REGENERATION FALSE)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Sets the configuration to build (Debug, Release, etc...)")
endif()

# Compile and Link settings
set(COMPILE_AND_LINK_EXTRA_FLAGS "")
if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -fsigned-char")
elseif(NOT CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_C_STANDARD_REQUIRED TRUE)
    set(CMAKE_C_STANDARD "11")
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_STANDARD "11")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -Wall -Wextra -Wundef")
endif ()

option (WARNINGS_AS_ERRORS "Specifies whether to treat warnings as errors. Recommended at developing time." OFF)
if (WARNINGS_AS_ERRORS)
    if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -Werror")
    elseif (MSVC)
        set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} /WX")
    else ()
        message (WARNING "You've specified \"WARNINGS_AS_ERRORS\" as \"ON\", but we don't know how to make it for your compiler. If you wish you can specifiy the required flags manually.")
    endif ()
endif ()

if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()
# Activating the default multi-processor build setting for all Visual Studio versions
if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")

# set build output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

if (WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#cmake_dependent_option( BUILD_MSVC_STATIC_RUNTIME "Specifies whether to the static runtime (/MT and /MTd) or the DLL runtime (/MD and /MDd).
#NOTE: This will also affect which set of dependency libraries are linked with." FALSE "MSVC" FALSE )
#if (WIN32)
#    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
#    if (MSVC AND BUILD_MSVC_STATIC_RUNTIME)
#        foreach(_BUILD_CONFIG DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
#            foreach(_BUILD_VARS CMAKE_CXX_FLAGS_${_BUILD_CONFIG} CMAKE_C_FLAGS_${_BUILD_CONFIG})
#                string(REGEX REPLACE /MD /MT ${_BUILD_VARS} ${${_BUILD_VARS}})
#            endforeach()
#        endforeach()
#    endif()
#endif()

if (WIN32 OR APPLE)
    set(BUILD_SUFFIX "_d" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
else()
    set(BUILD_SUFFIX "" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
endif()

if (BUILD_SUFFIX)
    set(HAS_BUILD_SUFFIX TRUE)
    set(CMAKE_DEBUG_POSTFIX ${BUILD_SUFFIX})
endif()

## Search for ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(ENV(CCACHE_CPP2) "yes")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (WIN32)
    #find_package(WindowsSDK)
    message ("CMAKE_VS_PLATFORM_NAME: ${CMAKE_VS_PLATFORM_NAME}")
    message ("CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION: ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

    #set(WinSDK C:/Program Files (x86)/Windows Kits/10)

    get_filename_component(WindowsSDK_ROOT "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE)
    message(STATUS "WindowsSDK_ROOT: ${WindowsSDK_ROOT}")

    list(APPEND WindowsSDK_INCLUDE_DIR ${WindowsSDK_ROOT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared)
    list(APPEND WindowsSDK_INCLUDE_DIR ${WindowsSDK_ROOT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um)
    list(APPEND WindowsSDK_INCLUDE_DIR ${WindowsSDK_ROOT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/winrt)
    list(APPEND WindowsSDK_INCLUDE_DIR ${WindowsSDK_ROOT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt)

    list(APPEND WindowsSDK_LIB_DIR ${WindowsSDK_ROOT}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared/${CMAKE_VS_PLATFORM_NAME})
    list(APPEND WindowsSDK_LIB_DIR ${WindowsSDK_ROOT}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/${CMAKE_VS_PLATFORM_NAME})
    list(APPEND WindowsSDK_LIB_DIR ${WindowsSDK_ROOT}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/winrt/${CMAKE_VS_PLATFORM_NAME})
    list(APPEND WindowsSDK_LIB_DIR ${WindowsSDK_ROOT}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt/${CMAKE_VS_PLATFORM_NAME})

    message ("WindowsSDK include dir: ${WindowsSDK_INCLUDE_DIR}")
    message ("WindowsSDK library dir: ${WindowsSDK_LIB_DIR}")
    include_directories(${WindowsSDK_INCLUDE_DIR})
    link_directories(${WindowsSDK_LIB_DIR})

    add_definitions(-D_WIN32 -D_MBCS)
endif ()

include_directories(
    ${CMAKE_SOURCE_DIR}/dev/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/dev/lib
)

SET( COMMON
    dev/include/glad.c
    dev/include/stb_image.h
    src/common/json_load.cpp
    src/common/json_load.h
    src/common/lib_utils.cpp
    src/common/lib_utils.h
    src/common/math.h
    src/common/singleton.h
)

SET( ENGINE
    src/engine/camera.cpp
    src/engine/camera.h
    src/engine/engine.cpp
    src/engine/engine.h
)

SET( RENDER
    src/render/model.cpp
    src/render/model.h
    src/render/model_manager.cpp
    src/render/model_manager.h
    src/render/render.cpp
    src/render/render.h
    src/render/render_object.cpp
    src/render/render_object.h
    src/render/shader.cpp
    src/render/shader.h
    src/render/shader_manager.cpp
    src/render/shader_manager.h
    src/render/texture.cpp
    src/render/texture.h
)

SOURCE_GROUP( OpenGLTest\\Common        FILES  ${COMMON} )
SOURCE_GROUP( OpenGLTest\\Engine        FILES  ${ENGINE} )
SOURCE_GROUP( OpenGLTest\\Render        FILES  ${RENDER} )

add_executable( OpenGLTest
    ${COMMON}
    ${ENGINE}
    ${RENDER}
    src/main.cpp
)

target_link_libraries(OpenGLTest glew32 glfw3 soil2-debug assimpd)

target_include_directories(OpenGLTest PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(OpenGLTest PRIVATE ${CMAKE_SOURCE_DIR}/src/engine)
target_include_directories(OpenGLTest PRIVATE ${CMAKE_SOURCE_DIR}/src/render)

if (MSVC)
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6.0")
        set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
    endif ()
endif (MSVC)
