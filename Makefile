OBJS = pilrc.o lex.o util.o bitmap.o

CFLAGS = -O2 -g -DUNIX

all: pilrc

pilrc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o pilrc

clean:
	rm -f *.[oa] pilrc

