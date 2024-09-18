#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
env.Append(CXXFLAGS=["-Isrc/sbs", "-Isrc/script", "-Isrc/frontend"])
sources = Glob("src/*.cpp")
frontend = Glob("src/frontend/*.cpp")
gui = Glob("src/frontend/gui/*.cpp")
scriptproc = Glob("src/script/*.cpp")
sbs = Glob("src/sbs/*.cpp")

env.ParseConfig('/opt/wx/bin/wx-config --cxxflags --libs')

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libskyscraper.{}.{}.framework/libskyscraper.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources + frontend + scriptproc + sbs + gui,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libskyscraper{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources + frontend + scriptproc + sbs + gui,
    )

Default(library)
