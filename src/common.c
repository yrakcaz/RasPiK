#include "../include/common.h"

uint32_t strlen(char *str)
{
    uint32_t i = 0;
    while (str[i])
        i++;
    return i;
}

static void swap(char *a, char *b)
{
    char c = *a;
    *a = *b;
    *b = c;
}

static void reverse(char *str)
{
    int start = 0;
    int end = strlen(str) - 1;
    while (start < end)
    {
        swap(str + start, str + end);
        str++;
        end--;
    }
}

static uint32_t numlen(int val)
{
    int i = 0;
    while (val)
    {
        val /= 10;
        i++;
    }
    return i;
}

char *itoa(int val, int base)
{
    int i = 0;
    int neg = 0;
    char *str = malloc(numlen(val) + 1);
    if (!val)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (val < 0 && base == 10)
    {
        neg = 1;
        val = -val;
    }
    while (val != 0)
    {
        int rem = val % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        val = val / base;
    }
    if (neg)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
    return str;
}
