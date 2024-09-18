cmake_minimum_required(VERSION 3.2)
 
project(Skyscraper)
 
#set (CMAKE_CXX_FLAGS "-DBT_USE_DOUBLE_PRECISION ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tools/cmake;${CMAKE_MODULE_PATH}")
#if(NOT APPLE)
#	if(UNIX)
#		set(FRONTENDGLINC "-lGL")
#	endif()
#	#if (MSVC)
#		#add_link_options("/SUBSYSTEM:WINDOWS")
#	#endif()
#endif()
 
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

if (WIN32)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_CXX_FLAGS "-D_OGREBULLET_DYNAMIC /MP ${CMAKE_CXX_FLAGS}")
endif ()

#find_package(OGRE REQUIRED)
 
#if(NOT OGRE_FOUND)
  #message(SEND_ERROR "Failed to find OGRE.")
#endif()
 
#set (OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_RTShaderSystem_LIBRARIES} ${OGRE_Bites_LIBRARIES})

#find wxWidgets
find_package(wxWidgets COMPONENTS core base REQUIRED)

#find FMOD
#find_package(FMOD REQUIRED)

#find GTK
#if(NOT APPLE)
#	if(UNIX)
#		find_package(GTK3 3.0 REQUIRED gtk)
#	endif ()
#endif()

#find BULLET
#find_package(Bullet REQUIRED)

#find Caelum
#find_package(Caelum REQUIRED)

file(GLOB FRONTEND_FILES
	src/frontend/skyscraper.cpp
	src/frontend/mainscreen.cpp
	src/frontend/enginecontext.cpp
	src/*.cpp
	#src/frontend/gui/*.cpp
)
 
file(GLOB SCRIPTPROC_FILES
	src/script/*.cpp
)

file(GLOB SBS_FILES
	src/sbs/sbs.cpp
#	src/sbs/*.cpp
)
 
#file(GLOB ALLOC_FILES
#	src/alloc/*.cpp
#)

#file(GLOB GODOT_CPP_FILES
#	godot-cpp/src/*.cpp
#	godot-cpp/src/classes/*.cpp
#	godot-cpp/src/core/*.cpp
#	godot-cpp/src/variant/*.cpp
#)

include_directories(
	src/frontend
	src/sbs
	src/script
	src/alloc
	godot-cpp/include
	godot-cpp/gdextension
	godot-cpp/gen/include
)
 
include(${wxWidgets_USE_FILE})

#SBS library
add_library(SBS SHARED ${SBS_FILES})

#ScriptProc Library
if (NOT WIN32)
	add_library(ScriptProc SHARED ${SCRIPTPROC_FILES})
endif ()

#Alloc Library
#add_library(Alloc SHARED ${ALLOC_FILES})

#Godot-cpp library
#add_library(godot-cpp STATIC ${GODOT_CPP_FILES})

#frontend executable
if (NOT WIN32)
	add_executable(skyscraper ${FRONTEND_FILES})
else ()
	add_executable(skyscraper ${FRONTEND_FILES} ${SCRIPTPROC_FILES})

#	target_compile_options(OgreBulletCol PRIVATE -D_OGREBULLET_EXPORTS "/MD")
#	target_compile_options(OgreBulletDyn PRIVATE -D_OGREBULLET_EXPORTS "/MD")
	target_compile_options(SBS PRIVATE -DSBS_DLL "/MD")
	target_compile_options(skyscraper PRIVATE "/MD")
endif ()

#target_precompile_headers(OgreBulletCol PRIVATE ogrebullet/Collisions/include/OgreBulletCollisionsPreRequisites.h)
#target_precompile_headers(OgreBulletDyn PRIVATE ogrebullet/Dynamics/include/OgreBulletDynamicsPreRequisites.h)
target_precompile_headers(SBS PRIVATE src/sbs/globals.h)

if (NOT WIN32)
	target_precompile_headers(ScriptProc PRIVATE src/sbs/globals.h)
endif ()

target_precompile_headers(skyscraper PRIVATE src/sbs/globals.h)
 
#target_link_libraries(SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${FMOD_LIBRARY})

if (NOT WIN32)
	target_link_libraries(ScriptProc SBS ${GODOT_CPP_LIBRARIES} ${wxWidgets_LIBRARIES})
	target_link_libraries(skyscraper ScriptProc SBS ${GODOT_CPP_LIBRARIES} ${wxWidgets_LIBRARIES})
else ()
	target_link_libraries(skyscraper SBS ${GODOT_CPP_LIBRARIES} ${wxWidgets_LIBRARIES})
endif ()
 
if (APPLE)
set_target_properties(skyscraper PROPERTIES
		MACOSX_BUNDLE ON
		OUTPUT_NAME "Skyscraper"
		BUILD_WITH_INSTALL_RPATH 1
		INSTALL_RPATH "@executable_path/../Frameworks")

ADD_CUSTOM_COMMAND(TARGET skyscraper
		POST_BUILD
		COMMAND ${CMAKE_HOME_DIRECTORY}/tools/mac/copyframework.sh)

#install(CODE "execute_process(COMMAND bash \".macos_app.sh\" \"${OGRE_LIBRARY_DIRS}/macosx/release\")")
endif()

if (APPLE)
	set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
endif ()

#if (NOT WIN32)
#ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
#		POST_BUILD
#		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.sh)
#else ()
#ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
#		POST_BUILD
#		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.cmd)
#endif ()

if (UNIX AND NOT APPLE)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/scripts/skyscraper
	     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
