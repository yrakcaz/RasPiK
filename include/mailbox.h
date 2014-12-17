#ifndef MAILBOX_H
# define MAILBOX_H

# include <stdint.h>
# include "mmio.h"
# include "utils.h"
# include "timers.h"

# define MAILBOX_BASE    0x2000B880

# define MAILBOX_READ    0x00
# define MAILBOX_WRITE   0x20
# define MAILBOX_STATE   0x18

# define MAILBOX_FULL    0x80000000
# define MAILBOX_EMPTY   0x40000000

# define MAILBOX_TIMEOUT (1 << 20)

# define REP_SUCCESS     0x80000000
# define REP_FAILURE     0x80000001

# define KERNEL_START    0x80000000
# define CLOCK_TAG       0x00030002
# define POWERGET_TAG    0x00020001
# define POWERSET_TAG    0x00028001

void write_mailbox(uint8_t chan, uint32_t data);
uint32_t read_mailbox(uint8_t chan);
uint32_t getpower_mailbox(uint32_t device);
int setpower_mailbox(uint32_t device, uint32_t state);
uint32_t getclock_mailbox(uint32_t clock);

#endif /* !MAILBOX_H */
