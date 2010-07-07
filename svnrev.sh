#!/bin/bash
svnrev/svnrev -osrc/revsbs.h -i src/buttonpanel.* src/callbutton.* src/camera.* src/control.* src/directional.* src/door.* src/elevator.* src/elevatordoor.* src/floor.* src/floorindicator.* src/globals.* src/object.* src/sbs.* src/shaft.* src/sound.* src/stairs.* src/unix.* src/wall.*

svnrev/svnrev -osrc/revmain.h -i src/cameracontrol.* src/console.* src/debugpanel.* src/editelevator.* src/fileio.* src/keydialog.* src/meshcontrol.* src/objectinfo.* src/skyscraper.* src/stackwalker.* src/stats.* src/textwindow.* src/uexception.h

