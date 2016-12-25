#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

#include "config.h"
#include "definitions.h"
#include "configuration.h"
#include "commands.h"


int
main()
{

  mkconfig_dir();

  enter_shell_loop();

  return 0;

}

/* vim-run: make && ./src/taperecorder  */
/* vim-run: make CFLAGS=-DDEBUG && ./src/taperecorder  */
