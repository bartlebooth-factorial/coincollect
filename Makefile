install: coincollect

coincollect:
	cc -lncurses main.c -o coincollect

clean:
	rm coincollect
