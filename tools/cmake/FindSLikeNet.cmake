# Comes form project edunetgames
# originally FindRakNet.cmake
# - Try to find SLikeNet/SLikeNet
# Once done this will define
#
#  SLikeNet_FOUND - system has SLikeNet
#  SLikeNet_INCLUDES - the SLikeNet include directory
#  SLikeNet_LIBRARY - Link these to use SLikeNet

FIND_LIBRARY (SLikeNet_LIBRARY_RELEASE NAMES SLikeNetLibStatic
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    $ENV{SLIKENET_ROOT}/lib
    )
	
FIND_LIBRARY (SLikeNet_LIBRARY_DEBUG NAMES SLikeNetLibStatic
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    $ENV{SLIKENET_ROOT}/lib
    )	
	
	

FIND_PATH (SLikeNet_INCLUDES slikenet/peer.h
    ENV CPATH
    /usr/include
    /usr/local/include
    /opt/local/include
	$ENV{SLIKENET_ROOT}/include
    )
 
IF(SLikeNet_INCLUDES AND SLikeNet_LIBRARY_RELEASE)
    SET(SLikeNet_FOUND TRUE)
ENDIF(SLikeNet_INCLUDES AND SLikeNet_LIBRARY_RELEASE)

IF(SLikeNet_FOUND)
  SET(SLikeNet_INCLUDES ${SLikeNet_INCLUDES})
  
  
   IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(SLikeNet_LIBRARY optimized ${SLikeNet_LIBRARY_RELEASE} debug ${SLikeNet_LIBRARY_DEBUG})
      ELSE()
        # if there are no configuration types and CMAKE_BUILD_TYPE has no value
        # then just use the release libraries
        SET(SLikeNet_LIBRARY ${SLikeNet_LIBRARY_RELEASE} )
      ENDIF()
  IF(NOT SLikeNet_FIND_QUIETLY)
    MESSAGE(STATUS "Found SLikeNet: ${SLikeNet_LIBRARIES}")
  ENDIF(NOT SLikeNet_FIND_QUIETLY)
ELSE(SLikeNet_FOUND)
  IF(SLikeNet_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find SLikeNet")
  ENDIF(SLikeNet_FIND_REQUIRED)
ENDIF(SLikeNet_FOUND)
