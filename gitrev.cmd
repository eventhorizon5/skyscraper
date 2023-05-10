erase src\sbs\gitrev.h
echo #ifdef GITREV_H >> src\sbs\gitrev.h
echo #define GITREV_H >> src\sbs\gitrev.h
echo. >> src\sbs\gitrev.h
git rev-list HEAD | find /c /v "" > out
set /p VV=<out
echo #define GIT_REV %VV% >> src\sbs\gitrev.h
erase out
echo #endif >> src\sbs\gitrev.h
