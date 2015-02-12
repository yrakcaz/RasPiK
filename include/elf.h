#ifndef ELF_H
# define ELF_H

# include <stdint.h>
# include "utils.h"
# include "fs/vfs.h"

# define FLAG_READ (1 << 0)
# define FLAG_WRITE (1 << 1)

typedef    uint8_t      Elf_Byte;

typedef    uint32_t    Elf32_Addr;
# define    ELF32_FSZ_ADDR    4
typedef    uint32_t    Elf32_Off;
# define    ELF32_FSZ_OFF    4
typedef    int32_t    Elf32_Sword;
# define    ELF32_FSZ_SWORD    4
typedef    uint32_t    Elf32_Word;
# define    ELF32_FSZ_WORD    4
typedef    uint16_t    Elf32_Half;
# define    ELF32_FSZ_HALF    2

# define    ELF_NIDENT    16

typedef struct
{
    uint8_t       e_ident[ELF_NIDENT];    /* Id bytes */
    Elf32_Half    e_type;            /* file type */
    Elf32_Half    e_machine;        /* machine type */
    Elf32_Word    e_version;        /* version number */
    Elf32_Addr    e_entry;        /* entry point */
    Elf32_Off     e_phoff;        /* Program hdr offset */
    Elf32_Off     e_shoff;        /* Section hdr offset */
    Elf32_Word    e_flags;        /* Processor flags */
    Elf32_Half    e_ehsize;        /* sizeof ehdr */
    Elf32_Half    e_phentsize;        /* Program header entry size */
    Elf32_Half    e_phnum;        /* Number of program headers */
    Elf32_Half    e_shentsize;        /* Section header entry size */
    Elf32_Half    e_shnum;        /* Number of section headers */
    Elf32_Half    e_shstrndx;        /* String table index */
} Elf32_Ehdr;

/* e_ident offsets */
# define    EI_MAG0        0    /* '\177' */
# define    EI_MAG1        1    /* 'E'    */
# define    EI_MAG2        2    /* 'L'    */
# define    EI_MAG3        3    /* 'F'    */
# define    EI_CLASS    4    /* File class */
# define    EI_DATA        5    /* Data encoding */
# define    EI_VERSION    6    /* File version */
# define    EI_OSABI    7    /* Operating system/ABI identification */
# define    EI_ABIVERSION    8    /* ABI version */
# define    EI_PAD        9    /* Start of padding bytes up to EI_NIDENT*/

/* e_ident[ELFMAG0,ELFMAG3] */
# define    ELFMAG0        0x7f
# define    ELFMAG1        'E'
# define    ELFMAG2        'L'
# define    ELFMAG3        'F'
# define    ELFMAG        "\177ELF"
# define    SELFMAG        4

/* e_ident[EI_CLASS] */
# define    ELFCLASSNONE    0    /* Invalid class */
# define    ELFCLASS32    1    /* 32-bit objects */
# define    ELFCLASS64    2    /* 64-bit objects */
# define    ELFCLASSNUM    3

/* e_ident[EI_DATA] */
# define    ELFDATANONE    0    /* Invalid data encoding */
# define    ELFDATA2LSB    1    /* 2's complement values, LSB first */
# define    ELFDATA2MSB    2    /* 2's complement values, MSB first */

/* e_ident[EI_VERSION] */
# define    EV_NONE        0    /* Invalid version */
# define    EV_CURRENT    1    /* Current version */
# define    EV_NUM        2

/* e_ident[EI_OSABI] */
# define    ELFOSABI_SYSV        0    /* UNIX System V ABI */
# define    ELFOSABI_HPUX        1    /* HP-UX operating system */
# define ELFOSABI_NETBSD        2    /* NetBSD */
# define ELFOSABI_LINUX        3    /* GNU/Linux */
# define ELFOSABI_HURD        4    /* GNU/Hurd */
# define ELFOSABI_86OPEN        5    /* 86Open */
# define ELFOSABI_SOLARIS    6    /* Solaris */
# define ELFOSABI_MONTEREY    7    /* Monterey */
# define ELFOSABI_IRIX        8    /* IRIX */
# define ELFOSABI_FREEBSD    9    /* FreeBSD */
# define ELFOSABI_TRU64        10    /* TRU64 UNIX */
# define ELFOSABI_MODESTO    11    /* Novell Modesto */
# define ELFOSABI_OPENBSD    12    /* OpenBSD */
/* Unofficial OSABIs follow */
# define ELFOSABI_ARM        97    /* ARM */
# define    ELFOSABI_STANDALONE    255    /* Standalone (embedded) application */

