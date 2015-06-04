cmake_minimum_required(VERSION 2.6)
 
project(Skyscraper)
 
if(UNIX)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tools/cmake;${CMAKE_MODULE_PATH}")
	if(NOT APPLE)
		set(FRONTENDGLINC "-lGL")
	endif()
endif(UNIX)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
find_package(OGRE REQUIRED)
 
if(NOT OGRE_FOUND)
  message(SEND_ERROR "Failed to find OGRE.")
endif()

find_package(OIS REQUIRED)
 
set (OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Overlay_LIBRARIES})

#find FMOD
find_package(FMOD REQUIRED)

#find GTK
find_package(GTK2 COMPONENTS gtk REQUIRED)

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
 
file(GLOB SVNREV_FILES
	svnrev/*.c
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

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_Overlay_INCLUDE_DIRS}
	${FMOD_INCLUDE_DIR}
	${GTK2_INCLUDE_DIRS}
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
 
#svnrev
add_executable(svnrev/svnrev ${SVNREV_FILES})
if(UNIX)
	add_custom_target(MakeRev ALL ./svnrev.sh)
	add_dependencies(MakeRev svnrev/svnrev)
endif()

#ogrebullet libraries
add_library(OgreBulletCol SHARED ${OGREBULLET_COLL_FILES})
add_library(OgreBulletDyn SHARED ${OGREBULLET_DYN_FILES})

#SBS library
add_library(SBS SHARED ${SBS_FILES})

#frontend executable
add_executable(skyscraper ${FRONTEND_FILES})
 
target_link_libraries(OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})
target_link_libraries(OgreBulletDyn OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})

target_link_libraries(SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${FMOD_LIBRARY})

target_link_libraries(skyscraper SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${FMOD_LIBRARY} ${GTK2_LIBRARIES} ${Caelum_LIBRARIES} ${FRONTENDGLINC})
 
