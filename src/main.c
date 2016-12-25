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
check_command(struct command_t command, char *command_name)
{
  int i;
  for (i = 0; i < command.na; ++i) {
#ifdef DEBUG
    printf("%s\n", command.aliases[i]);
#endif
    if ( strcmp ( command.aliases[i], command_name ) == 0 ) {
#ifdef DEBUG
      printf("MATCH!!!!!!!!!!\n");
#endif
      return 0;
    }
  }
  return 1;
}



int
main()
{


  /*Configuration variables*/
  const char *clear_command = malloc(sizeof(char*)*MAX_SIZE_CMD);
  const char *play_command = malloc(sizeof(char*)*MAX_SIZE_CMD);
  const char *rec_command = malloc(sizeof(char*)*MAX_SIZE_CMD);
  const char *prompt = malloc(sizeof(char*)*MAX_SIZE_CMD);

  char *command = malloc(sizeof(char*)*MAX_SIZE_CMD);
  char *command_name = malloc(sizeof(char*)*MAX_SIZE_CMD);
  char *command_argument = malloc(sizeof(char*)*MAX_SIZE_CMD);

  /*regex initialization*/
  regex_t command_regex;
  int regex_compile_result, result;
  unsigned int i;

  /*
   * maximum number of matches allowed
   *  (we only need one)
   */
  size_t nmatch = 3;
  regmatch_t *pmatch = malloc(nmatch*sizeof(int));


  mkconfig_dir();



  /*parse configuration variables*/
  get_config_string("clear_command", &clear_command);
  get_config_string("rec_command", &rec_command);
  get_config_string("play_command", &play_command);
  get_config_string("prompt", &prompt);


  /*regex compilation*/
  regex_compile_result = regcomp(&command_regex,
      "^ *([a-zA-Z]+) *( ?+.*)?$", REG_EXTENDED);

  if (regex_compile_result != 0){
    fprintf(stderr, "Error compiling the regular expression\n");
    return 1;
  }
  if ( command == NULL ) {
    printf("Not enough memory for command allocation\n");
    return 1;
  }

  while (1) {
    printf(prompt);

    fgets(command, MAX_SIZE_CMD, stdin);

    /*remove endline character from command to compare*/
    if ((strlen(command)>0) && (command[strlen (command) - 1] == '\n'))
        command[strlen (command) - 1] = '\0';

    if ( strlen(command) == 0 ) continue;

    result = regexec(&command_regex, command, nmatch, pmatch, 0);

    if ( result != 0 ){
      fprintf(stderr, "Error executing the regular expression\n");
      continue;
    }

    for (i = 0; i < nmatch; ++i) {
      if (pmatch[i].rm_so == -1) break;
      if (i == 1) {
        strncpy(command_name, command+pmatch[i].rm_so, pmatch[i].rm_eo);
#ifdef DEBUG
        printf("command_name = %s\n", command_name);
#endif
      }
      else if (i == 2) {
        strncpy(command_argument, command+pmatch[i].rm_so, pmatch[i].rm_eo);
#ifdef DEBUG
        printf("command_argument = %s\n", command_argument);
#endif
      }
/*#ifdef DEBUG*/
      /*printf("%d -> %d\n", pmatch[i].rm_so, pmatch[i].rm_eo);*/
      /*printf("%d\n", i);*/
/*#endif*/
    }

    if ( check_command(QUIT, command_name) == 0 ) {
      return 0;
    } else if ( check_command(LIST, command_name) == 0 ) {
      list_recordings();
    } else if ( check_command(RECORD, command_name) == 0 ) {
      record(rec_command, command_argument);
    } else if ( check_command(PLAY, command_name) == 0 ) {
      play(play_command, atoi(command_argument));
    } else if ( check_command(DELETE, command_name) == 0 ) {
      delete(atoi(command_argument));
    } else if ( check_command(CLEAR, command_name) == 0 ) {
      system(clear_command);
    } else {
      printf(" !! Unkown command: %s\n", command );
    }

    memset(command, 0, MAX_SIZE_CMD*sizeof(char*));
    memset(command_name, 0, MAX_SIZE_CMD*sizeof(char*));
    memset(command_argument, 0, MAX_SIZE_CMD*sizeof(char*));
  }

  return 0;

}

/* vim-run: make && ./src/taperecorder  */
/* vim-run: make CFLAGS=-DDEBUG && ./src/taperecorder  */
