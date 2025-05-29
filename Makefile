#
#

all: mac

mac:	hms.dylib
linux:	hms.so

SQL = ../sqlite/sqlite-autoconf-3490200/sqlite3
SQL = sqlite3

CFLAGS += -I ../sqlite/sqlite-autoconf-3490200
CFLAGS += -g -fPIC

# K&R style indentation
INDENT += -nbad -bap -nbc -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0
INDENT += -d0 -di1 -nfc1 -i4 -ip0 -l75 -lp -npcs
INDENT += -npsl -nsc -nsob

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

test: test-1.sql test-1.csv
	$(SQL) < test-1.sql
