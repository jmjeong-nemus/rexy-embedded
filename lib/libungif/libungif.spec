Summary: A library for manipulating GIF format image files.
Name: libungif
Version: 4.1.4
Release: 1
License: MIT
URL: http://sourceforge.net/projects/libungif/
Source: http://dl.sourceforge.net/%{name}/%{name}-%{version}.tar.bz2
Group: System Environment/Libraries
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
# The following libgif.so handles packages built against the
# previous broken giflib package
Provides: libgif.so giflib
Obsoletes: giflib

%description
The libungif package contains a shared library of functions for loading
and saving GIF format image files.  The libungif library can load any
GIF file, but it will save GIFs only in uncompressed format (i.e., it
won't use the patented LZW compression used to save "normal" compressed
GIF files).

Install the libungif package if you need to manipulate GIF files.  You
should also install the libungif-progs package.

%package devel
Summary: Development tools for programs which will use the libungif library.
Group: Development/Libraries
Requires: %{name} = %{version}

%description devel
This package contains the static libraries, header files and documentation
necessary for development of programs that will use the libungif library
to load and save GIF format image files.

You should install this package if you need to develop programs which
will use the libungif library functions for loading and saving GIF format
image files.  You'll also need to install the libungif package.

%package progs
Summary: Programs for manipulating GIF format image files.
Group: Applications/Multimedia
Requires: %{name} = %{version}

%description progs
The libungif-progs package contains various programs for manipulating
GIF format image files.

Install this package if you need to manipulate GIF format image files.
You'll also need to install the libungif package.

%prep
%setup

%build
%configure
make all

%install
rm -rf ${RPM_BUILD_ROOT}

%{makeinstall}
ln -sf libungif.so.%{version} ${RPM_BUILD_ROOT}%{_libdir}/libgif.so.%{version}
ln -sf libgif.so.%{version} ${RPM_BUILD_ROOT}%{_libdir}/libgif.so.4
ln -sf libgif.so.4 ${RPM_BUILD_ROOT}%{_libdir}/libgif.so
ln -sf libungif.a ${RPM_BUILD_ROOT}%{_libdir}/libgif.a

chmod 755 ${RPM_BUILD_ROOT}%{_libdir}/*.so*
chmod 644 ${RPM_BUILD_ROOT}%{_libdir}/*.a*
chmod 644 COPYING README UNCOMPRESSED_GIF NEWS ONEWS
chmod 644 ChangeLog TODO BUGS AUTHORS
chmod 644 doc/* util/giffiltr.c util/gifspnge.c

%clean
rm -rf ${RPM_BUILD_ROOT}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc COPYING README UNCOMPRESSED_GIF NEWS ONEWS
%doc ChangeLog TODO BUGS AUTHORS
%{_libdir}/lib*.so.*

%files devel
%defattr(-,root,root)
%doc doc/* util/giffiltr.c util/gifspnge.c
%{_libdir}/lib*.a
%{_libdir}/lib*.so
%{_libdir}/lib*.la
%{_includedir}/gif_lib.h

%files progs
%attr(0755 root root) %{_bindir}/*

%changelog
* Sat May 29 2004 Toshio Kuratomi <toshio@tiki-lounge.com> - 4.1.3-1
- Upgrade to version 4.1.3

* Tue Feb 17 2004 Toshio Kuratomi <toshio@tiki-lounge.com> - 4.1.1-1
- Upgrade to version 4.1.1
- Make the spec more like the standard RH spec with a few fedora-extras
  flourishes thrown in.
  
* Tue Jan 19 1999 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 4.1
  + Fix a few minor memory leaks in error conditions.
  + Add a new function EGifOpen( void *userData, OutputFunc writeFunc) that
    allows user specified gif writing function.
- Merge spec file from libungif-3.1-5

* Mon Dec 14 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 4.0 (Fixes rather nasty behaviour when dealing with
  Extensions.)

* Fri Oct 9 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 3.1.1 (Fixes some segfaults and memory leaks when dealing
  with extensions and colourmaps.)

* Mon Sep 7 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 3.1.0 (which incorporates the patches in 3.0-4)
- Updated Source: and URL: to reflect change in directories/pages.

* Tue May 26 1998 Dick Porter <dick@cymru.net>
- Fixed some "warning: cast to pointer from integer of different size" on Alpha

* Tue May 5 1998 Marc Ewing <marc@redhat.com>
- Made it obsolete giflib and provide libgif.so and giflib (previous
  giflib packages were built incorrectly and packages built against
  it require libgif.so but work fine with this package)
- cleaned buildroot
- Removed Toshio as packager so he doesn't get yelled at when Red Hat
  breaks it :-)

* Fri Apr 24 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Initial revision of libungif, a giflib derivative modified to not use LZW
  compression.
