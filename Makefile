
output: jurl.c dirs.c command_window.c globals.c input.h output
	gcc -Wall jurl.c dirs.c input.c globals.c command_window.c -lncurses -o output
