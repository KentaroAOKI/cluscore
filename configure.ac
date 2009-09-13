#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])

# Sets version number.

# Initializes Autoconf and Automake.
AC_INIT(libcluscore, [0.1.1])
AM_INIT_AUTOMAKE(libcluscore, [0.1.1], no-define)
AC_CONFIG_SRCDIR([cluscore/cc_object.c])
AC_CONFIG_HEADERS([include/config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday memset regcomp strchr strdup])

AC_ARG_ENABLE(opt, [  --ebable-debugmode      turn on debug-mode])
if test "$enable_debugmode" = "yes"; then
	CFLAGS="-g -DLIBCLUSCORE_DEBUG"
else
	CFLAGS="-O3"
fi

AC_SUBST([CFLAGS])
AC_SUBST([LINKLIBS])

AC_CONFIG_FILES([cluscore/makefile
                 test/makefile
                 makefile])
AC_OUTPUT
