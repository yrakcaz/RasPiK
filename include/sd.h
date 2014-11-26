#ifndef SD_H
# define SD_H

/* Return values */
# define SUCCESS 0
# define ERROR   1
# define RDONLY  2
# define NOTRDY  3
# define INVALID 4

/* SD status */
# define NOTINIT 0x01
# define NOSD    0x02
# define WPROT   0x04

/* SD types */
# define MMC     0x01
# define SD1     0x02
# define SD2     0x04
# define SDC     (SD1 | SD2)
# define BLK     0x08

/* SD Commands */
# define CMD0    0
# define CMD1    1
# define ACMD41  (0x80 + 41)
# define CMD8    8
# define CMD9    9
# define CMD10   10
# define CMD11   11
# define CMD12   12
# define CMD13   13
# define ACMD13  (0x80 + 13)
# define CMD16   16
# define CMD17   17
# define CMD18   18
# define CMD23   23
# define ACMD23  (0x80 + 23)
# define CMD24   24
# define CMD25   25
# define CMD41   41
# define CMD55   55
# define CMD58   58

/* Usefull defines */
# define CTRL_SYNC      0
# define CTRL_SCOUNT    1
# define CTRL_BLKSZ     2

/* External functions */
int init_sd(uint8_t drive);
int status_sd(uint8_t drive);
int read_sd(uint8_t drive, uint8_t *buffer, uint32_t sector, uint8_t count);
int write_sd(uint8_t drive, const uint8_t *buffer, uint32_t sector, uint8_t count);
int ioctl_sd(uint8_t drive, uint8_t command, void *buffer);

#endif /* !SD_H */
