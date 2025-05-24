#
#

all: hms

clean:
	rm -f hms *.BAK

hms: hms.c
	$(CC) $(CFLAGS) -o $@ $<

format:
	find . -name '*.[ch]' | xargs -n1 indent -nut -i4
