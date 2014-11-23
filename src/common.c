#include "../include/common.h"

uint32_t strlen(char *str)
{
    uint32_t i = 0;
    while (str[i])
        i++;
    return i;
}
