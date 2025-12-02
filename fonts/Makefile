OBJS = pilfont.o

CC ?= gcc

CFLAGS += -O2 $(DEFINES)

all: pilfont

pilfont: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o pilfont core

distribute:
	tar -cf pilfont_1.0.tar COPYING README Makefile pilfont.c
	gzip -9 pilfont_1.0.tar
