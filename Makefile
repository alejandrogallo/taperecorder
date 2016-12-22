

CFLAGS = $(shell pkg-config --cflags libconfig)
LDFLAGS = $(shell pkg-config --libs libconfig)

taperecorder: src/main.c
	gcc -ansi $< -o taperecorder $(CFLAGS) $(LDFLAGS)
