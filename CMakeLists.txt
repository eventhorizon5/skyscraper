cmake_minimum_required(VERSION 3.2)
 
project(Skyscraper)

set(CMAKE_AUTORCC ON)

set (CMAKE_CXX_FLAGS "-DBT_USE_DOUBLE_PRECISION ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tools/cmake;${CMAKE_MODULE_PATH}")
if(NOT APPLE)
	if(UNIX)
		set(FRONTENDGLINC "-lGL")
	endif()
	#if (MSVC)
		#add_link_options("/SUBSYSTEM:WINDOWS")
	#endif()
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

if (WIN32)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}$<0:>)
	set(CMAKE_CXX_FLAGS "-D_OGREBULLET_DYNAMIC /MP ${CMAKE_CXX_FLAGS}")
endif ()

find_package(OGRE REQUIRED)
 
if(NOT OGRE_FOUND)
  message(SEND_ERROR "Failed to find OGRE.")
endif()
 
set (OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_RTShaderSystem_LIBRARIES} ${OGRE_Bites_LIBRARIES})

#find OgreProcedural
find_package(OgreProcedural REQUIRED)

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

#find OpenXR
find_package(OpenXR)

if(WIN32)
	file(GLOB FRONTEND_FILES
		src/frontend/*.cpp
		src/frontend/gui/*.cpp
		msvc/Skyscraper.rc
	)
else()
	file(GLOB FRONTEND_FILES
                src/frontend/*.cpp
                src/frontend/gui/*.cpp
        )
endif()
 
file(GLOB VM_FILES
	src/vm/*.cpp
)

file(GLOB SCRIPTPROC_FILES
	src/script/*.cpp
)

file(GLOB SBS_FILES
	src/sbs/*.cpp
	msvc/SBS.rc
)
 
file(GLOB ALLOC_FILES
	src/alloc/*.cpp
)

file(GLOB OPENXR_FILES
	src/openxr/src/*.cpp
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
	${OpenXR_INCLUDE_DIR}
	${OgreProcedural_INCLUDE_DIR}
	src/frontend
	src/vm
	src/sbs
	src/script
	src/alloc
	src/openxr/include
	src/openxr
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
if (NOT WIN32)
	add_library(ScriptProc SHARED ${SCRIPTPROC_FILES})
endif ()

#Alloc Library
add_library(Alloc SHARED ${ALLOC_FILES})

#OpenXR Library
if (WIN32)
	add_library(OpenXR SHARED ${OPENXR_FILES})
endif ()

#frontend executable
add_executable(skyscraper ${FRONTEND_FILES})

if (NOT WIN32)
	add_library(VM SHARED ${VM_FILES})
else ()
	add_library(VM SHARED ${VM_FILES} ${SCRIPTPROC_FILES})
	target_compile_options(OgreBulletCol PRIVATE -D_OGREBULLET_EXPORTS "/MD")
	target_compile_options(OgreBulletDyn PRIVATE -D_OGREBULLET_EXPORTS "/MD")
	target_compile_options(OpenXR PRIVATE -DOGREOPENXRDLL_EXPORTS "/MD")
	target_compile_options(SBS PRIVATE -DSBS_DLL "/MD")
	target_compile_options(VM PRIVATE -DVM_EXPORTS "/MD")
	target_compile_options(skyscraper PRIVATE "/MD")
endif ()

target_precompile_headers(OgreBulletCol PRIVATE ogrebullet/Collisions/include/OgreBulletCollisionsPreRequisites.h)
target_precompile_headers(OgreBulletDyn PRIVATE ogrebullet/Dynamics/include/OgreBulletDynamicsPreRequisites.h)
target_precompile_headers(SBS PRIVATE src/sbs/globals.h)
target_precompile_headers(VM PRIVATE src/sbs/globals.h)

if (NOT WIN32)
	target_precompile_headers(ScriptProc PRIVATE src/sbs/globals.h)
endif ()

target_precompile_headers(skyscraper PRIVATE src/sbs/globals.h)
 
target_link_libraries(OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})
target_link_libraries(OgreBulletDyn OgreBulletCol ${OGRE_LIBRARIES} ${BULLET_LIBRARIES})

if (WIN32)
	set (OGRE_D3D11_LIB ${OGRE_DIR}/../../lib/RelWithDebInfo/RenderSystem_Direct3D11.lib)
	target_link_libraries(OpenXR ${OGRE_LIBRARIES} ${OPENXR_loader_LIBRARY} ${OGRE_D3D11_LIB})
endif ()

target_link_libraries(SBS OgreBulletCol OgreBulletDyn ${OGRE_LIBRARIES} ${FMOD_LIBRARY} ${OgreProcedural_LIBRARIES})

if (NOT WIN32)
	target_link_libraries(ScriptProc SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES})
	target_link_libraries(VM SBS ${OGRE_LIBRARIES} ${Caelum_LIBRARIES})
	target_link_libraries(skyscraper VM ScriptProc SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} ${FMOD_LIBRARY} ${GTK3_LIBRARIES} ${FRONTENDGLINC})
else ()
	target_link_libraries(VM SBS OpenXR ${OGRE_LIBRARIES} ${Caelum_LIBRARIES} ${OPENXR_loader_LIBRARY})
	target_link_libraries(skyscraper VM SBS ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} ${FMOD_LIBRARY} ${GTK3_LIBRARIES} ${FRONTENDGLINC})
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

if (NOT WIN32)
ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
		POST_BUILD
		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.sh)
else ()
ADD_CUSTOM_COMMAND(TARGET OgreBulletDyn
		POST_BUILD
		COMMAND ${CMAKE_HOME_DIRECTORY}/gitrev.cmd)
endif ()

if (APPLE)
ADD_CUSTOM_COMMAND(TARGET SBS
		POST_BUILD
		COMMAND install_name_tool -change libOgreProcedural.dylib "@rpath/libOgreProcedural.dylib" libSBS.dylib)

ADD_CUSTOM_COMMAND(TARGET ScriptProc
                POST_BUILD
                COMMAND install_name_tool -change libOgreProcedural.dylib "@rpath/libOgreProcedural.dylib" libScriptProc.dylib)

ADD_CUSTOM_COMMAND(TARGET skyscraper
		POST_BUILD
		COMMAND install_name_tool -change libOgreProcedural.dylib "@rpath/libOgreProcedural.dylib" Skyscraper.app/Contents/MacOS/Skyscraper)
endif ()

if (UNIX AND NOT APPLE)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/scripts/skyscraper
	     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
