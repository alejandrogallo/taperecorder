#include <libconfig.h>
int
get_config_dir(char *dir);

int
get_config_file(char *full_path);

int
get_recordings_dir(char *dir);

int
mkconfig_dir(void);

int
get_config(config_t* cfg);

int
get_config_string(char *keyword, const char **value);

