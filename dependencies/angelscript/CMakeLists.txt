cmake_minimum_required(VERSION 3.16)
#project(AngelScriptVendor LANGUAGES CXX)

# Options you may want to control from the root (they’ll appear in the cache)
option(ANGELSCRIPT_BUILD_SHARED "Build AngelScript as a shared library" ON)
option(ANGELSCRIPT_MAX_PORTABILITY "Enable AS_MAX_PORTABILITY" ON)

set(AS_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(AS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB AS_SOURCES CONFIGURE_DEPENDS "${AS_SRC_DIR}/*.cpp")

set(libkind STATIC)
if(ANGELSCRIPT_BUILD_SHARED)
  set(libkind SHARED)
endif()

add_library(AngelScript ${libkind} ${AS_SOURCES})
#add_library(AngelScript::AngelScript ALIAS AngelScript)

# Put all AngelScript artifacts in the parent build folder
if(UNIX AND NOT APPLE)
set_target_properties(AngelScript PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>/lib"
)
else()
set_target_properties(AngelScript PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<0:>"
)
endif()

target_include_directories(AngelScript
  PUBLIC
    "${AS_INCLUDE_DIR}"
)

if(ANGELSCRIPT_MAX_PORTABILITY)
  target_compile_definitions(AngelScript PUBLIC AS_MAX_PORTABILITY)
endif()

# Reasonable warnings (adjust as you like)
if(MSVC)
  target_compile_options(AngelScript PRIVATE /W3 /permissive-)
else()
  target_compile_options(AngelScript PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(MSVC AND ANGELSCRIPT_BUILD_SHARED)
  # target-scoped property (CMake ≥3.4)
  set_target_properties(AngelScript PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
