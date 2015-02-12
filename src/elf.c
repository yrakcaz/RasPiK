#include "elf.h"

static int check_elf(Elf32_Ehdr *hdr)
{
    if (!hdr)
        return 0;
    if (hdr->e_ident[EI_MAG0] != ELFMAG0)
        return 0;
    if (hdr->e_ident[EI_MAG1] != ELFMAG1)
        return 0;
    if (hdr->e_ident[EI_MAG2] != ELFMAG2)
        return 0;
    if (hdr->e_ident[EI_MAG3] != ELFMAG3)
        return 0;
    if (hdr->e_ident[EI_CLASS] != ELFCLASS32)
        return 0;
    if (hdr->e_ident[EI_DATA] != ELFDATA2LSB)
        return 0;
    if (hdr->e_machine != EM_ARM)
        return 0;
    if (hdr->e_ident[EI_VERSION] != EV_CURRENT)
        return 0;
    if (hdr->e_type != ET_EXEC)
        return 0;
    return 1;
}

static void *memcpy(void *dest, const void *src, uint32_t n)
{
  const char* s = src;
  char* d = dest;

  for (; n > 0; n--, d++, s++)
    *d = *s;

  return (dest);
}

static void *memset(void *s, int c, uint32_t n)
{
    for (int i = 0; i < n; i++)
        ((char *)s)[i] = c;
    return s;
}

static void load_segment(uint8_t *data, Elf32_Phdr *phead)
{
    uint32_t memsz = phead->p_memsz;
    if (!memsz)
        return;
    uint32_t filesz = phead->p_filesz;
    uint32_t mempos = phead->p_vaddr;
    uint32_t filepos = phead->p_offset;
    uint32_t flags = FLAG_READ;
    if (phead->p_flags & PF_W)
        flags |= FLAG_WRITE;

    memcpy((void *)mempos, &data[filepos], filesz);
    memset((void *)(mempos + filesz), 0, memsz - filesz);
}

uint32_t load_elf(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return -1;

    struct stat sb;
    if (stat(fd, &sb) <= 0)
    {
        close(fd);
        return -1;
    }
    uint8_t buf[sb.st_size];
    read(fd, buf, sb.st_size);
    Elf32_Ehdr *hdr = (Elf32_Ehdr *)buf;
    if (!check_elf(hdr))
    {
        close(fd);
        return -1;
    }

    Elf32_Phdr *ptab = (Elf32_Phdr *)((uint8_t *)hdr + hdr->e_phoff);
    for (int i = 0; i < hdr->e_phnum; i++)
        if (ptab[i].p_type == PT_LOAD)
            load_segment(buf, &ptab[i]);

    close(fd);

    return hdr->e_entry;
}

int exec(const char *path, char **args)
{
    uint32_t addr = load_elf(path);
    if (addr == -1)
        return -1;

    int i;
    for (i = 0; args[i]; i++);
    asm volatile("mov r0, %0" :: "r"((uint32_t)addr));
    asm volatile("push {r0}");
    asm volatile("mov r0, %0" :: "r"((uint32_t)i));
    asm volatile("mov r1, %0" :: "r"((uint32_t)args));
    asm volatile("pop {pc}");

    return -1;
}