/* e_type */
# define    ET_NONE        0    /* No file type */
# define    ET_REL        1    /* Relocatable file */
# define    ET_EXEC        2    /* Executable file */
# define    ET_DYN        3    /* Shared object file */
# define    ET_CORE        4    /* Core file */
# define    ET_NUM        5

# define    ET_LOOS        0xfe00    /* Operating system specific range */
# define    ET_HIOS        0xfeff
# define    ET_LOPROC    0xff00    /* Processor-specific range */
# define    ET_HIPROC    0xffff

/* e_machine */
# define    EM_NONE        0    /* No machine */
# define    EM_M32        1    /* AT&T WE 32100 */
# define    EM_SPARC    2    /* SPARC */
# define    EM_386        3    /* Intel 80386 */
# define    EM_68K        4    /* Motorola 68000 */
# define    EM_88K        5    /* Motorola 88000 */
# define    EM_486        6    /* Intel 80486 */
# define    EM_860        7    /* Intel 80860 */
# define    EM_MIPS        8    /* MIPS I Architecture */
# define    EM_S370        9    /* Amdahl UTS on System/370 */
# define    EM_MIPS_RS3_LE    10    /* MIPS RS3000 Little-endian */
            /* 11-14 - Reserved */
# define    EM_RS6000    11    /* IBM RS/6000 XXX reserved */
# define    EM_PARISC    15    /* Hewlett-Packard PA-RISC */
# define    EM_NCUBE    16    /* NCube XXX reserved */
# define    EM_VPP500    17    /* Fujitsu VPP500 */
# define    EM_SPARC32PLUS    18    /* Enhanced instruction set SPARC */
# define    EM_960        19    /* Intel 80960 */
# define    EM_PPC        20    /* PowerPC */
# define    EM_PPC64    21    /* 64-bit PowerPC */
            /* 22-35 - Reserved */
# define    EM_V800        36    /* NEC V800 */
# define    EM_FR20        37    /* Fujitsu FR20 */
# define    EM_RH32        38    /* TRW RH-32 */
# define    EM_RCE        39    /* Motorola RCE */
# define    EM_ARM        40    /* Advanced RISC Machines ARM */
# define    EM_ALPHA    41    /* DIGITAL Alpha */
# define    EM_SH        42    /* Hitachi Super-H */
# define    EM_SPARCV9    43    /* SPARC Version 9 */
# define    EM_TRICORE    44    /* Siemens Tricore */
# define    EM_ARC        45    /* Argonaut RISC Core */
# define    EM_H8_300    46    /* Hitachi H8/300 */
# define    EM_H8_300H    47    /* Hitachi H8/300H */
# define    EM_H8S        48    /* Hitachi H8S */
# define    EM_H8_500    49    /* Hitachi H8/500 */
# define    EM_IA_64    50    /* Intel Merced Processor */
# define    EM_MIPS_X    51    /* Stanford MIPS-X */
# define    EM_COLDFIRE    52    /* Motorola Coldfire */
# define    EM_68HC12    53    /* Motorola MC68HC12 */
# define    EM_MMA        54    /* Fujitsu MMA Multimedia Accelerator */
# define    EM_PCP        55    /* Siemens PCP */
# define    EM_NCPU        56    /* Sony nCPU embedded RISC processor */
# define    EM_NDR1        57    /* Denso NDR1 microprocessor */
# define    EM_STARCORE    58    /* Motorola Star*Core processor */
# define    EM_ME16        59    /* Toyota ME16 processor */
# define    EM_ST100    60    /* STMicroelectronics ST100 processor */
# define    EM_TINYJ    61    /* Advanced Logic Corp. TinyJ embedded family processor */
# define    EM_X86_64    62    /* AMD x86-64 architecture */
# define    EM_PDSP        63    /* Sony DSP Processor */
            /* 64-65 - Reserved */
