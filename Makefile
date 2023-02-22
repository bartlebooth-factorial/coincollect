coincollect:
	cc -lncurses coincollect.c -o coincollect

install:
	cp coincollect /usr/local/bin

uninstall:
	rm /usr/local/bin/coincollect

clean:
	rm coincollect

.PHONY: coincollect
