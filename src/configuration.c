#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libconfig.h>

#include "config.h"
#include "definitions.h"


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
mkconfig_dir(void)
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
get_config(config_t* cfg)
{
  char *config_file_path = malloc(MAX_SIZE_RECORDING_PATH_NAME);

  /*configuration init*/
  config_init(cfg);
  get_config_file(config_file_path);
#ifdef DEBUG
  printf("Reading config file %s\n", config_file_path);
#endif
  if ( ! config_read_file( cfg, config_file_path) ) {
    fprintf(stderr, "Error reading configuration file\n");
    config_destroy(cfg);
    return 1;
  }
  return 0;
}

int
get_config_string(char *keyword, const char **value){
  config_t cfg;
  get_config(&cfg);

#ifdef DEBUG
  printf("Getting %s (%s) from configuration\n", keyword, *value);
#endif

  config_lookup_string(&cfg, keyword, value);

#ifdef DEBUG
  printf("    Result -> '%s'\n", *value);
#endif

  /*config_destroy(&cfg);*/
  return 0;
}


/* vim-run: make && ./src/taperecorder  */
/* vim-run: make CFLAGS=-DDEBUG && ./src/taperecorder  */

