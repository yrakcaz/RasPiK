#include "utils.h"

uint32_t strlen(const char *str)
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
    char *str = kmalloc(numlen(val) + 1);
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

char *strcat(const char *str1, const char *str2)
{
    char *ret = kmalloc(strlen(str1) + strlen(str2) + 1);
    int i = 0;
    for (int j = 0; str1[j]; i++, j++)
        ret[i] = str1[j];
    for (int j = 0; str2[j]; i++, j++)
        ret[i] = str2[j];
    ret[i] = '\0';
    return ret;
}

int strcmp(const char *str1, const char *str2)
{
    if (strlen(str1) != strlen(str2))
        return 1;
    for (int i = 0; i < strlen(str1); i++)
        if (str1[i] != str2[i])
            return 1;
    return 0;
}
