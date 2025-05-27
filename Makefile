#
#

all: mac

mac:	hms.dylib
linux:	hms.so

CFLAGS += -I ../sqlite/sqlite-autoconf-3490200
CFLAGS += -g -fPIC

# K&R style indentation
INDENT += -nbad -bap -bbo -nbc -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0
INDENT += -cp33 -cs -d0 -di1 -nfc1 -nfca -hnl -i4 -ip0 -l75 -lp -npcs
INDENT += -nprs -npsl -saf -sai -saw -nsc -nsob -nss

clean:
	rm -f *.dylib *.BAK *.so
	rm -f *.[ch]~ *.dylib-*

# Linux
%.so: %.c
	$(CC) $(CFLAGS) -shared -o $@ $<

# macOS
%.dylib: %.c
	$(CC) $(CFLAGS) -dynamiclib -o $@ $< 

format:
	find . -name '*.[ch]' | xargs -n1 indent -nut $(INDENT)
