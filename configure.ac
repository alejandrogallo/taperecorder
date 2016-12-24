# Process this file with autoconf to produce a configure script.

AC_INIT([Taperecorder],
        [esyscmd(./version.sh)],
        [aamsgallo@gmail.com],
        [taperecorder],
        [https://github.com/alejandrogallo/taperecorder])
AC_COPYRIGHT([GPLv3])

AC_LANG([C])


AM_INIT_AUTOMAKE([1.8.2 gnu dist-xz])

dnl configure checks for this file's existence to make sure that the directory
dnl that it is told contains the source code in fact does.
AC_CONFIG_SRCDIR([src/main.c])

AM_CONFIG_HEADER([config.h])

# Checks for programs

AC_PROG_AWK
AC_PROG_CC
AC_ISC_POSIX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_CC_STDC
AC_C_TYPEOF


# Checks for libraries.

PKG_CHECK_MODULES([LIBCONFIG], [libconfig >= 1.4],,
  AC_MSG_ERROR([libconfig 1.4 or newer not found.])
)


# Checks for header files.

AC_HEADER_STDC
AC_CHECK_HEADERS([libconfig.h \
                  regex.h \
                  getopt.h \
                  unistd.h \
                  alloca.h \
                  errno.h \
                  limits.h \
                  malloc.h \
                  stddef.h \
                  stdlib.h \
                  string.h \
                  sys/param.h \
                  unistd.h])


# Checks for typedefs, structures, and compiler characteristics.

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([mkdir remove memset strcmp strcpy strcat malloc])



# Generate files

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
