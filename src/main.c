#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libconfig.h>

#include "commands.h"

#define MAX_SIZE_CMD 256
#define MAX_SIZE_RECORDING_PATH_NAME 256
#define TAPERECORDER_CONFIG_FOLDER_NAME "/.taperecorder"
#define TAPERECORDER_RECORDINGS_FOLDER TAPERECORDER_CONFIG_FOLDER_NAME "/recordings"
#define TAPERECORDER_CONFIG_FILE TAPERECORDER_CONFIG_FOLDER_NAME "/config"

int
get_config_dir(char *dir){
  strcpy(dir , getenv("HOME"));
  strcat(dir , TAPERECORDER_CONFIG_FOLDER_NAME);
  return 0;
}

int
get_config_file(char *full_path){
  strcpy(full_path , getenv("HOME"));
  strcat(full_path , TAPERECORDER_CONFIG_FOLDER_NAME);
  strcat(full_path , "/");
  strcat(full_path , "config");
  return 0;
}

int
get_recordings_dir(char *dir){
  strcpy(dir , getenv("HOME"));
  strcat(dir , TAPERECORDER_RECORDINGS_FOLDER);
  return 0;
}

int
get_recordings(char ***file_paths, int *number_of_files, unsigned int full_path)
{
  DIR *dir;
  struct dirent *ent;
  char *recordings_dir = malloc(MAX_SIZE_RECORDING_PATH_NAME);
  int i = 0, n;

  get_recordings_dir(recordings_dir);

  /*Get number of files*/
  if ((dir = opendir (recordings_dir)) != NULL) {
    /* print all the files and directories within directory */
    i=0;
    while ((ent = readdir (dir)) != NULL){
      if ( ( strcmp(ent->d_name, ".") != 0 ) && ( strcmp(ent->d_name, "..") != 0 ) )
        i = i+1;
    }
    closedir (dir);
  }
  *number_of_files = i;

  n = *number_of_files;

  /*memory allocation for file_paths*/
  *file_paths = malloc(sizeof(char*)*n);
  for (i = 0; i < n; ++i) {
    (*file_paths)[i] = malloc(sizeof(char*)*MAX_SIZE_RECORDING_PATH_NAME);
    if ( (*file_paths)[i] == NULL )  fprintf(stderr, "Too many recordings for memory allocation\n");
  }
  if ( *file_paths == NULL ) {
    fprintf(stderr, "Too many recordings for memory allocation\n");
    return 1;
  }

  if ((dir = opendir (recordings_dir)) != NULL) {
    /* print all the files and directories within directory */
    i=0;
    while ((ent = readdir (dir)) != NULL) {
      if ( ( strcmp(ent->d_name, ".") != 0 ) && ( strcmp(ent->d_name, "..") != 0 ) ) {
        i = i+1;
        if ( full_path == 1 ){
          strcpy((*file_paths)[i-1], recordings_dir);
          strcat((*file_paths)[i-1], "/");
          strcat((*file_paths)[i-1], ent->d_name);
        } else {
          strcpy((*file_paths)[i-1], ent->d_name);
        }
      }
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
list_recordings(){
  char **files;
  int number_of_files;
  int i;

  get_recordings(&files, &number_of_files, 0);
  for (i = 0; i < number_of_files; ++i) {
    printf("%d) %s\n", i+1, files[i]);
  }

  return 0;

}

int
delete(int recording_number){
  char **files;
  int number_of_files,i;

  get_recordings(&files, &number_of_files, 1);

  if (recording_number == 0)
    recording_number = number_of_files;

  for (i = 1; i <= number_of_files; ++i) {
    if (i == recording_number) {
      printf("Deleting %s\n", files[i-1]);
      if (remove(files[i-1]) == 0){
        printf("Done!\n");
        return 0;
      }
      else {
        fprintf(stderr, "Error deleting file\n");
        return 0;
      }
    }
  }
  printf(" !! Number %d not found!\n", recording_number);
  return 1;
}

int
play(const char *command, int recording_number){
  char *full_command = malloc(MAX_SIZE_CMD);
  char **files;
  int number_of_files,i;

  get_recordings(&files, &number_of_files, 1);

  if (recording_number == 0)
    recording_number = number_of_files;

  for (i = 1; i <= number_of_files; ++i) {
    if (i == recording_number) {
      sprintf(full_command, command, files[i-1]);
      printf("%s\n", full_command);
      system(full_command);
      return 0;
    }
  }
  printf(" !! Number %d not found!\n", recording_number);
  return 1;
}

int
record(const char *command, char *filename)
{
  char *full_command = malloc(MAX_SIZE_CMD);
  char *recordings_dir = malloc(MAX_SIZE_CMD);
  char *full_filename = malloc(MAX_SIZE_CMD);
  if (strlen(filename) == 0) {
    int number_of_files;
    char **files;
    get_recordings(&files, &number_of_files, 0);
    strcpy(filename, "My Recording ");
    sprintf(filename, "%s_%d.wav", "My_Recording", number_of_files);
  }
  get_recordings_dir(recordings_dir);
  strcpy(full_filename, recordings_dir);
  strcat(full_filename, "/");
  strcat(full_filename, filename);
  printf("%s\n", full_filename);
  sprintf(full_command, command, full_filename);
#ifdef DEBUG
  printf("%s\n", full_command);
#endif
  system(full_command);
  return 0;
}


int
mkconfig_dir()
{
  int result;
  char main_config_dir[MAX_SIZE_RECORDING_PATH_NAME];
  char recordings_dir[MAX_SIZE_RECORDING_PATH_NAME];
  DIR *dir;

  get_config_dir(main_config_dir);
  get_recordings_dir(recordings_dir);

  if ((dir = opendir (main_config_dir)) == NULL) {
    result = mkdir(main_config_dir, 0777);
    if (result != 0) {
      fprintf(stderr, "Fail in creating configuration directory in %s!\n", main_config_dir);
      return 1;
    }
  }
  closedir(dir);
  if ((dir = opendir (recordings_dir)) == NULL) {
    result = mkdir(recordings_dir, 0777);
    if (result != 0) {
      fprintf(stderr, "Fail in creating configuration directory in %s!\n", recordings_dir);
      return 1;
    }
  }
  closedir(dir);

  return 0;

}


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

  config_t cfg;
  char *config_file_path = malloc(MAX_SIZE_RECORDING_PATH_NAME);

  /*Configuration variables*/
  const char *rec_command = "rec %s";
  const char *play_command = "play %s";
  const char *prompt = ">> ";

  char *command = malloc(MAX_SIZE_CMD);
  char *command_name = malloc(MAX_SIZE_CMD);
  char *command_argument = malloc(MAX_SIZE_CMD);

  /*regex initialization*/
  regex_t command_regex;
  int regex_compile_result, result;
  int i;

  /*
   * maximum number of matches allowed
   *  (we only need one)
   */
  size_t nmatch = 3;
  regmatch_t *pmatch = malloc(nmatch*sizeof(int));

  mkconfig_dir();

  /*configuration init*/
  config_init(&cfg);
  get_config_file(config_file_path);
#ifdef DEBUG
  printf("Reading config file %s\n", config_file_path);
#endif
  if ( ! config_read_file( &cfg, config_file_path) ) {
    fprintf(stderr, "Error reading configuration file\n");
  }

  /*parse configuration variables*/
  config_lookup_string(&cfg, "rec_command", &rec_command);
  config_lookup_string(&cfg, "play_command", &play_command);
  config_lookup_string(&cfg, "prompt", &prompt);

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
#ifdef DEBUG
      printf("%d -> %d\n", pmatch[i].rm_so, pmatch[i].rm_eo);
      printf("%d\n", i);
#endif
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
      system("clear");
    } else {
      printf(" !! Unkown command: %s\n", command );
    }

    memset(command, 0, sizeof(command));
    memset(command_name, 0, sizeof(command_name));
    memset(command_argument, 0, sizeof(command_argument));
  }

  return 0;

}

/* vim-run: make && ./bin/taperecorder  */
