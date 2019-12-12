
output: jute.c dirs.c command_window.c date_time.c colors.c main_window.h input.c output
	gcc -Wall jute.c dirs.c input.c date_time.c colors.c command_window.c main_window.c -lncurses -o output
