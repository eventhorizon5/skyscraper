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
 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set proper rpath on Mac
if (APPLE)
	set (CMAKE_INSTALL_RPATH "@executable_path/Libs")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
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
	src/sbs
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

#frontend executable
add_executable(skyscraper ${FRONTEND_FILES})
 
target_link_libraries(OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})
target_link_libraries(OgreBulletDyn OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})

target_link_libraries(SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${FMOD_LIBRARY})

target_link_libraries(skyscraper SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} ${FMOD_LIBRARY} ${GTK3_LIBRARIES} ${Caelum_LIBRARIES} ${FRONTENDGLINC})
 
