
DEBUG ?=

CFLAGS = $(shell pkg-config --cflags libconfig)
LDFLAGS = $(shell pkg-config --libs libconfig)

bin_DIR = bin

CFLAGS += $(and $(DEBUG),-DDEBUG)

$(bin_DIR)/taperecorder: src/main.c
	mkdir -p $(bin_DIR)
	gcc -Wall -pedantic -ansi $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf bin

