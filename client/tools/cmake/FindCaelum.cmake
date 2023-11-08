#-------------------------------------------------------------------
# This file is part of the CMake build system for USUL
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Locate Caelum Ogre plugin
# This module defines
#  Caelum_FOUND        - True when the Caelum include directory is found.
#  Caelum_INCLUDE_DIRS - the Caelum include files.
#  Caelum_LIBRARIES    - the Caelum library files.
#
# CAELUM_ROOT is an environment variable that would be set for windows users.
#
# Created by Jean-François VERDON. This was influenced by the FindBoost.cmake module.

SET(Caelum_INCLUDE_PATH_DESCRIPTION "the base directory containing the Caelum include files. E.g /usr/Caelum/include or C:/MyLibs/Caelum")
SET(Caelum_DIR_MESSAGE "Set the Caelum_INCLUDE_DIR cmake cache entry to ${Caelum_INCLUDE_PATH_DESCRIPTION}")

#
# Look for include files.
#
SET(Caelum_DIR_SEARCH /usr/include
                      /usr/local/include
                      $ENV{CAELUM_ROOT}/include)

FIND_PATH(Caelum_INCLUDE_DIR NAMES Caelum.h PATH_SUFFIXES Caelum PATHS ${Caelum_DIR_SEARCH} DOC "The directory should contain Caelum includes")
SET(Caelum_INCLUDE_DIRS ${Caelum_INCLUDE_DIR})

#
# Look for library files.
#
SET(Caelum_LIB_DIR_SEARCH
   /usr/lib
   /usr/local/lib
   $ENV{CAELUM_ROOT}/lib
)

FIND_LIBRARY(Caelum_LIBRARY NAMES Caelum PATH_SUFFIXES Release Debug PATHS ${Caelum_LIB_DIR_SEARCH})
SET(Caelum_LIBRARIES ${Caelum_LIBRARY})

# Set CMake return value
set (Caelum_FOUND "FALSE")
if (Caelum_INCLUDE_DIRS AND Caelum_LIBRARIES)
   set (Caelum_FOUND "TRUE")
endif (Caelum_INCLUDE_DIRS AND Caelum_LIBRARIES)

