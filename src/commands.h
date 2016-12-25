#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct command_t {
  const char name[265];
  const char *aliases[5];
  const int na;
} command_t;

int
list_recordings();

int
delete(int recording_number);

int
play(const char *command, int recording_number);

int
record(const char *command, char *filename) ;

int
check_command(struct command_t command, char *command_name);

int
run_command (char *command);

int
enter_shell_loop(void);

#endif /* ifndef COMMANDS_H */

