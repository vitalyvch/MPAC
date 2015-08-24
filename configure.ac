#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lmpac':
AC_CHECK_LIB([mpac], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h float.h limits.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
# AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_CHECK_FUNCS([clock_gettime gethostbyaddr gethostbyname gettimeofday inet_ntoa memset pow select socket sqrt strstr])

AC_CONFIG_FILES([Makefile
                 benchmarks/Makefile
                 benchmarks/cpu/Makefile
                 benchmarks/mem/Makefile
                 benchmarks/net/Makefile
                 common/Makefile
                 tests/Makefile
                 tests/common_tests/Makefile])
AC_OUTPUT
