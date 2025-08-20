cmake_minimum_required(VERSION 3.16)

# Options you may want to control from the root (they’ll appear in the cache)
option(OGREPROCEDURAL_BUILD_SHARED "Build OgreProcedural as a shared library" ON)

set(OP_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(OP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB OP_SOURCES CONFIGURE_DEPENDS "${OP_SRC_DIR}/*.cpp")

set(libkind STATIC)
if(OGREPROCEDURAL_BUILD_SHARED)
  set(libkind SHARED)
endif()

add_library(OgreProcedural ${libkind} ${OP_SOURCES})

target_include_directories(OgreProcedural
  PUBLIC
    "${OP_INCLUDE_DIR}/OgreProcedural"
    "${OGRE_INCLUDE_DIRS}"
)

if(MSVC)
  target_compile_options(OgreProcedural PRIVATE /W3 /permissive- /MD)
else()
  target_compile_options(OgreProcedural PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(MSVC AND OGREPROCEDURAL_BUILD_SHARED)
  # target-scoped property (CMake ≥3.4)
  set_target_properties(OgreProcedural PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_link_libraries(OgreProcedural ${OGRE_LIBRARIES})

