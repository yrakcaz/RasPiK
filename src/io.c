#include "io.h"

//static s_fd fd_table[NBMAX_FD];
//static s_dir dir_table[NBMAX_DIR];

static const char *pwd = "/";

void chdir(const char *path)
{
    pwd = path;
}

const char *get_pwd(void)
{
    return pwd;
}


