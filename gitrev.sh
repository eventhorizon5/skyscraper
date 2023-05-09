#!/bin/sh
rm src/sbs/gitrev.h > /dev/null 2>&1
echo \#ifndef GITREV_H >> src/sbs/gitrev.h
echo \#define GITREV_H >> src/sbs/gitrev.h
echo >> src/sbs/gitrev.h
echo \#define GIT_REV	`git rev-list HEAD | wc -l` >> src/sbs/gitrev.h
echo \#endif >> src/sbs/gitrev.h

