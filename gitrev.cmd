@echo off
echo Generating GitRev header...
echo #ifndef GITREV_H> src\sbs\gitrev.h
echo #define GITREV_H>> src\sbs\gitrev.h
echo.>> src\sbs\gitrev.h
echo|set /p="#define GIT_REV ">> src\sbs\gitrev.h
git rev-list HEAD | find /c /v "">> src\sbs\gitrev.h
echo.>> src\sbs\gitrev.h
echo #endif>> src\sbs\gitrev.h
