#ifndef ELF_H
# define ELF_H

# include <stdint.h>
# include "utils.h"

uint32_t load_elf(const char *path);
int exec(const char *path, char **args);

#endif /* !ELF_H */
