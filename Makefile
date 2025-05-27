#
#

all: mac

mac:	hms.dylib
linux:	hms.so

CFLAGS += -I ../sqlite/sqlite-autoconf-3490200
CFLAGS += -g -fPIC

clean:
	rm -f *.dylib *.BAK *.so

# Linux
%.so: %.c
	$(CC) $(CFLAGS) -shared -o $@ $<

# macOS
%.dylib: %.c
	$(CC) $(CFLAGS) -dynamiclib -o $@ $< 

format:
	find . -name '*.[ch]' | xargs -n1 indent -nut -i4
