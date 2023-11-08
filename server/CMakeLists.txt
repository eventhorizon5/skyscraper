cmake_minimum_required(VERSION 3.2)
 
project(Server)
 
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set (CMAKE_CXX_FLAGS "-DBT_USE_DOUBLE_PRECISION ${CMAKE_CXX_FLAGS}")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
if (UNIX AND NOT APPLE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

#find_package(OGRE REQUIRED)
 
#if(NOT OGRE_FOUND)
#  message(SEND_ERROR "Failed to find OGRE.")
#endif()
 
#set (OGRE_LIBRARIES ${OGRE_LIBRARIES})

file(GLOB MAIN_FILES
	src/main/*.cpp
)
 
file(GLOB SBS_FILES
	src/sbs/*.cpp
)
 
file(GLOB SCRIPT_FILES
	src/script/*.cpp
)

include_directories( ${OGRE_INCLUDE_DIRS}
	src/main
	src/sbs
	src/script
)
 
#SBS library
#add_library(SBS SHARED ${SBS_FILES})

#Script Processor Library
#add_library(ScriptProc SHARED ${SCRIPT_FILES})

#frontend executable
add_executable(server ${MAIN_FILES})

#target_precompile_headers(SBS PRIVATE src/sbs/globals.h)
#target_precompile_headers(ScriptProc PRIVATE src/sbs/globals.h)
#target_precompile_headers(server PRIVATE src/sbs/globals.h)
 
#target_link_libraries(SBS ${OGRE_LIBRARIES})
#target_link_libraries(ScriptProc SBS)
#target_link_libraries(server SBS ${OGRE_LIBRARIES})
target_link_libraries(server)
 
#ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
#		POST_BUILD
#		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.sh)

#if (UNIX AND NOT APPLE)
#	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/scripts/skyscraper
#	     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endif()