# define    EM_FX66        66    /* Siemens FX66 microcontroller */
# define    EM_ST9PLUS    67    /* STMicroelectronics ST9+ 8/16 bit microcontroller */
# define    EM_ST7        68    /* STMicroelectronics ST7 8-bit microcontroller */
# define    EM_68HC16    69    /* Motorola MC68HC16 Microcontroller */
# define    EM_68HC11    70    /* Motorola MC68HC11 Microcontroller */
# define    EM_68HC08    71    /* Motorola MC68HC08 Microcontroller */
# define    EM_68HC05    72    /* Motorola MC68HC05 Microcontroller */
# define    EM_SVX        73    /* Silicon Graphics SVx */
# define    EM_ST19        74    /* STMicroelectronics ST19 8-bit cpu */
# define    EM_VAX        75    /* Digital VAX */
# define    EM_CRIS        76    /* Axis Communications 32-bit embedded processor */
# define    EM_JAVELIN    77    /* Infineon Technologies 32-bit embedded cpu */
# define    EM_FIREPATH    78    /* Element 14 64-bit DSP processor */
# define    EM_ZSP        79    /* LSI Logic's 16-bit DSP processor */
# define    EM_MMIX        80    /* Donald Knuth's educational 64-bit processor */
# define    EM_HUANY    81    /* Harvard's machine-independent format */
# define    EM_PRISM    82    /* SiTera Prism */
# define    EM_AVR        83    /* Atmel AVR 8-bit microcontroller */
# define    EM_FR30        84    /* Fujitsu FR30 */
# define    EM_D10V        85    /* Mitsubishi D10V */
# define    EM_D30V        86    /* Mitsubishi D30V */
# define    EM_V850        87    /* NEC v850 */
# define    EM_M32R        88    /* Mitsubishi M32R */
# define    EM_MN10300    89    /* Matsushita MN10300 */
# define    EM_MN10200    90    /* Matsushita MN10200 */
# define    EM_PJ        91    /* picoJava */
# define    EM_OPENRISC    92    /* OpenRISC 32-bit embedded processor */
# define    EM_ARC_A5    93    /* ARC Cores Tangent-A5 */
# define    EM_XTENSA    94    /* Tensilica Xtensa Architecture */
# define    EM_NS32K    97    /* National Semiconductor 32000 series */

/* Unofficial machine types follow */
# define    EM_ALPHA_EXP    36902    /* used by NetBSD/alpha; obsolete */
# define    EM_NUM        36903

/*
 * Program Header
 */
typedef struct
{
    Elf32_Word    p_type;        /* entry type */
    Elf32_Off    p_offset;    /* offset */
    Elf32_Addr    p_vaddr;    /* virtual address */
    Elf32_Addr    p_paddr;    /* physical address */
    Elf32_Word    p_filesz;    /* file size */
    Elf32_Word    p_memsz;    /* memory size */
    Elf32_Word    p_flags;    /* flags */
    Elf32_Word    p_align;    /* memory & file alignment */
} Elf32_Phdr;

/* p_type */
# define    PT_NULL        0        /* Program header table entry unused */
# define    PT_LOAD        1        /* Loadable program segment */
# define    PT_DYNAMIC    2        /* Dynamic linking information */
# define    PT_INTERP    3        /* Program interpreter */
# define    PT_NOTE        4        /* Auxiliary information */
# define    PT_SHLIB    5        /* Reserved, unspecified semantics */
# define    PT_PHDR        6        /* Entry for header table itself */
# define    PT_NUM        7

/* p_flags */
# define    PF_R        0x4    /* Segment is readable */
# define    PF_W        0x2    /* Segment is writable */
# define    PF_X        0x1    /* Segment is executable */

# define    PF_MASKOS    0x0ff00000    /* Opersting system specific values */
# define    PF_MASKPROC    0xf0000000    /* Processor-specific values */

# define    PT_LOPROC    0x70000000    /* Processor-specific range */
# define    PT_HIPROC    0x7fffffff

# define    PT_MIPS_REGINFO    0x70000000

uint32_t load_elf(const char *path);
int exec(const char *path, char **args);

#endif /* !ELF_H */
