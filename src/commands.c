#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "definitions.h"
#include "configuration.h"

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
