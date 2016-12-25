AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -pedantic -Wall -Wextra -Werror

bin_PROGRAMS = taperecorder
taperecorder_SOURCES = main.c \
commands.c \
configuration.c \
definitions.h

taperecorder_LDFLAGS = -lconfig -lreadline

