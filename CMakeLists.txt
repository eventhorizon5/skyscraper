cmake_minimum_required(VERSION 3.2)
 
project(Skyscraper)
 
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set (CMAKE_CXX_FLAGS "-DBT_USE_DOUBLE_PRECISION ${CMAKE_CXX_FLAGS}")
endif ()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tools/cmake;${CMAKE_MODULE_PATH}")
if(NOT APPLE)
	if(UNIX)
		set(FRONTENDGLINC "-lGL")
	endif()
endif()
 
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

find_package(OGRE REQUIRED)
 
if(NOT OGRE_FOUND)
  message(SEND_ERROR "Failed to find OGRE.")
endif()
 
set (OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_RTShaderSystem_LIBRARIES} ${OGRE_Bites_LIBRARIES})

#find wxWidgets
find_package(wxWidgets COMPONENTS core base REQUIRED)

#find FMOD
find_package(FMOD REQUIRED)

#find GTK
if(NOT APPLE)
	if(UNIX)
		find_package(GTK3 3.0 REQUIRED gtk)
	endif ()
endif()

#find BULLET
find_package(Bullet REQUIRED)

#find Caelum
find_package(Caelum REQUIRED)

file(GLOB FRONTEND_FILES
	src/frontend/*.cpp
	src/frontend/gui/*.cpp
)
 
file(GLOB SCRIPTPROC_FILES
	src/script/*.cpp
)

file(GLOB SBS_FILES
	src/sbs/*.cpp
)
 
file(GLOB OGREBULLET_COLL_FILES
	ogrebullet/Collisions/src/*.cpp
	ogrebullet/Collisions/src/Debug/*.cpp
	ogrebullet/Collisions/src/Shapes/*.cpp
	ogrebullet/Collisions/src/Utils/*.cpp
)

file (GLOB OGREBULLET_DYN_FILES
	ogrebullet/Dynamics/src/*.cpp
	ogrebullet/Dynamics/src/Constraints/*.cpp
	ogrebullet/Dynamics/src/Prefab/*.cpp
)

include_directories( ${OGRE_INCLUDE_DIRS}
	${OGRE_Overlay_INCLUDE_DIRS}
        ${OGRE_RTShaderSystem_INCLUDE_DIRS}
	${OGRE_Bites_INCLUDE_DIRS}
	${FMOD_INCLUDE_DIR}
	${GTK3_INCLUDE_DIRS}
	${BULLET_INCLUDE_DIRS}
	${Caelum_INCLUDE_DIRS}
	src/frontend
	src/sbs
	src/script
	ogrebullet/Collisions/include
	ogrebullet/Collisions/include/Debug
	ogrebullet/Collisions/include/Shapes
	ogrebullet/Collisions/include/Utils
	ogrebullet/Dynamics/include
	ogrebullet/Dynamics/include/Constraints
	ogrebullet/Dynamics/include/Prefab
)
 
include(${wxWidgets_USE_FILE})

#ogrebullet libraries
add_library(OgreBulletCol SHARED ${OGREBULLET_COLL_FILES})
add_library(OgreBulletDyn SHARED ${OGREBULLET_DYN_FILES})

#SBS library
add_library(SBS SHARED ${SBS_FILES})

#ScriptProc Library
add_library(ScriptProc SHARED ${SCRIPTPROC_FILES})

#frontend executable
add_executable(skyscraper ${FRONTEND_FILES})

target_precompile_headers(OgreBulletCol PRIVATE ogrebullet/Collisions/include/OgreBulletCollisionsPreRequisites.h)
target_precompile_headers(OgreBulletDyn PRIVATE ogrebullet/Dynamics/include/OgreBulletDynamicsPreRequisites.h)
target_precompile_headers(SBS PRIVATE src/sbs/globals.h)
target_precompile_headers(ScriptProc PRIVATE src/sbs/globals.h)
target_precompile_headers(skyscraper PRIVATE src/sbs/globals.h)
 
target_link_libraries(OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})
target_link_libraries(OgreBulletDyn OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})

target_link_libraries(SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${FMOD_LIBRARY})

target_link_libraries(ScriptProc SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES})

target_link_libraries(skyscraper ScriptProc SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} ${FMOD_LIBRARY} ${GTK3_LIBRARIES} ${Caelum_LIBRARIES} ${FRONTENDGLINC})
 
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

ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
		POST_BUILD
		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.sh)

if (UNIX AND NOT APPLE)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/scripts/skyscraper
	     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

