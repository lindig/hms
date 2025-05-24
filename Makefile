#
#

all: hms.dylib

CFLAGS += -I ../sqlite/sqlite-autoconf-3490200
CFLAGS += -g -fPIC

clean:
	rm -f *.dylib *.BAK

%.dylib: %.c
	$(CC) $(CFLAGS) -dynamiclib -o $@ $< 


format:
	find . -name '*.[ch]' | xargs -n1 indent -nut -i4
