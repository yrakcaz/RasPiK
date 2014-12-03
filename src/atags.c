#include "atags.h"

static void print_afield(const char *name, uint32_t value)
{
    klog(name, strlen(name), WHITE);
    klog(": 0x", 4, WHITE);
    char *addr = itoa(value, 16);
    klog(addr, strlen(addr), WHITE);
    kfree(addr);
    klog("\n", 1, WHITE);
}

static void print_acore(s_acore *core)
{
    klog("CORE :\n", 7, BLUE);
    if (core->header.size == 5)
    {
        print_afield("flags", core->flags);
        print_afield("page_size", core->page_size);
        print_afield("root", core->root);
        klog("\n", 1, WHITE);
    }
    else
        klog("No data\n\n", 9, WHITE);
}

static void print_amem(s_amem *mem)
{
    klog("MEM :\n", 6, BLUE);
    print_afield("size", mem->size);
    print_afield("addr", mem->addr);
    klog("\n", 1, WHITE);
}

static void print_aramdsk(s_aramdsk *ramdsk)
{
    klog("RAMDSK :\n", 9, BLUE);
    print_afield("flags", ramdsk->flags);
    print_afield("size", ramdsk->size);
    print_afield("start", ramdsk->start);
    klog("\n", 1, WHITE);
}

static void print_ainitrd(s_ainitrd *initrd)
{
    klog("INITRD :\n", 9, BLUE);
    print_afield("addr", initrd->addr);
    print_afield("size", initrd->size);
    klog("\n", 1, WHITE);
}

static void print_aserial(s_aserial *serial)
{
    klog("SERIAL :\n", 9, BLUE);
    print_afield("low", serial->low);
    print_afield("high", serial->high);
    klog("\n", 1, WHITE);
}

static void print_arev(s_arev *rev)
{
    klog("REV :\n", 6, BLUE);
    print_afield("rev", rev->rev);
    klog("\n", 1, WHITE);
}

static void print_afb(s_afb *framebuf)
{
    klog("FB :\n", 5, BLUE);
    print_afield("address", framebuf->address);
    print_afield("size", framebuf->size);
    klog("\n", 1, WHITE);
}

static void print_acmd(s_acmd *cmd)
{
    klog("CMD :\n", 6, BLUE);
    klog("cmdline: ", 9, WHITE);
    klog(&(cmd->cmd), strlen(&(cmd->cmd)), WHITE);
    klog("\n\n", 2, WHITE);
}

void print_atags(s_aheader *atags)
{
    klog("ATAGS :\n", 8, RED);
    print_afield("address", (uint32_t)atags);
    klog("\n", 1, WHITE);

    uint32_t tag;

    do
    {
        tag = atags->tag;
        switch (tag)
        {
            case ANONE:
                klog("DONE!\n\n", 7, RED);
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
                klog("UNKNOWN!\n\n", 10, RED);
                break;
        }
        atags = (s_aheader *)((uint32_t)atags + (atags->size * sizeof(uint32_t)));
    } while (tag);
}
