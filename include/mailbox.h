#ifndef MAILBOX_H
# define MAILBOX_H

# include "mmio.h"
# include "utils.h"

# define MAILBOX_BASE    0x2000B880

# define MAILBOX_READ    0x00
# define MAILBOX_WRITE   0x20
# define MAILBOX_STATE   0x18

# define MAILBOX_FULL    0x80000000
# define MAILBOX_EMPTY   0x40000000

# define MAILBOX_TIMEOUT (1 << 20)

void write_mailbox(uint8_t chan, uint32_t data);
uint32_t verify_mailbox(uint8_t chan);

#endif /* !MAILBOX_H */
