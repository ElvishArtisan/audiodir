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


AM_CPPFLAGS = -Wall @TAGLIB_INCLUDES@

libexec_PROGRAMS = audiodir.cgi
audiodir_cgi_SOURCES = audiodir.cpp cgilib.cpp conflib.cpp filelist.cpp\
                       audiodir.h conflib.h cgilib.h filelist.h
audiodir_cgi_LDADD = @TAGLIB_LIBS@ -lz
audiodir_cgi_LDFLAGS = @STATIC_LDFLAG@

EXTRA_DIST = autogen.sh audiodir.spec.in audiodir.conf-sample get_target.sh\
             get_distro.sh

rpm:	dist
	mkdir -p $(HOME)/rpmbuild/SOURCES
	cp audiodir-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux audiodir.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/audiodir-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/audiodir-$(VERSION)-@RPM_RELEASE@.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/audiodir-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/audiodir-$(VERSION)

CLEANFILES = *~
DISTCLEANFILES = depcomp install-sh missing mkinstalldirs audiodir.spec
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 *.tar.gz *.rpm
