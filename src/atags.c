#include "atags.h"

static void print_afield(const char *name, uint32_t value)
{
    klog("\t");
    klog(name);
    klog(": 0x");
    char *addr = itoa(value, 16);
    klog(addr);
    kfree(addr);
    klog("\n");
}

static void print_acore(s_acore *core)
{
    klog("CORE\n");
    if (core->header.size == 5)
    {
        print_afield("flags", core->flags);
        print_afield("page_size", core->page_size);
        print_afield("root", core->root);
    }
    else
        klog("\tNo data\n");
}

static void print_amem(s_amem *mem)
{
    klog("MEM\n");
    print_afield("size", mem->size);
    print_afield("addr", mem->addr);
}

static void print_aramdsk(s_aramdsk *ramdsk)
{
    klog("RAMDSK\n");
    print_afield("flags", ramdsk->flags);
    print_afield("size", ramdsk->size);
    print_afield("start", ramdsk->start);
}

static void print_ainitrd(s_ainitrd *initrd)
{
    klog("INITRD\n");
    print_afield("addr", initrd->addr);
    print_afield("size", initrd->size);
}

static void print_aserial(s_aserial *serial)
{
    klog("SERIAL\n");
    print_afield("low", serial->low);
    print_afield("high", serial->high);
}

static void print_arev(s_arev *rev)
{
    klog("REV\n");
    print_afield("rev", rev->rev);
}

static void print_afb(s_afb *framebuf)
{
    klog("FB\n");
    print_afield("address", framebuf->address);
    print_afield("size", framebuf->size);
}

static void print_acmd(s_acmd *cmd)
{
    klog("CMD\n");
    klog("\tcmdline: ");
    klog(&(cmd->cmd));
    klog("\n");
}

void print_atags(s_aheader *atags)
{
    klog("ATAGS\n");
    print_afield("address", (uint32_t)atags);

    if (!atags)
        return;

    uint32_t tag;
    do
    {
        tag = atags->tag;
        switch (tag)
        {
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
          case ACMND:
           print_acmd((s_acmd *)atags);
           break;
          case ANONE:
          default:
           break;
        }
        atags = (s_aheader *)((uint32_t)atags + (atags->size * sizeof(uint32_t)));
    } while (tag);
}
