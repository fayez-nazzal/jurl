
output: jute.c dirs.c command_window.c date_time.c colors.c main_window.c  output
	gcc -Wall jute.c dirs.c date_time.c colors.c command_window.c main_window.c -lncurses -o output -lpthread

