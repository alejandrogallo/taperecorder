
typedef struct command_t {
  const char name[265];
  const char *aliases[5];
  const int na;
} command_t;




struct command_t RECORD = {"record", {"record", "r", "rec"}, 3};
struct command_t PLAY = {"play", {"play", "p"}, 2};
struct command_t LIST = {"list", {"list", "ls", "l"}, 3};
struct command_t DELETE = {"delete", {"delete", "d", "rm", "remove", "r"}, 5};
struct command_t QUIT = {"quit", {"quit", "q", "exit" }, 3};
struct command_t CLEAR = {"clear", {"clear", "cls"}, 2};


int
list_recordings();

int
delete(int recording_number);

int
play(const char *command, int recording_number);

int
record(const char *command, char *filename) ;

/* vim-run: rm bin/taperecorder ; make && ./bin/taperecorder  */
