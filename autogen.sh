#!/bin/sh

rm -rf .deps Makefile Makefile.in aclocal.m4 autom4te.cache bin* build compile config.h config.h.in config.log config.status configure depcomp install-sh missing stamp-h1 ltmain.sh

aclocal && \
autoheader && \
autoconf && \
libtoolize --automake --copy --debug --force 2> /dev/null > /dev/null && \
automake --add-missing --foreign 2> /dev/null

