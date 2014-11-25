#include "../include/atags.h"

static void print_afield(const char *name, uint32_t value)
{
    write_console(name, strlen((char *)name), WHITE);
    write_console(": 0x", 4, WHITE);
    write_console(itoa(value, 16), strlen(itoa(value, 16)), WHITE);
    write_console("\n", 1, WHITE);
}

static void print_acore(s_acore *core)
{
    write_console("CORE :\n", 7, BLUE);
    if (core->header.size == 5)
    {
        print_afield("flags", core->flags);
        print_afield("page_size", core->page_size);
        print_afield("root", core->root);
        write_console("\n", 1, WHITE);
    }
    else
        write_console("No data\n\n", 9, WHITE);
}

static void print_amem(s_amem *mem)
{
    write_console("MEM :\n", 6, BLUE);
    print_afield("size", mem->size);
    print_afield("addr", mem->addr);
    write_console("\n", 1, WHITE);
}

static void print_aramdsk(s_aramdsk *ramdsk)
{
    write_console("RAMDSK :\n", 9, BLUE);
    print_afield("flags", ramdsk->flags);
    print_afield("size", ramdsk->size);
    print_afield("start", ramdsk->start);
    write_console("\n", 1, WHITE);
}

static void print_ainitrd(s_ainitrd *initrd)
{
    write_console("INITRD :\n", 9, BLUE);
    print_afield("addr", initrd->addr);
    print_afield("size", initrd->size);
    write_console("\n", 1, WHITE);
}

static void print_aserial(s_aserial *serial)
{
    write_console("SERIAL :\n", 9, BLUE);
    print_afield("low", serial->low);
    print_afield("high", serial->high);
    write_console("\n", 1, WHITE);
}

static void print_arev(s_arev *rev)
{
    write_console("REV :\n", 6, BLUE);
    print_afield("rev", rev->rev);
    write_console("\n", 1, WHITE);
}

static void print_afb(s_afb *framebuf)
{
    write_console("FB :\n", 5, BLUE);
    print_afield("address", framebuf->address);
    print_afield("size", framebuf->size);
    write_console("\n", 1, WHITE);
}

static void print_acmd(s_acmd *cmd)
{
    write_console("CMD :\n", 6, BLUE);
    write_console("cmdline: ", 9, WHITE);
    write_console(&(cmd->cmd), strlen(&(cmd->cmd)), WHITE);
    write_console("\n\n", 2, WHITE);
}

void print_atags(s_aheader *atags)
{
    write_console("ATAGS :\n", 8, RED);
    print_afield("address", (uint32_t)atags);
    write_console("\n", 1, WHITE);

    uint32_t tag;

    do
    {
        tag = atags->tag;
        switch (tag)
        {
            case ANONE:
                write_console("DONE!\n\n", 7, RED);
                break;
            case ACORE:
                print_acore((s_acore *)atags);
                break;
            case AMEM:
                print_amem((s_amem *)atags);
                break;
            case ARAMDSK:
                print_aramdsk((s_aramdsk *)atags);
                break;
            case AINITRD:
                print_ainitrd((s_ainitrd *)atags);
                break;
            case ASERIAL:
                print_aserial((s_aserial *)atags);
                break;
            case AREV:
                print_arev((s_arev *)atags);
                break;
            case AFB:
                print_afb((s_afb *)atags);
                break;
            case ACMD:
                print_acmd((s_acmd *)atags);
                break;
            default:
                write_console("UNKNOWN!\n\n", 10, RED);
                break;
        }
        atags = (s_aheader *)((uint32_t)atags + (atags->size * sizeof(uint32_t)));
    } while (tag);
}
