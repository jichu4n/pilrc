AUTOMAKE_OPTIONS=foreign

if PILRCUI
PILRCUI_PROG=pilrcui
INCLUDES += $(GTK_CFLAGS)
else
PILRCUI_PROG=
endif

bin_PROGRAMS=pilrc $(PILRCUI_PROG)

COMMON_SOURCES=pilrc.c lex.c util.c restype.c bitmap.c font.c plex.c makeKbd.c

pilrc_SOURCES=$(COMMON_SOURCES) main.c
noinst_HEADERS=bitmap.h macres.h plex.h std.h \
	font.h lex.h pilrc.h resource.h util.h makeKbd.h

pilrcui_SOURCES=$(COMMON_SOURCES) xwin.c
pilrcui_LDADD=$(GTK_LIBS)

DOC_FILES=doc/archive.html doc/contrib.html doc/download.gif \
	doc/download.html doc/history.html doc/index.html doc/manual.html

EXAMPLE_FILES=example/images/icon1bpp.bmp example/images/icon2bpp.bmp \
	example/images/icon8bpp.bmp example/images/info1bpp.bmp \
	example/images/palm1bpp.bmp example/images/palm2bpp.bmp \
	example/images/palm8bpp.bmp example/images/smic1bpp.bmp \
	example/images/smic2bpp.bmp example/images/smic8bpp.bmp \
	example/5x11.txt example/Makefile example/README.txt example/device.c \
	example/device.h example/gccfix.c example/gccfix.h example/palm.c \
	example/palm.h example/pilrctst.def example/pilrctst.prc \
	example/pilrctst.rcp example/resource.h

PPMQUANT_FILES=ppmquant/palette-02.pbm ppmquant/palette-04.pgm \
	ppmquant/palette-16.pgm ppmquant/palette-256.ppm

FONTS_FILES=fonts/pilfont.zip
ROOT_FILES=makefile.os2 pilrc.dsp pilrc.mak README.txt LICENSE.txt resource.rc

EXTRA_DIST=$(DOC_FILES) $(EXAMPLE_FILES) $(FONTS_FILES) $(ROOT_FILES) \
	$(PPMQUANT_FILES)
