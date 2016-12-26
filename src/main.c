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
main(int argc, char **argv)
{

  /*char command[MAX_SIZE_CMD];*/
  const char __OPTIONS[] = "hvVc:";
  int choice;
  int option_index = 0;
  static struct option long_options[] =
  {
    /* Argument styles: no_argument, required_argument, optional_argument */
    {"verbose", no_argument, 0, 'V'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"command", required_argument, 0, 'c'},
    {0,0,0,0}
  };


  while (1)
  {
    choice = getopt_long( argc, argv, __OPTIONS,
        long_options, &option_index);
    if (choice == -1)
      break;
    switch( choice )
    {
      case 'v':
        printf("%s -- Version %s", PACKAGE, VERSION);
        exit(0);
        break;
      case 'h':
        printf("Usage: %s "
            "[-h|--help] "
            "[-v|--version] "
            "[-V|--verbose] "
            "[-c|--command <command>] "
            "\n",PACKAGE);
        printf("\n  %s\n",PACKAGE_DESCRIPTION);
        printf("\n");
        printf("  Options:\n"
               "    -h|--help       Display this message and exit.\n"
               "    -v|--version    Display version and exit.\n"
               "    -V|--verbose    Be verbose in the output.\n"
               "    -c|--command    Execute a command.\n"
               "\n"
               "This program is maintained by Alejandro Gallo\n");
        exit(0);
        break;
      case 'c':
        run_command(optarg);
        break;
      case '?':
        /* getopt_long will have already printed an error */
        break;
      default:
        /* Not sure how to get here... */
        return EXIT_FAILURE;
    }
  }

  mkconfig_dir();

  enter_shell_loop();

  return 0;

}

/* vim-run: make && ./src/taperecorder --help  */
/* vim-run: make && ./src/taperecorder --version  */
/* vim-run: make && ./src/taperecorder  */
/* vim-run: make CFLAGS=-DDEBUG && ./src/taperecorder  */
