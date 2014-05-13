## automake.am
##
## Automake.am for the AudioDir package.
##
## Use automake to process this into a Makefile.in
##   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
##
##      $Id: Makefile.am,v 1.6 2008/03/07 18:50:04 fredg Exp $
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##


pkglibdir = /srv/www/cgi-bin
AM_CFLAGS = -Wall
INCLUDES = @TAGLIB_INCLUDES@

install-exec-local:	
	cp audiodir.cgi $(pkglibdir)

uninstall-local:
	rm -f $(pkglibdir)/audiodir.cgi

noinst_PROGRAMS = audiodir.cgi
audiodir_cgi_SOURCES = audiodir.cpp cgilib.cpp conflib.cpp filelist.cpp\
                       audiodir.h conflib.h cgilib.h filelist.h
audiodir_cgi_LDADD = @TAGLIB_LIBS@

EXTRA_DIST = autogen.sh audiodir.spec.in audiodir.conf get_target.sh\
             get_distro.sh

rpm:	dist
	cp audiodir-$(VERSION).tar.gz $(RPM_ROOT)/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux audiodir.spec
	mv $(RPM_ROOT)/RPMS/$(ARCH)/audiodir-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(RPM_ROOT)/SRPMS/audiodir-$(VERSION)-@RPM_RELEASE@.src.rpm $(top_srcdir)/
	rm $(RPM_ROOT)/SOURCES/audiodir-$(VERSION).tar.gz
	rm -rf $(RPM_ROOT)/BUILD/audiodir-$(VERSION)

CLEANFILES = *~
DISTCLEANFILES = depcomp install-sh missing mkinstalldirs audiodir.spec
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 *.tar.gz
