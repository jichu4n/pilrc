OBJS = pilrc.o lex.o util.o bitmap.o main.o font.o plex.o
XOBJS = pilrc.o lex.o util.o bitmap.o xwin.o font.o plex.o

CC = gcc
CFLAGS = -O2 -g -DUNIX -Wall -Wno-switch
XINCLUDE = -I/local/include
XLIBS = -L/local/lib -lgtk -lgdk -lglib -lX11 -lm

all: pilrc pilrcui

pilrc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o pilrc

pilrcui: $(OBJS) $(XOBJS)
	$(CC) `gtk-config --libs` $(CFLAGS) $(XOBJS) -o pilrcui $(XLIBS)

xwin.o: xwin.c
	$(CC) `gtk-config --cflags` $(CFLAGS) $(XINCLUDE) -c xwin.c -o xwin.o

clean:
	rm -f *.[oa] pilrc pilrcui stamp.pilrc

