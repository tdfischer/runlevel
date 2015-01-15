all: spacekit-runlevel

spacekit-runlevel: main.c
	gcc -Wall -Werror -o $@ `pkg-config --cflags --libs graviton-server-1.0 graviton-client-1.0` $<
