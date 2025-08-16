# - Locate IMGUI library
# This module defines
#  IMGUI_LIBRARY, the library to link against
#  IMGUI_FOUND, if false, do not try to link to IMGUI
#  IMGUI_INCLUDE_DIR, where to find headers.

IF(IMGUI_LIBRARY AND IMGUI_INCLUDE_DIR)
  # in cache already
  SET(IMGUI_FIND_QUIETLY TRUE)
ENDIF(IMGUI_LIBRARY AND IMGUI_INCLUDE_DIR)


FIND_PATH(IMGUI_INCLUDE_DIR
  imgui.h
  PATHS
  $ENV{IMGUI_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  "C:/Program Files (x86)/IMGUI SoundSystem/IMGUI Studio API Windows/api/core/inc"
  PATH_SUFFIXES imgui
)

FIND_LIBRARY(IMGUI_LIBRARY
  NAMES imgui libimgui imgui_vc
  PATHS
  $ENV{IMGUI_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  "C:/Program Files (x86)/IMGUI SoundSystem/IMGUI Studio API Windows/api/core/lib/x64"
)

IF(IMGUI_LIBRARY AND IMGUI_INCLUDE_DIR)
  SET(IMGUI_FOUND "YES")
  IF(NOT IMGUI_FIND_QUIETLY)
    MESSAGE(STATUS "Found ImGUI: ${IMGUI_LIBRARY}")
  ENDIF(NOT IMGUI_FIND_QUIETLY)
ELSE(IMGUI_LIBRARY AND IMGUI_INCLUDE_DIR)
  IF(NOT IMGUI_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find ImGUI!")
  ENDIF(NOT IMGUI_FIND_QUIETLY)
ENDIF(IMGUI_LIBRARY AND IMGUI_INCLUDE_DIR)

