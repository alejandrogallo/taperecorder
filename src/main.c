#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libconfig.h>

#define MAX_SIZE_CMD 256
#define TAPERECORDER_CONFIG_FOLDER_NAME "/.taperecorder"
#define TAPERECORDER_RECORDINGS_FOLDER TAPERECORDER_CONFIG_FOLDER_NAME "/recordings"
#define TAPERECORDER_CONFIG_FILE TAPERECORDER_CONFIG_FOLDER_NAME "/config"

int
record(const char *command, char *filename)
{
  char *full_command = malloc(MAX_SIZE_CMD);
  /*strcpy(full_command, command);*/
  /*strcat(full_command, filename);*/
  /*printf("%s\n", full_command);*/
  sprintf(full_command, command, filename);
  printf("%s\n", full_command);
  system(full_command);
  return 0;
}


int
mkconfig_dir()
{
  int result;
  char main_config_dir[256];

  strcpy(main_config_dir , getenv("HOME"));
  strcat(main_config_dir , TAPERECORDER_CONFIG_FOLDER_NAME);

  result = mkdir(main_config_dir, 0777);
  if (result != 0) {
    fprintf(stderr, "Fail in creating configuration directory in %s!\n", main_config_dir);
    return 1;
  }

  return 0;

}

int
list_recordings(){
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("/home/ale/.taperecorder/recordings/")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      printf ("%s\n", ent->d_name);
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
  }
  return 0;
}

int
main(int argc, char *argv[])
{

  config_t cfg, *cfg_pointer;
  cfg_pointer = &cfg;

  /*Configuration variables*/
  const char *rec_command = "rec %s";
  const char *play_command = "play %s";
  const char *prompt = ">> ";

  char *command = malloc(MAX_SIZE_CMD);
  char *command_name = malloc(MAX_SIZE_CMD);
  char *command_argument = malloc(MAX_SIZE_CMD);

  /*regex initialization*/
  regex_t rec_regex;
  regex_t list_regex;
  regex_t quit_regex;
  regex_t delete_regex;
  regex_t play_regex;
  regex_t clear_regex;
  int regex_compile_result, result;
  int i;

  /*
   * maximum number of matches allowed
   *  (we only need one)
   */
  size_t nmatch = 3;
  regmatch_t pmatch[nmatch];

  mkconfig_dir();

  /*configuration init*/
  config_init(cfg_pointer);
  if ( ! config_read_file( cfg_pointer, "/home/ale/.taperecorder/config" ) ) {
    fprintf(stderr, "Error reading configuration file\n");
  }

  /*parse configuration variables*/
  config_lookup_string(cfg_pointer, "rec_command", &rec_command);
  config_lookup_string(cfg_pointer, "play_command", &play_command);
  config_lookup_string(cfg_pointer, "prompt", &prompt);

  /*regex compilation*/
  regex_compile_result = regcomp(&list_regex,
      "^ *(l|ls|list) *( +.*)?$", REG_EXTENDED);
  regex_compile_result = regcomp(&rec_regex,
      "^ *(r|rec|record) *( +.*)?$", REG_EXTENDED);
  regex_compile_result = regcomp(&delete_regex,
      "^ *(d|del|delete|rm|remove) *( +.*)?$", REG_EXTENDED);
  regex_compile_result = regcomp(&play_regex,
      "^ *(p|play)( +.*)?$", REG_EXTENDED);
  regex_compile_result = regcomp(&quit_regex,
      "^ *(q|quit|exit) *$", REG_EXTENDED);
  regex_compile_result = regcomp(&clear_regex,
      "^ *(clear) *$", REG_EXTENDED);

  if (command == NULL) {
    printf("Not enough memory for command allocation\n");
    return 1;
  }

  while (1) {
    printf(prompt);
    fgets(command, MAX_SIZE_CMD, stdin);

    /*remove endline character from command to compare*/
    if ((strlen(command)>0) && (command[strlen (command) - 1] == '\n'))
        command[strlen (command) - 1] = '\0';

    if ( regexec(&quit_regex, command, 0, NULL, 0) == 0 ) {
      printf("Exiting...\n");
      return 0;
    } else if ( regexec(&list_regex, command, 0, NULL, 0) == 0 ) {
      printf("Listing...\n");
      list_recordings();
    } else if ( regexec(&rec_regex, command, nmatch, pmatch, 0) == 0 ) {
      printf("Recording...\n");
      for (i = 0; i < nmatch; ++i) {
        printf("nmatch = %d\n", nmatch);
        if (pmatch[i].rm_so == -1) break;
        if (i == 1) {
          strncpy(command_name, command+pmatch[i].rm_so, pmatch[i].rm_eo);
          printf("%s\n", command_name);
        } else if (i == 2) {
          strncpy(command_argument, command+pmatch[i].rm_so, pmatch[i].rm_eo);
          printf("%s\n", command_argument);
        }
        printf("%d -> %d\n", pmatch[i].rm_so, pmatch[i].rm_eo);
        printf("%d\n", i);
      }
      record(rec_command, command_argument);
    } else if ( regexec(&play_regex, command, 0, NULL, 0) == 0 ) {
      printf("Playing...\n");
    } else if ( regexec(&delete_regex, command, 0, NULL, 0) == 0 ) {
      printf("Deleting...\n");
    } else if ( regexec(&clear_regex, command, 0, NULL, 0) == 0 ) {
      system("clear");
    } else {
      printf(" !! Unkown command: %s\n", command );
    }

    command[0] = '\0';
    command_name[0] = '\0';
    command_argument[0] = '\0';
  }

  return 0;

}

/* vim-run: make && ./taperecorder  */
