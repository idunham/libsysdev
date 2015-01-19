# Makefile for libsysdev
# Written by and copyright Isaac Dunham, in the year of our Lord 2015
# No rights reserved - see LICENSE for details.


# set DESTDIR if you're packaging, PREFIX to install somewhere
DESTDIR ?=
PREFIX  ?= /usr
LIBDIR  ?= ${PREFIX}/lib
BINDIR  ?= ${PREFIX}/bin
INCDIR  ?= ${PREFIX}/include

# set AR and CC if you're cross-compiling
AR     ?= ar
# We assume SUSv4, but set _ATFILE_SOURCE for old glibc versions
XFLAGS += -D_XOPEN_SOURCE=700 -D_ATFILE_SOURCE -I.
CFLAGS ?= -Os -g
# so the user/packager can select -fpic if they want
PIC     = -fPIC
# so the user/packager can select whether to build shared/static/both
LIBS   ?= libsysdev.a libsysdev.so.${SOVER}

# This should not be modified unless you are adding code or
# releasing a new version.
# SOVER is the soversion; only modify it when libsysdev no longer provides
# the same ABI.
# SOMIN is the minor version; update for a release with a new symbol export
# AROBJS/SOOBJS should be updated to add new source files.
SOVER   = 0
SOMIN   = 0
AROBJS  = getprodids.o  \
	getsyspath.o    \
	convenience.o
SOOBJS  = getprodids.lo \
	getsyspath.lo   \
	convenience.lo


all: ${LIBS} devinfo

install: ${LIBS} devinfo
	install -d -m 0755 ${DESTDIR}${LIBDIR}
	install -d -m 0755 ${DESTDIR}${BINDIR}
	install -d -m 0755 ${DESTDIR}${INCDIR}/libsysdev
# installing the shared lib?
	echo ${LIBS} | grep libsysdev.so && \
	    install -m 0644 libsysdev.so* ${DESTDIR}${LIBDIR}/
	echo ${LIBS} | grep libsysdev.a  && \
	    install -m 0644 libsysdev.a ${DESTDIR}${LIBDIR}
	install -m 0644 libsysdev/sysdev.h  \
	    ${DESTDIR}${INCDIR}/libsysdev/sysdev.h
	install -m 0644 devinfo ${DESTDIR}${BINDIR}

clean:
	rm -f ${SOOBJS} ${AROBJS} libsysdev.so* libsysdev.a devinfo

%.lo: %.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${XFLAGS} ${PIC} -c -o $@ $<

%.o: %.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${XFLAGS} -c -o $@ $<

libsysdev.so.${SOVER}: ${SOOBJS}
	${CC} ${LDFLAGS} -shared -o $@.${SOMIN} -Wl,-soname,$@ ${SOOBJS}
	ln -sf $@.${SOMIN} $@
	ln -sf $@.${SOMIN} libsysdev.so

libsysdev.a: ${AROBJS}
	${AR} -rcs $@ ${AROBJS}

%: util/%.c ${LIBS}
	${CC} -I. ${CFLAGS} ${CPPFLAGS} ${XFLAGS} \
	    util/$@.c -o $@ ${LDFLAGS} -L. -lsysdev 

.PHONY: all clean install
