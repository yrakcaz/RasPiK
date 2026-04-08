#include "mem.h"

extern char *get_sp();

static u_header heap_list;
static u_header *heap_start = NULL;

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
    u_header *cur = heap_start;
    u_header *block = (u_header *)ptr - 1;
    while (block <= cur || block >= cur->metadata.next)
    {
        if ((block > cur || block < cur->metadata.next) && cur >= cur->metadata.next)
            break;
        cur = cur->metadata.next;
    }
    if (block + block->metadata.len == cur->metadata.next)
    {
        block->metadata.len += cur->metadata.next->metadata.len;
        block->metadata.next = cur->metadata.next->metadata.next;
    }
    else
        block->metadata.next = cur->metadata.next;
    if (cur + cur->metadata.len == block)
    {
        cur->metadata.len += block->metadata.len;
        cur->metadata.next = block->metadata.next;
    }
    else
        cur->metadata.next = block;
}

void *kmalloc(uint32_t size)
{
    u_header *cur;
    u_header *prev_block = heap_start;
    unsigned real_sz = (size + sizeof(u_header) - 1) / sizeof(u_header) + 1;
    if (!heap_start)
    {
        prev_block = &heap_list;
        heap_start = prev_block;
        heap_list.metadata.next = heap_start;
        heap_list.metadata.len = 0;
    }
    cur = prev_block->metadata.next;
    while (1)
    {
        if (cur->metadata.len >= real_sz)
        {
            if (cur->metadata.len == real_sz)
                prev_block->metadata.next = cur->metadata.next;
            else
            {
                cur->metadata.len -= real_sz;
                cur += cur->metadata.len;
                cur->metadata.len = real_sz;
            }
            heap_start = prev_block;
            return (void *)(cur + 1);
        }
        if (cur == heap_start)
        {
            if (real_sz < NBALLOC)
                real_sz = NBALLOC;
            char *page = sbrk((uint32_t)(real_sz * sizeof(u_header)));
            if (page == (char*)-1)
                return NULL;
            u_header *new_block = (u_header *)page;
            new_block->metadata.len = real_sz * sizeof(u_header);
            kfree((void *)(new_block + 1));
            cur = heap_start;
        }
        prev_block = cur;
        cur = cur->metadata.next;
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
    u_header *block_hdr = (u_header *)ptr - 1;
    uint32_t nbytes = sizeof (u_header) * (block_hdr->metadata.len - 1);
    if (size == nbytes)
        return ptr;
    u_header *new_ptr = kmalloc(size);
    if (!new_ptr)
        return NULL;
    if (size < nbytes)
        nbytes = size;
    kmemcpy(new_ptr, ptr, nbytes);
    kfree(ptr);
    return new_ptr;
}
