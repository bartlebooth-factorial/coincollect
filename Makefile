install: coincollect

coincollect:
	gcc -lncurses main.c -o coincollect

clean:
	rm coincollect
