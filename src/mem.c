#include "mem.h"

extern char *get_sp();

static u_header h_list;
static u_header *h_first = NULL;

void *sbrk(uint32_t increment)
{
    extern char _end;
    static char *heap_end;
    char *previous_heap_end;

    if (!heap_end)
        heap_end = &_end;

    previous_heap_end = heap_end;

    if (heap_end + increment > get_sp())
        return (void *)-1;

    heap_end += increment;
    return previous_heap_end;
}

void kfree(void *ptr)
{
    if (!ptr)
        return;
    u_header *it = h_first;
    u_header *blk = (u_header *)ptr - 1;
    while (blk <= it || blk >= it->metadata.next)
    {
        if ((blk > it || blk < it->metadata.next) && it >= it->metadata.next)
            break;
        it = it->metadata.next;
    }
    if (blk + blk->metadata.len == it->metadata.next)
    {
        blk->metadata.len += it->metadata.next->metadata.len;
        blk->metadata.next = it->metadata.next->metadata.next;
    }
    else
        blk->metadata.next = it->metadata.next;
    if (it + it->metadata.len == blk)
    {
        it->metadata.len += blk->metadata.len;
        it->metadata.next = blk->metadata.next;
    }
    else
        it->metadata.next = blk;
    h_first = it;
}

void *kmalloc(uint32_t size)
{
    u_header *p;
    u_header *prev = h_first;
    unsigned real_sz = (size + sizeof(u_header) - 1) / sizeof(u_header) + 1;
    if (!h_first)
    {
        prev = &h_list;
        h_first = prev;
        h_list.metadata.next = h_first;
        h_list.metadata.len = 0;
    }
    p = prev->metadata.next;
    while (1)
    {
        if (p->metadata.len >= real_sz)
        {
            if (p->metadata.len == real_sz)
                prev->metadata.next = p->metadata.next;
            else
            {
                p->metadata.len -= real_sz;
                p += p->metadata.len;
                p->metadata.len = real_sz;
            }
            h_first = prev;
            return (void *)(p + 1);
        }
        if (p == h_first)
        {
            if (real_sz < NBALLOC)
                real_sz = NBALLOC;
            char *page = sbrk((intptr_t)(real_sz * sizeof(u_header)));
            if (page == (char*)-1)
                return NULL;
            u_header *blk = (u_header *)page;
            blk->metadata.len = real_sz * sizeof(u_header);
            kfree((void *)(blk + 1));
            p = h_first;
        }
        prev = p;
        p = p->metadata.next;
    }
    return NULL;
}

void *kcalloc(uint32_t num, uint32_t len)
{
    void *ptr = kmalloc(num * len);
    if (ptr)
        for (uint32_t i = 0; i < num * len; i++)
            ((char *)ptr)[i] = 0;
    return ptr;
}

static void *kmemcpy(void *dst, void *src, uint32_t len)
{
    char *d = dst;
    char *s = src;
    for (int i = 0; i < len; i++)
        d[i] = s[i];
    return dst;
}

void *krealloc(void *ptr, uint32_t size)
{
    if (!ptr)
        return kmalloc(size);
    if (size <= 0)
    {
        kfree(ptr);
        return NULL;
    }
    u_header *bp = (u_header *)ptr - 1;
    uint32_t nbytes = sizeof (u_header) * (bp->metadata.len - 1);
    if (size == nbytes)
        return ptr;
    u_header *p = kmalloc(size);
    if (!p)
        return NULL;
    if (size < nbytes)
        nbytes = size;
    kmemcpy(p, ptr, nbytes);
    kfree(ptr);
    return p;
}
