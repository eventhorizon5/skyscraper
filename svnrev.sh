#!/bin/bash
svnrev/svnrev -osrc/sbs/revsbs.h -i src/sbs/*.cpp src/sbs/*.h

svnrev/svnrev -osrc/frontend/revmain.h -i src/frontend/*.cpp src/frontend/*.h

