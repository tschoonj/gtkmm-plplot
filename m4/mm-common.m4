## Copyright (c) 2009  Openismus GmbH  <http://www.openismus.com/>
##
## macros/mm-common.m4.  Generated from mm-common.m4.in by configure.
##
## mm-common is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation, either version 2 of the License,
## or (at your option) any later version.
##
## mm-common is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with mm-common.  If not, see <http://www.gnu.org/licenses/>.

#serial 20090814

## _MM_PRE_INIT
##
## Private pre-initialization macro for use with AC_REQUIRE().  For now,
## all it does is register a forbidden token pattern with autom4te, so
## that unexpanded macro calls in the output can be caught.
##
AC_DEFUN([_MM_PRE_INIT], [m4_pattern_forbid([^_?MM_])])

## _MM_PREREQ(this-package, this-version, min-version, user-package)
##
m4_define([_MM_PREREQ],
[dnl
m4_if(m4_quote(m4_version_compare([$2], [$3])), [-1],
      [m4_fatal([$4 requires $1 $3 (version $2 is installed)])])[]dnl
])

## MM_PREREQ(min-version)
##
## Require at least mm-common <min-version> to be installed, otherwise
## abort with a fatal error message.  The version is checked statically
## at the time the configure script is generated.
##
AC_DEFUN([MM_PREREQ],
[dnl
m4_assert([$# >= 1])[]dnl
AC_REQUIRE([_MM_PRE_INIT])[]dnl
_MM_PREREQ([mm-common], [0.9.8], [$1], m4_defn([AC_PACKAGE_NAME]))[]dnl
])
