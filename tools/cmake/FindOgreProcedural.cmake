include(FindPkgMacros)
include(PreprocessorUtils)

findpkg_begin(OgreProcedural)

getenv_path(OgreProcedural_HOME)
getenv_path(PROGRAMFILES)
getenv_path(OgreProcedural_SOURCE)

# construct search paths from environmental hints and
# OS specific guesses
if (WIN32)
  set(OgreProcedural_PREFIX_GUESSES
    ${ENV_PROGRAMFILES}/OgreProcedural
    C:/ogre-procedural
  )
elseif (UNIX)
  set(OgreProcedural_PREFIX_GUESSES
    /opt/procedural
    /opt/OgreProcedural
    /usr/lib${LIB_SUFFIX}/procedural
    /usr/lib${LIB_SUFFIX}/OgreProcedural
    /usr/local/lib${LIB_SUFFIX}/procedural
    /usr/local/lib${LIB_SUFFIX}/OgreProcedural
    $ENV{HOME}/procedural
    $ENV{HOME}/OgreProcedural
  )
endif ()
set(OgreProcedural_PREFIX_PATH
  ${OgreProcedural_HOME} ${OgreProcedural_SDK} ${ENV_OgreProcedural_HOME} ${ENV_OgreProcedural_SDK}
  ${OgreProcedural_PREFIX_GUESSES}
)
create_search_paths(OgreProcedural)

# If both OgreProcedural_BUILD and OgreProcedural_SOURCE are set, prepare to find Ogre in a build dir
set(OgreProcedural_PREFIX_SOURCE ${OgreProcedural_SOURCE} ${ENV_OgreProcedural_SOURCE})
set(OgreProcedural_PREFIX_BUILD ${OgreProcedural_BUILD} ${ENV_OgreProcedural_BUILD})
set(OgreProcedural_PREFIX_DEPENDENCIES_DIR ${OgreProcedural_DEPENDENCIES_DIR} ${ENV_OgreProcedural_DEPENDENCIES_DIR})
if (OgreProcedural_PREFIX_SOURCE AND OgreProcedural_PREFIX_BUILD)
  foreach(dir ${OgreProcedural_PREFIX_SOURCE})
    set(OgreProcedural_INC_SEARCH_PATH ${dir}/library/include ${OgreProcedural_INC_SEARCH_PATH})
    set(OgreProcedural_LIB_SEARCH_PATH ${dir}/lib ${OgreProcedural_LIB_SEARCH_PATH})
  endforeach(dir)
  foreach(dir ${OgreProcedural_PREFIX_BUILD})
    set(OgreProcedural_INC_SEARCH_PATH ${dir}/include ${OgreProcedural_INC_SEARCH_PATH})
    set(OgreProcedural_LIB_SEARCH_PATH ${dir}/lib ${OgreProcedural_LIB_SEARCH_PATH})
    set(OgreProcedural_BIN_SEARCH_PATH ${dir}/bin ${OgreProcedural_BIN_SEARCH_PATH})
  endforeach(dir)
else()
  set(OgreProcedural_PREFIX_SOURCE "NOTFOUND")
  set(OgreProcedural_PREFIX_BUILD "NOTFOUND")
endif ()

set(OgreProcedural_PREFIX_SOURCE ${OgreProcedural_SOURCE} ${ENV_OgreProcedural_SOURCE})
set(OgreProcedural_PREFIX_BUILD ${OgreProcedural_BUILD} ${ENV_OgreProcedural_BUILD})


set(OgreProcedural_LIBRARY_NAMES "OgreProcedural")
get_debug_names(OgreProcedural_LIBRARY_NAMES)

find_path(OgreProcedural_INCLUDE_DIR NAMES OgreProcedural.h HINTS ${OgreProcedural_CONFIG_INCLUDE_DIR} ${OgreProcedural_INC_SEARCH_PATH} ${OgreProcedural_FRAMEWORK_INCLUDES} ${OgreProcedural_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "OgreProcedural")

find_library(OgreProcedural_LIBRARY_REL NAMES ${OgreProcedural_LIBRARY_NAMES} HINTS ${OgreProcedural_LIB_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(OgreProcedural_LIBRARY_DBG NAMES ${OgreProcedural_LIBRARY_NAMES_DBG} HINTS ${OgreProcedural_LIB_SEARCH_PATH} PATH_SUFFIXES "" "debug")
make_library_set(OgreProcedural_LIBRARY)

findpkg_finish(OgreProcedural)
