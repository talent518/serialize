#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([bin/serialize], [1.0], [talent518@yeah.net])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([serialize], [1.0])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h strings.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([gettimeofday memchr memset strchr strtol strtoul])

# zlib is required
AC_ARG_WITH(glib, [  --with-glib=DIR         where to find the glib-2.0 library, default /usr], , [with_glib=/usr])
CFLAGS="-I$with_glib/include/glib-2.0 $CFLAGS"
if test -d "$with_glib/lib64/glib-2.0/include"; then
	CFLAGS="-I$with_glib/lib64/glib-2.0/include $CFLAGS"
	LDFLAGS="-L$with_glib/lib64 $LDFLAGS"
else
	CFLAGS="-I$with_glib/lib/glib-2.0/include $CFLAGS"
	LDFLAGS="-L$with_glib/lib $LDFLAGS"
fi
PKG_CHECK_MODULES(glib, glib-2.0 >= 2.40.0)

AC_CHECK_LIB(glib-2.0, g_hash_table_new, [glib_LIB=yes], [glib_LIB=no])

if test x$glib_LIB = xno; then
	AC_CHECK_HEADERS([glib.h],[
		GLIB_STATIC_LIB="-lglib-2.0 -lintl -liconv"
		AC_SUBST(GLIB_STATIC_LIB)
	],[
		AC_MSG_ERROR([In -lglib-2.0 library not found function g_hash_table_new])
	])
else
	AC_CHECK_HEADERS([glib.h],[
		LDFLAGS="-lglib-2.0"
		GLIB_STATIC_LIB=
		AC_SUBST(GLIB_STATIC_LIB)
	],[
		AC_MSG_ERROR([In -lglib-2.0 library not found function g_hash_table_new])
	])
fi

AC_ARG_ENABLE(debug, [  --enable-debug turn on debug], CFLAGS="$CFLAGS -O0 -DDEBUG")
AC_ARG_ENABLE(bench, [  --enable-bench bench test], CFLAGS="$CFLAGS -O3 -DBENCH")

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
