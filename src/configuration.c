#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

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

