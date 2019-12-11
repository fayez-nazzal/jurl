
output: jurl.c dirs.c command_window.c globals.h input.h output
	gcc -Wall jurl.c dirs.c input.c command_window.c -lncurses -o output
