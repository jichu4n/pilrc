OBJS 	= pilrc.o lex.o util.o bitmap.o main.o font.o plex.o
XOBJS	= pilrc.o lex.o util.o bitmap.o xwin.o font.o plex.o

CC 	= gcc
CFLAGS 	= -Wall -O2 -g -DUNIX
XINC	= -I/local/include
XLIBS 	= -L/local/lib -lgtk -lgdk -lglib -lX11 -lm

all: pilrc pilrcui

pilrc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o pilrc

pilrcui: $(OBJS) $(XOBJS)
	$(CC) `gtk-config --libs` $(CFLAGS) $(XOBJS) -o pilrcui $(XLIBS)

xwin.o: xwin.c
	$(CC) `gtk-config --cflags` $(CFLAGS) $(XINC) -c xwin.c -o xwin.o

clean:
	rm -f *.[oa] stamp.pilrc

distclean:
	rm -f *.[oa] pilrc pilrcui stamp.pilrc pilrc.exe pilrcui.exe
