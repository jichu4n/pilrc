AUTOMAKE_OPTIONS=foreign

if PILRCUI
PILRCUI_PROG=pilrcui
INCLUDES += $(GTK_CFLAGS)
else
PILRCUI_PROG=
endif

bin_PROGRAMS=pilrc $(PILRCUI_PROG)

COMMON_SOURCES=pilrc.c lex.c util.c bitmap.c font.c plex.c

pilrc_SOURCES=$(COMMON_SOURCES) main.c
noinst_HEADERS=bitmap.h macres.h plex.h std.h \
	font.h lex.h pilrc.h resource.h util.h

pilrcui_SOURCES=$(COMMON_SOURCES) xwin.c
pilrcui_LDADD=$(GTK_LIBS)

DOC_FILES=doc/index.htm doc/palmlogo.gif doc/pilrc.gif doc/pilrc.htm \
	doc/readme.dos doc/readme.os2 doc/readme.x

EXAMPLE_FILES=example/font.txt example/mkasm.cmd example/pilrcbin.pbm \
	example/pilrctst.asm example/pilrctst.inc example/runme.bat \
	example/foo.bat example/pilrc.bmp example/pilrcic.bmp \
	example/pilrctst.c example/pilrctst.rcp example/runme.cmd \
	example/mkasm.bat example/pilrcasc.pbm example/pilrcics.bmp \
	example/pilrctst.h example/readme.txt example/string.txt

PPMQUANT_FILES=ppmquant/palette-02.pbm ppmquant/palette-04.pgm \
	ppmquant/palette-16.pgm ppmquant/palette-256.ppm

FONTS_FILES=fonts/pilfont.zip
VSLICK_FILES=vslick/rcp.vlx
ROOT_FILES=makefile.os2 pilrc.dsp pilrc.mak readme.txt resource.rc

EXTRA_DIST=$(DOC_FILES) $(EXAMPLE_FILES) $(FONTS_FILES) $(VSLICK_FILES) \
	$(ROOT_FILES) $(PPMQUANT_FILES)
