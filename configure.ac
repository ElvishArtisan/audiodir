dnl configure.in
dnl 
dnl Use autoconf to process this into a configure script
dnl   (C) Copyright 2002-2007,2014 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT(audiodir,0.4.0)
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_PROG_CXX

#
# Option Switches
#
AC_ARG_ENABLE(static,[  --enable-static         enable static build],
		      [STATIC_ENABLED=yes],[])
if test -z $STATIC_ENABLED ; then
  AC_SUBST(STATIC_LDFLAG,"")
else
  AC_SUBST(STATIC_LDFLAG,"-static")
fi

#
# Check for Qt
#
BNV_HAVE_QT
if test $have_qt = "no" ; then
  AC_MSG_ERROR([*** Qt toolkit not found, unable to continue ***])
fi

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine Distro
#
AR_GET_DISTRO()
if test $ar_distro_name = "RedHat" ; then
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(VENDOR,"suse")
fi

#
# TagLib Library
#
AC_CHECK_PROG(HAVE_TAGLIB,taglib-config,[yes],[],[$PATH:$prefix/bin])
if test -z $HAVE_TAGLIB ; then
  AC_MSG_ERROR([*** TagLib not found ***])
else
  AC_SUBST(TAGLIB_INCLUDES,`taglib-config --cflags`)
  AC_SUBST(TAGLIB_LIBS,`taglib-config --libs`)
fi


#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
AC_SUBST(RPM_DATESTAMP,`date +%a\ %b\ %d\ %Y`)


AC_OUTPUT(Makefile \
  audiodir.spec \
)
