/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Provides an interface to the EMMC controller and commands for interacting
 * with an emmc card */

/* References:
 *
 * PLSS     - SD Group Physical Layer Simplified Specification ver 3.00
 * HCSS        - SD Group Host Controller Simplified Specification ver 3.00
 *
 * Broadcom BCM2835 Peripherals Guide
 */

#include "drivers/imports/emmc.h"

static char driver_name[] = "emmc";
static char device_name[] = "emmc0";

static uint32_t hci_ver = 0;
static uint32_t capabilities_0 = 0;
static uint32_t capabilities_1 = 0;

static uint32_t emmc_commands[] = {
    SD_CMD_INDEX(0),
    SD_CMD_RESERVED(1),
    SD_CMD_INDEX(2) | SD_RESP_R2,
    SD_CMD_INDEX(3) | SD_RESP_R6,
    SD_CMD_INDEX(4),
    SD_CMD_INDEX(5) | SD_RESP_R4,
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_INDEX(7) | SD_RESP_R1b,
    SD_CMD_INDEX(8) | SD_RESP_R7,
    SD_CMD_INDEX(9) | SD_RESP_R2,
    SD_CMD_INDEX(10) | SD_RESP_R2,
    SD_CMD_INDEX(11) | SD_RESP_R1,
    SD_CMD_INDEX(12) | SD_RESP_R1b | SD_CMD_TYPE_ABORT,
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_INDEX(15),
    SD_CMD_INDEX(16) | SD_RESP_R1,
    SD_CMD_INDEX(17) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(18) | SD_RESP_R1 | SD_DATA_READ | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_INDEX(19) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(20) | SD_RESP_R1b,
    SD_CMD_RESERVED(21),
    SD_CMD_RESERVED(22),
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_INDEX(24) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(25) | SD_RESP_R1 | SD_DATA_WRITE | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_RESERVED(26),
    SD_CMD_INDEX(27) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(28) | SD_RESP_R1b,
    SD_CMD_INDEX(29) | SD_RESP_R1b,
    SD_CMD_INDEX(30) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(31),
    SD_CMD_INDEX(32) | SD_RESP_R1,
    SD_CMD_INDEX(33) | SD_RESP_R1,
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_INDEX(38) | SD_RESP_R1b,
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_RESERVED(41),
    SD_CMD_RESERVED(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_RESERVED(51),
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_INDEX(55) | SD_RESP_R1,
    SD_CMD_INDEX(56) | SD_RESP_R1 | SD_CMD_ISDATA,
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

static uint32_t emmc_acommands[] = {
    SD_CMD_RESERVED(0),
    SD_CMD_RESERVED(1),
    SD_CMD_RESERVED(2),
    SD_CMD_RESERVED(3),
    SD_CMD_RESERVED(4),
    SD_CMD_RESERVED(5),
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_RESERVED(7),
    SD_CMD_RESERVED(8),
    SD_CMD_RESERVED(9),
    SD_CMD_RESERVED(10),
    SD_CMD_RESERVED(11),
    SD_CMD_RESERVED(12),
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_RESERVED(15),
    SD_CMD_RESERVED(16),
    SD_CMD_RESERVED(17),
    SD_CMD_RESERVED(18),
    SD_CMD_RESERVED(19),
    SD_CMD_RESERVED(20),
    SD_CMD_RESERVED(21),
    SD_CMD_INDEX(22) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_RESERVED(24),
    SD_CMD_RESERVED(25),
    SD_CMD_RESERVED(26),
    SD_CMD_RESERVED(27),
    SD_CMD_RESERVED(28),
    SD_CMD_RESERVED(29),
    SD_CMD_RESERVED(30),
    SD_CMD_RESERVED(31),
    SD_CMD_RESERVED(32),
    SD_CMD_RESERVED(33),
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_RESERVED(38),
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_INDEX(41) | SD_RESP_R3,
    SD_CMD_INDEX(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_INDEX(51) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_RESERVED(55),
    SD_CMD_RESERVED(56),
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

static void emmc_power_off()
{
    /* Power off the SD card */
    uint32_t control0 = inb(EMMC_BASE + EMMC_CONTROL0);
    control0 &= ~(1 << 8);    // Set SD Bus Power bit off in Power Control Register
    outb(EMMC_BASE + EMMC_CONTROL0, control0);
}

static uint32_t emmc_get_base_clock_hz()
{
    uint32_t base_clock;
#if SDHCI_IMPLEMENTATION == SDHCI_IMPLEMENTATION_GENERIC
    capabilities_0 = inb(EMMC_BASE + EMMC_CAPABILITIES_0);
    base_clock = ((capabilities_0 >> 8) & 0xff) * 1000000;
#elif SDHCI_IMPLEMENTATION == SDHCI_IMPLEMENTATION_BCM_2708
    uint32_t mb_addr = 0x40007000;        // 0x7000 in L2 cache coherent mode
    volatile uint32_t *mailbuffer = (uint32_t *)mb_addr;

    /* Get the base clock rate */
    // set up the buffer
    mailbuffer[0] = 8 * 4;        // size of this message
    mailbuffer[1] = 0;            // this is a request

    // next comes the first tag
    mailbuffer[2] = 0x00030002;    // get clock rate tag
    mailbuffer[3] = 0x8;        // value buffer size
    mailbuffer[4] = 0x4;        // is a request, value length = 4
    mailbuffer[5] = 0x1;        // clock id + space to return clock id
    mailbuffer[6] = 0;            // space to return rate (in Hz)

    // closing tag
    mailbuffer[7] = 0;

    // send the message
    write_mailbox(MAILBOX_PROP, mb_addr);

    // read the response
    read_mailbox(MAILBOX_PROP);

    if(mailbuffer[1] != REP_SUCCESS)
        return 0;

    if(mailbuffer[5] != 0x1)
        return 0;

    base_clock = mailbuffer[6];
#else
    return 0;
#endif

    return base_clock;
}

#if SDHCI_IMPLEMENTATION == SDHCI_IMPLEMENTATION_BCM_2708
static int bcm_2708_power_off()
{
    uint32_t mb_addr = 0x40007000;        // 0x7000 in L2 cache coherent mode
    volatile uint32_t *mailbuffer = (uint32_t *)mb_addr;

    /* Power off the SD card */
    // set up the buffer
    mailbuffer[0] = 8 * 4;        // size of this message
    mailbuffer[1] = 0;            // this is a request

    // next comes the first tag
    mailbuffer[2] = 0x00028001;    // set power state tag
    mailbuffer[3] = 0x8;        // value buffer size
    mailbuffer[4] = 0x8;        // is a request, value length = 8
    mailbuffer[5] = 0x0;        // device id and device id also returned here
    mailbuffer[6] = 0x2;        // set power off, wait for stable and returns state

    // closing tag
    mailbuffer[7] = 0;

    // send the message
    write_mailbox(MAILBOX_PROP, mb_addr);

    // read the response
    read_mailbox(MAILBOX_PROP);

    if(mailbuffer[1] != REP_SUCCESS)
        return -1;

    if(mailbuffer[5] != 0x0)
        return -1;

    if((mailbuffer[6] & 0x3) != 0)
        return 1;

    return 0;
}

static int bcm_2708_power_on()
{
    uint32_t mb_addr = 0x40007000;        // 0x7000 in L2 cache coherent mode
    volatile uint32_t *mailbuffer = (uint32_t *)mb_addr;

    /* Power on the SD card */
    // set up the buffer
    mailbuffer[0] = 8 * 4;        // size of this message
    mailbuffer[1] = 0;            // this is a request

    // next comes the first tag
    mailbuffer[2] = 0x00028001;    // set power state tag
    mailbuffer[3] = 0x8;        // value buffer size
    mailbuffer[4] = 0x8;        // is a request, value length = 8
    mailbuffer[5] = 0x0;        // device id and device id also returned here
    mailbuffer[6] = 0x3;        // set power off, wait for stable and returns state

    // closing tag
    mailbuffer[7] = 0;

    // send the message
    write_mailbox(MAILBOX_PROP, mb_addr);

    // read the response
    read_mailbox(MAILBOX_PROP);

    if(mailbuffer[1] != REP_SUCCESS)
        return -1;

    if(mailbuffer[5] != 0x0)
        return -1;

    if((mailbuffer[6] & 0x3) != 1)
        return 1;

    return 0;
}

static int bcm_2708_power_cycle()
{
    if(bcm_2708_power_off() < 0)
        return -1;

    wait(5000);

    return bcm_2708_power_on();
}
#endif

// Set the clock dividers to generate a target value
static uint32_t emmc_get_clock_divider(uint32_t base_clock, uint32_t target_rate)
{
    uint32_t targetted_divisor = 0;
    if(target_rate > base_clock)
        targetted_divisor = 1;
    else
    {
        targetted_divisor = base_clock / target_rate;
        uint32_t mod = base_clock % target_rate;
        if(mod)
            targetted_divisor--;
    }

    // Decide on the clock mode to use

    // Currently only 10-bit divided clock mode is supported

    if(hci_ver >= 2)
    {
        // HCI version 3 or greater supports 10-bit divided clock mode
        // This requires a power-of-two divider

        // Find the first bit set
        int divisor = -1;
        for(int first_bit = 31; first_bit >= 0; first_bit--)
        {
            uint32_t bit_test = (1 << first_bit);
            if(targetted_divisor & bit_test)
            {
                divisor = first_bit;
                targetted_divisor &= ~bit_test;
                if(targetted_divisor)
                    divisor++;
                break;
            }
        }

        if(divisor == -1)
            divisor = 31;
        if(divisor >= 32)
            divisor = 31;

        if(divisor != 0)
            divisor = (1 << (divisor - 1));

        if(divisor >= 0x400)
            divisor = 0x3ff;

        uint32_t freq_select = divisor & 0xff;
        uint32_t upper_bits = (divisor >> 8) & 0x3;
        uint32_t ret = (freq_select << 8) | (upper_bits << 6) | (0 << 5);

        return ret;
    }
    else
        return SD_GET_CLOCK_DIVIDER_FAIL;
}

// Switch the clock rate whilst running
static int emmc_switch_clock_rate(uint32_t base_clock, uint32_t target_rate)
{
    // Decide on an appropriate divider
    uint32_t divider = emmc_get_clock_divider(base_clock, target_rate);
    if(divider == SD_GET_CLOCK_DIVIDER_FAIL)
        return -1;

    // Wait for the command inhibit (CMD and DAT) bits to clear
    while(inb(EMMC_BASE + EMMC_STATUS) & 0x3)
        wait(1000);

    // Set the SD clock off
    uint32_t control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 &= ~(1 << 2);
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    wait(2000);

    // Write the new divider
    control1 &= ~0xffe0;        // Clear old setting + clock generator select
    control1 |= divider;
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    wait(2000);

    // Enable the SD clock
    control1 |= (1 << 2);
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    wait(2000);

    return 0;
}

static void TIMEOUT_WAIT(int cond, uint64_t usec)
{
    uint64_t time = get_time();
    do
    {
        uint64_t time1 = get_time();
        if (time1 >= time + usec)
            break;
    } while(!cond);
}

// Reset the CMD line
static int emmc_reset_cmd()
{
    uint32_t control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 |= SD_RESET_CMD;
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    TIMEOUT_WAIT((inb(EMMC_BASE + EMMC_CONTROL1) & SD_RESET_CMD) == 0, 1000000);
    if((inb(EMMC_BASE + EMMC_CONTROL1) & SD_RESET_CMD) != 0)
        return -1;
    return 0;
}

// Reset the CMD line
static int emmc_reset_dat()
{
    uint32_t control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 |= SD_RESET_DAT;
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    TIMEOUT_WAIT((inb(EMMC_BASE + EMMC_CONTROL1) & SD_RESET_DAT) == 0, 1000000);
    if((inb(EMMC_BASE + EMMC_CONTROL1) & SD_RESET_DAT) != 0)
        return -1;
    return 0;
}

static uint32_t byte_swap(unsigned int in)
{
    uint32_t b0 = in & 0xFF;
    uint32_t b1 = (in >> 8) & 0xFF;
    uint32_t b2 = (in >> 16) & 0xFF;
    uint32_t b3 = (in >> 24) & 0xFF;
    uint32_t ret = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    return ret;
}

static void write_word(uint32_t val, uint8_t* buf, int offset)
{
    buf[offset + 0] = val & 0xff;
    buf[offset + 1] = (val >> 8) & 0xff;
    buf[offset + 2] = (val >> 16) & 0xff;
    buf[offset + 3] = (val >> 24) & 0xff;
}

static uint32_t read_word(uint8_t* buf, int offset)
{
    uint32_t b0 = buf[offset + 0] & 0xff;
    uint32_t b1 = buf[offset + 1] & 0xff;
    uint32_t b2 = buf[offset + 2] & 0xff;
    uint32_t b3 = buf[offset + 3] & 0xff;
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void memcpy(void *dst, const void *src, uint32_t size)
{
    for (int i = 0; i < size; i++)
        ((char *)dst)[i] = ((char *)src)[i];
}

void memset(void *ptr, int c, uint32_t size)
{
    for (int i = 0; i < size; i++)
        ((char *)ptr)[i] = c;
}

static void emmc_issue_command_int(struct emmc_block_dev *dev, uint32_t cmd_reg, uint32_t argument, useconds_t timeout)
{
    dev->last_cmd_reg = cmd_reg;
    dev->last_cmd_success = 0;

    // This is as per HCSS 3.7.1.1/3.7.2.2

    // Check Command Inhibit
    while(inb(EMMC_BASE + EMMC_STATUS) & 0x1)
        wait(1000);

    // Is the command with busy?
    if((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B)
    {
        // With busy

        // Is is an abort command?
        if((cmd_reg & SD_CMD_TYPE_MASK) != SD_CMD_TYPE_ABORT)
        {
            // Not an abort command

            // Wait for the data line to be free
            while(inb(EMMC_BASE + EMMC_STATUS) & 0x2)
                wait(1000);
        }
    }

    // Is this a DMA transfer?
    int is_emmcma = 0;
    if((cmd_reg & SD_CMD_ISDATA) && (dev->use_emmcma))
        is_emmcma = 1;

    if(is_emmcma)
    {
        // Set system address register (ARGUMENT2 in RPi)

        // We need to define a 4 kiB aligned buffer to use here
        // Then convert its virtual address to a bus address
        outb(EMMC_BASE + EMMC_ARG2, SDMA_BUFFER_PA);
    }

    // Set block size and block count
    // For now, block size = 512 bytes, block count = 1,
    //  host SDMA buffer boundary = 4 kiB
    if(dev->blocks_to_transfer > 0xffff)
    {
        dev->last_cmd_success = 0;
        return;
    }
    uint32_t blksizecnt = dev->block_size | (dev->blocks_to_transfer << 16);
    outb(EMMC_BASE + EMMC_BLKSIZECNT, blksizecnt);

    // Set argument 1 reg
    outb(EMMC_BASE + EMMC_ARG1, argument);

    if(is_emmcma)
    {
        // Set Transfer mode register
        cmd_reg |= SD_CMD_DMA;
    }

    // Set command reg
    outb(EMMC_BASE + EMMC_CMDTM, cmd_reg);

    wait(2000);

    // Wait for command complete interrupt
    TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_INTERRUPT) & 0x8001, timeout);
    uint32_t irpts = inb(EMMC_BASE + EMMC_INTERRUPT);

    // Clear command complete status
    outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff0001);

    // Test for errors
    if((irpts & 0xffff0001) != 0x1)
    {
        dev->last_error = irpts & 0xffff0000;
        dev->last_interrupt = irpts;
        return;
    }

    wait(2000);

    // Get response data
    switch(cmd_reg & SD_CMD_RSPNS_TYPE_MASK)
    {
        case SD_CMD_RSPNS_TYPE_48:
        case SD_CMD_RSPNS_TYPE_48B:
            dev->last_r0 = inb(EMMC_BASE + EMMC_RESP0);
            break;

        case SD_CMD_RSPNS_TYPE_136:
            dev->last_r0 = inb(EMMC_BASE + EMMC_RESP0);
            dev->last_r1 = inb(EMMC_BASE + EMMC_RESP1);
            dev->last_r2 = inb(EMMC_BASE + EMMC_RESP2);
            dev->last_r3 = inb(EMMC_BASE + EMMC_RESP3);
            break;
    }

    // If with data, wait for the appropriate interrupt
    if((cmd_reg & SD_CMD_ISDATA) && (is_emmcma == 0))
    {
        uint32_t wr_irpt;
        int is_write = 0;
        if(cmd_reg & SD_CMD_DAT_DIR_CH)
            wr_irpt = (1 << 5);     // read
        else
        {
            is_write = 1;
            wr_irpt = (1 << 4);     // write
        }

        int cur_block = 0;
        uint32_t *cur_buf_addr = (uint32_t *)dev->buf;
        while(cur_block < dev->blocks_to_transfer)
        {
            TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_INTERRUPT) & (wr_irpt | 0x8000), timeout);
            irpts = inb(EMMC_BASE + EMMC_INTERRUPT);
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff0000 | wr_irpt);

            if((irpts & (0xffff0000 | wr_irpt)) != wr_irpt)
            {
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }

            // Transfer the block
            size_t cur_byte_no = 0;
            while(cur_byte_no < dev->block_size)
            {
                if(is_write)
                {
                    uint32_t data = read_word((uint8_t *)cur_buf_addr, 0);
                    outb(EMMC_BASE + EMMC_DATA, data);
                }
                else
                {
                    uint32_t data = inb(EMMC_BASE + EMMC_DATA);
                    write_word(data, (uint8_t *)cur_buf_addr, 0);
                }
                cur_byte_no += 4;
                cur_buf_addr++;
            }

            cur_block++;
        }
    }

    // Wait for transfer complete (set if read/write transfer or with busy)
    if((((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) ||
       (cmd_reg & SD_CMD_ISDATA)) && (is_emmcma == 0))
    {
        // First check command inhibit (DAT) is not already 0
        if((inb(EMMC_BASE + EMMC_STATUS) & 0x2) == 0)
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff0002);
        else
        {
            TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_INTERRUPT) & 0x8002, timeout);
            irpts = inb(EMMC_BASE + EMMC_INTERRUPT);
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff0002);

            // Handle the case where both data timeout and transfer complete
            //  are set - transfer complete overrides data timeout: HCSS 2.2.17
            if(((irpts & 0xffff0002) != 0x2) && ((irpts & 0xffff0002) != 0x100002))
            {
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff0002);
        }
    }
    else if (is_emmcma)
    {
        // For SDMA transfers, we have to wait for either transfer complete,
        //  DMA int or an error

        // First check command inhibit (DAT) is not already 0
        if((inb(EMMC_BASE + EMMC_STATUS) & 0x2) == 0)
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff000a);
        else
        {
            TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_INTERRUPT) & 0x800a, timeout);
            irpts = inb(EMMC_BASE + EMMC_INTERRUPT);
            outb(EMMC_BASE + EMMC_INTERRUPT, 0xffff000a);

            // Detect errors
            if((irpts & 0x8000) && ((irpts & 0x2) != 0x2))
            {
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }

            // Detect DMA interrupt without transfer complete
            // Currently not supported - all block sizes should fit in the
            //  buffer
            if((irpts & 0x8) && ((irpts & 0x2) != 0x2))
            {
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }

            // Detect transfer complete
            if(irpts & 0x2)
                memcpy(dev->buf, (const void *)SDMA_BUFFER, dev->block_size);
            else
            {
                if((irpts == 0) && ((inb(EMMC_BASE + EMMC_STATUS) & 0x3) == 0x2))
                    outb(EMMC_BASE + EMMC_CMDTM, emmc_commands[STOP_TRANSMISSION]);
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }
        }
    }

    // Return success
    dev->last_cmd_success = 1;
}

static void emmc_handle_card_interrupt(struct emmc_block_dev *dev)
{
    // Handle a card interrupt
    if(dev->card_rca)
        emmc_issue_command_int(dev, emmc_commands[SEND_STATUS], dev->card_rca << 16,
                         500000);
}

static void emmc_handle_interrupts(struct emmc_block_dev *dev)
{
    uint32_t irpts = inb(EMMC_BASE + EMMC_INTERRUPT);
    uint32_t reset_mask = 0;

    if(irpts & SD_COMMAND_COMPLETE)
        reset_mask |= SD_COMMAND_COMPLETE;
    if(irpts & SD_TRANSFER_COMPLETE)
        reset_mask |= SD_TRANSFER_COMPLETE;
    if(irpts & SD_BLOCK_GAP_EVENT)
        reset_mask |= SD_BLOCK_GAP_EVENT;
    if(irpts & SD_DMA_INTERRUPT)
        reset_mask |= SD_DMA_INTERRUPT;
    if(irpts & SD_BUFFER_WRITE_READY)
    {
        reset_mask |= SD_BUFFER_WRITE_READY;
        emmc_reset_dat();
    }
    if(irpts & SD_BUFFER_READ_READY)
    {
        reset_mask |= SD_BUFFER_READ_READY;
        emmc_reset_dat();
    }
    if(irpts & SD_CARD_INSERTION)
        reset_mask |= SD_CARD_INSERTION;
    if(irpts & SD_CARD_REMOVAL)
    {
        reset_mask |= SD_CARD_REMOVAL;
        dev->card_removal = 1;
    }
    if(irpts & SD_CARD_INTERRUPT)
    {
        emmc_handle_card_interrupt(dev);
        reset_mask |= SD_CARD_INTERRUPT;
    }
    if(irpts & 0x8000)
        reset_mask |= 0xffff0000;
    outb(EMMC_BASE + EMMC_INTERRUPT, reset_mask);
}

int emmc_issue_command(struct emmc_block_dev *dev, uint32_t command, uint32_t argument, useconds_t timeout)
{
    // First, handle any pending interrupts
    emmc_handle_interrupts(dev);

    // Stop the command issue if it was the card remove interrupt that was
    //  handled
    if(dev->card_removal)
    {
        dev->last_cmd_success = 0;
        return -1;
    }

    // Now run the appropriate commands by calling emmc_issue_command_int()
    if(command & IS_APP_CMD)
    {
        command &= 0xff;
        if(emmc_acommands[command] == SD_CMD_RESERVED(0))
        {
            dev->last_cmd_success = 0;
            return -1;
        }
        dev->last_cmd = APP_CMD;

        uint32_t rca = 0;
        if(dev->card_rca)
            rca = dev->card_rca << 16;
        emmc_issue_command_int(dev, emmc_commands[APP_CMD], rca, timeout);
        if(dev->last_cmd_success)
        {
            dev->last_cmd = command | IS_APP_CMD;
            emmc_issue_command_int(dev, emmc_acommands[command], argument, timeout);
        }
    }
    else
    {
        if(emmc_commands[command] == SD_CMD_RESERVED(0))
        {
            dev->last_cmd_success = 0;
            return -1;
        }

        dev->last_cmd = command;
        emmc_issue_command_int(dev, emmc_commands[command], argument, timeout);
    }

    return 0;
}

int emmc_card_init(struct block_device **dev)
{
    // Check the sanity of the emmc_commands and emmc_acommands structures
    if(sizeof(emmc_commands) != (64 * sizeof(uint32_t)))
        return -1;
    if(sizeof(emmc_acommands) != (64 * sizeof(uint32_t)))
        return -1;

#if SDHCI_IMPLEMENTATION == SDHCI_IMPLEMENTATION_BCM_2708
    // Power cycle the card to ensure its in its startup state
    if(bcm_2708_power_cycle() != 0)
        return -1;
#endif

    // Read the controller version
    uint32_t ver = inb(EMMC_BASE + EMMC_SLOTISR_VER);
    uint32_t emmcversion = (ver >> 16) & 0xff;
    hci_ver = emmcversion;

    if(hci_ver < 2)
        return -1;

    // Reset the controller
    uint32_t control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 |= (1 << 24);
    // Disable clock
    control1 &= ~(1 << 2);
    control1 &= ~(1 << 0);
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    TIMEOUT_WAIT((inb(EMMC_BASE + EMMC_CONTROL1) & (0x7 << 24)) == 0, 1000000);
    if((inb(EMMC_BASE + EMMC_CONTROL1) & (0x7 << 24)) != 0)
        return -1;

    // Read the capabilities registers
    capabilities_0 = inb(EMMC_BASE + EMMC_CAPABILITIES_0);
    capabilities_1 = inb(EMMC_BASE + EMMC_CAPABILITIES_1);

    // Check for a valid card
    TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_STATUS) & (1 << 16), 500000);
    uint32_t status_reg = inb(EMMC_BASE + EMMC_STATUS);
    if((status_reg & (1 << 16)) == 0)
        return -1;

    // Clear control2
    outb(EMMC_BASE + EMMC_CONTROL2, 0);

    // Get the base clock rate
    uint32_t base_clock = emmc_get_base_clock_hz();
    if(base_clock == 0)
        base_clock = 100000000;

    // Set clock rate to something slow
    control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 |= 1;            // enable clock

    // Set to identification frequency (400 kHz)
    uint32_t f_id = emmc_get_clock_divider(base_clock, SD_CLOCK_ID);
    if(f_id == SD_GET_CLOCK_DIVIDER_FAIL)
        return -1;
    control1 |= f_id;

    control1 |= (7 << 16);        // data timeout = TMCLK * 2^10
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    TIMEOUT_WAIT(inb(EMMC_BASE + EMMC_CONTROL1) & 0x2, 0x1000000);
    if((inb(EMMC_BASE + EMMC_CONTROL1) & 0x2) == 0)
        return -1;

    // Enable the SD clock
    wait(2000);
    control1 = inb(EMMC_BASE + EMMC_CONTROL1);
    control1 |= 4;
    outb(EMMC_BASE + EMMC_CONTROL1, control1);
    wait(2000);

    // Mask off sending interrupts to the ARM
    outb(EMMC_BASE + EMMC_IRPT_EN, 0);
    // Reset interrupts
    outb(EMMC_BASE + EMMC_INTERRUPT, 0xffffffff);
    // Have all interrupts sent to the INTERRUPT register
    uint32_t irpt_mask = 0xffffffff & (~SD_CARD_INTERRUPT);
#ifdef SD_CARD_INTERRUPTS
    irpt_mask |= SD_CARD_INTERRUPT;
#endif
    outb(EMMC_BASE + EMMC_IRPT_MASK, irpt_mask);

    wait(2000);

    // Prepare the device structure
    struct emmc_block_dev *ret;
    if(*dev == NULL)
        ret = (struct emmc_block_dev *)kmalloc(sizeof(struct emmc_block_dev));
    else
        ret = (struct emmc_block_dev *)*dev;

    if (!ret)
        return -1;

    memset(ret, 0, sizeof(struct emmc_block_dev));
    ret->bd.driver_name = driver_name;
    ret->bd.device_name = device_name;
    ret->bd.block_size = 512;
    ret->bd.read = emmc_read;
#ifdef SD_WRITE_SUPPORT
    ret->bd.write = emmc_write;
#endif
    ret->bd.supports_multiple_block_read = 1;
    ret->bd.supports_multiple_block_write = 1;
    ret->base_clock = base_clock;

    // Send CMD0 to the card (reset to idle state)
    emmc_issue_command(ret, GO_IDLE_STATE, 0, 500000);
    if(FAIL(ret))
        return -1;

    // Send CMD8 to the card
    // Voltage supplied = 0x1 = 2.7-3.6V (standard)
    // Check pattern = 10101010b (as per PLSS 4.3.13) = 0xAA
    emmc_issue_command(ret, SEND_IF_COND, 0x1aa, 500000);
    int v2_later = 0;
    if(TIMEOUT(ret))
        v2_later = 0;
    else if(CMD_TIMEOUT(ret))
    {
        if(emmc_reset_cmd() == -1)
            return -1;
        outb(EMMC_BASE + EMMC_INTERRUPT, SD_ERR_MASK_CMD_TIMEOUT);
        v2_later = 0;
    }
    else if(FAIL(ret))
        return -1;
    else
    {
        if((ret->last_r0 & 0xfff) != 0x1aa)
            return -1;
        else
            v2_later = 1;
    }

    // Here we are supposed to check the response to CMD5 (HCSS 3.6)
    // It only returns if the card is a SDIO card
    emmc_issue_command(ret, IO_SET_OP_COND, 0, 10000);
    if(!TIMEOUT(ret))
    {
        if(CMD_TIMEOUT(ret))
        {
            if(emmc_reset_cmd() == -1)
                return -1;
            outb(EMMC_BASE + EMMC_INTERRUPT, SD_ERR_MASK_CMD_TIMEOUT);
        }
        else
            return -1;
    }

    // Call an inquiry ACMD41 (voltage window = 0) to get the OCR
    emmc_issue_command(ret, ACMD(41), 0, 500000);
    if(FAIL(ret))
        return -1;

    // Call initialization ACMD41
    int card_is_busy = 1;
    while(card_is_busy)
    {
        uint32_t v2_flags = 0;
        if(v2_later)
        {
            // Set SDHC support
            v2_flags |= (1 << 30);

            // Set 1.8v support
#ifdef SD_1_8V_SUPPORT
            if(!ret->failed_voltage_switch)
                v2_flags |= (1 << 24);
#endif

            // Enable SDXC maximum performance
#ifdef SDXC_MAXIMUM_PERFORMANCE
            v2_flags |= (1 << 28);
#endif
        }

        emmc_issue_command(ret, ACMD(41), 0x00ff8000 | v2_flags, 500000);
        if(FAIL(ret))
            return -1;

        if((ret->last_r0 >> 31) & 0x1)
        {
            // Initialization is complete
            ret->card_ocr = (ret->last_r0 >> 8) & 0xffff;
            ret->card_supports_emmchc = (ret->last_r0 >> 30) & 0x1;

#ifdef SD_1_8V_SUPPORT
            if(!ret->failed_voltage_switch)
                ret->card_supports_18v = (ret->last_r0 >> 24) & 0x1;
#endif

            card_is_busy = 0;
        }
        else
            wait(500000);
    }

    // At this point, we know the card is definitely an SD card, so will definitely
    //  support SDR12 mode which runs at 25 MHz
    emmc_switch_clock_rate(base_clock, SD_CLOCK_NORMAL);

    // A small wait before the voltage switch
    wait(5000);

    // Switch to 1.8V mode if possible
    if(ret->card_supports_18v)
    {
        // As per HCSS 3.6.1

        // Send VOLTAGE_SWITCH
        emmc_issue_command(ret, VOLTAGE_SWITCH, 0, 500000);
        if(FAIL(ret))
        {
            ret->failed_voltage_switch = 1;
            emmc_power_off();
            return emmc_card_init((struct block_device **)&ret);
        }

        // Disable SD clock
        control1 = inb(EMMC_BASE + EMMC_CONTROL1);
        control1 &= ~(1 << 2);
        outb(EMMC_BASE + EMMC_CONTROL1, control1);

        // Check DAT[3:0]
        status_reg = inb(EMMC_BASE + EMMC_STATUS);
        uint32_t dat30 = (status_reg >> 20) & 0xf;
        if(dat30 != 0)
        {
            ret->failed_voltage_switch = 1;
            emmc_power_off();
            return emmc_card_init((struct block_device **)&ret);
        }

        // Set 1.8V signal enable to 1
        uint32_t control0 = inb(EMMC_BASE + EMMC_CONTROL0);
        control0 |= (1 << 8);
        outb(EMMC_BASE + EMMC_CONTROL0, control0);

        // Wait 5 ms
        wait(5000);

        // Check the 1.8V signal enable is set
        control0 = inb(EMMC_BASE + EMMC_CONTROL0);
        if(((control0 >> 8) & 0x1) == 0)
        {
            ret->failed_voltage_switch = 1;
            emmc_power_off();
            return emmc_card_init((struct block_device **)&ret);
        }

        // Re-enable the SD clock
        control1 = inb(EMMC_BASE + EMMC_CONTROL1);
        control1 |= (1 << 2);
        outb(EMMC_BASE + EMMC_CONTROL1, control1);

        // Wait 1 ms
        wait(10000);

        // Check DAT[3:0]
        status_reg = inb(EMMC_BASE + EMMC_STATUS);
        dat30 = (status_reg >> 20) & 0xf;
        if(dat30 != 0xf)
        {
            ret->failed_voltage_switch = 1;
            emmc_power_off();
            return emmc_card_init((struct block_device **)&ret);
        }
    }

    // Send CMD2 to get the cards CID
    emmc_issue_command(ret, ALL_SEND_CID, 0, 500000);
    if(FAIL(ret))
        return -1;
    uint32_t card_cid_0 = ret->last_r0;
    uint32_t card_cid_1 = ret->last_r1;
    uint32_t card_cid_2 = ret->last_r2;
    uint32_t card_cid_3 = ret->last_r3;

    uint32_t *dev_id = (uint32_t *)kmalloc(4 * sizeof(uint32_t));
    dev_id[0] = card_cid_0;
    dev_id[1] = card_cid_1;
    dev_id[2] = card_cid_2;
    dev_id[3] = card_cid_3;
    ret->bd.device_id = (uint8_t *)dev_id;
    ret->bd.dev_id_len = 4 * sizeof(uint32_t);

    // Send CMD3 to enter the data state
    emmc_issue_command(ret, SEND_RELATIVE_ADDR, 0, 500000);
    if(FAIL(ret))
    {
        kfree(ret);
        return -1;
    }

    uint32_t cmd3_resp = ret->last_r0;

    ret->card_rca = (cmd3_resp >> 16) & 0xffff;
    uint32_t crc_error = (cmd3_resp >> 15) & 0x1;
    uint32_t illegal_cmd = (cmd3_resp >> 14) & 0x1;
    uint32_t error = (cmd3_resp >> 13) & 0x1;
    uint32_t status = (cmd3_resp >> 9) & 0xf;
    uint32_t ready = (cmd3_resp >> 8) & 0x1;

    if(crc_error)
    {
        kfree(ret);
        kfree(dev_id);
        return -1;
    }

    if(illegal_cmd)
    {
        kfree(ret);
        kfree(dev_id);
        return -1;
    }

    if(error)
    {
        kfree(ret);
        kfree(dev_id);
        return -1;
    }

    if(!ready)
    {
        kfree(ret);
        kfree(dev_id);
        return -1;
    }

    // Now select the card (toggles it to transfer state)
    emmc_issue_command(ret, SELECT_CARD, ret->card_rca << 16, 500000);
    if(FAIL(ret))
    {
        kfree(ret);
        return -1;
    }

    uint32_t cmd7_resp = ret->last_r0;
    status = (cmd7_resp >> 9) & 0xf;

    if((status != 3) && (status != 4))
    {
        kfree(ret);
        kfree(dev_id);
        return -1;
    }

    // If not an SDHC card, ensure BLOCKLEN is 512 bytes
    if(!ret->card_supports_emmchc)
    {
        emmc_issue_command(ret, SET_BLOCKLEN, 512, 500000);
        if(FAIL(ret))
        {
            kfree(ret);
            return -1;
        }
    }
    ret->block_size = 512;
    uint32_t controller_block_size = inb(EMMC_BASE + EMMC_BLKSIZECNT);
    controller_block_size &= (~0xfff);
    controller_block_size |= 0x200;
    outb(EMMC_BASE + EMMC_BLKSIZECNT, controller_block_size);

    // Get the cards SCR register
    ret->scr = (struct emmc_scr *)kmalloc(sizeof(struct emmc_scr));
    ret->buf = &ret->scr->scr[0];
    ret->block_size = 8;
    ret->blocks_to_transfer = 1;
    emmc_issue_command(ret, SEND_SCR, 0, 500000);
    ret->block_size = 512;
    if(FAIL(ret))
    {
        kfree(ret->scr);
        kfree(ret);
        return -1;
    }

    // Determine card version
    // Note that the SCR is big-endian
    uint32_t scr0 = byte_swap(ret->scr->scr[0]);
    ret->scr->emmc_version = SD_VER_UNKNOWN;
    uint32_t emmc_spec = (scr0 >> (56 - 32)) & 0xf;
    uint32_t emmc_spec3 = (scr0 >> (47 - 32)) & 0x1;
    uint32_t emmc_spec4 = (scr0 >> (42 - 32)) & 0x1;
    ret->scr->emmc_bus_widths = (scr0 >> (48 - 32)) & 0xf;
    if(emmc_spec == 0)
        ret->scr->emmc_version = SD_VER_1;
    else if(emmc_spec == 1)
        ret->scr->emmc_version = SD_VER_1_1;
    else if(emmc_spec == 2)
    {
        if(emmc_spec3 == 0)
            ret->scr->emmc_version = SD_VER_2;
        else if(emmc_spec3 == 1)
        {
            if(emmc_spec4 == 0)
                ret->scr->emmc_version = SD_VER_3;
            else if(emmc_spec4 == 1)
                ret->scr->emmc_version = SD_VER_4;
        }
    }

    if(ret->scr->emmc_bus_widths & 0x4)
    {
        // Set 4-bit transfer mode (ACMD6)
        // See HCSS 3.4 for the algorithm
#ifdef SD_4BIT_DATA
        // Disable card interrupt in host
        uint32_t old_irpt_mask = inb(EMMC_BASE + EMMC_IRPT_MASK);
        uint32_t new_iprt_mask = old_irpt_mask & ~(1 << 8);
        outb(EMMC_BASE + EMMC_IRPT_MASK, new_iprt_mask);

        // Send ACMD6 to change the card's bit mode
        emmc_issue_command(ret, SET_BUS_WIDTH, 0x2, 500000);
        if (!FAIL(ret))
        {
            // Change bit mode for Host
            uint32_t control0 = inb(EMMC_BASE + EMMC_CONTROL0);
            control0 |= 0x2;
            outb(EMMC_BASE + EMMC_CONTROL0, control0);

            // Re-enable card interrupt in host
            outb(EMMC_BASE + EMMC_IRPT_MASK, old_irpt_mask);
        }
#endif
    }

    // Reset interrupt register
    outb(EMMC_BASE + EMMC_INTERRUPT, 0xffffffff);

    *dev = (struct block_device *)ret;

    return 0;
}

static int emmc_ensure_data_mode(struct emmc_block_dev *edev)
{
    if(edev->card_rca == 0)
    {
        // Try again to initialise the card
        int ret = emmc_card_init((struct block_device **)&edev);
        if(ret != 0)
            return ret;
    }

    emmc_issue_command(edev, SEND_STATUS, edev->card_rca << 16, 500000);
    if(FAIL(edev))
    {
        edev->card_rca = 0;
        return -1;
    }

    uint32_t status = edev->last_r0;
    uint32_t cur_state = (status >> 9) & 0xf;
    if(cur_state == 3)
    {
        // Currently in the stand-by state - select it
        emmc_issue_command(edev, SELECT_CARD, edev->card_rca << 16, 500000);
        if(FAIL(edev))
        {
            edev->card_rca = 0;
            return -1;
        }
    }
    else if(cur_state == 5)
    {
        // In the data transfer state - cancel the transmission
        emmc_issue_command(edev, STOP_TRANSMISSION, 0, 500000);
        if(FAIL(edev))
        {
            edev->card_rca = 0;
            return -1;
        }

        // Reset the data circuit
        emmc_reset_dat();
    }
    else if(cur_state != 4)
    {
        // Not in the transfer state - re-initialise
        int ret = emmc_card_init((struct block_device **)&edev);
        if(ret != 0)
            return ret;
    }

    // Check again that we're now in the correct mode
    if(cur_state != 4)
    {
        emmc_issue_command(edev, SEND_STATUS, edev->card_rca << 16, 500000);
        if(FAIL(edev))
        {
            edev->card_rca = 0;
            return -1;
        }
        status = edev->last_r0;
        cur_state = (status >> 9) & 0xf;

        if(cur_state != 4)
        {
            edev->card_rca = 0;
            return -1;
        }
    }

    return 0;
}

#ifdef SDMA_SUPPORT
// We only support DMA transfers to buffers aligned on a 4 kiB boundary
static int emmc_suitable_for_dma(void *buf)
{
    if((uintptr_t)buf & 0xfff)
        return 0;
    else
        return 1;
}
#endif

static int emmc_do_data_command(struct emmc_block_dev *edev, int is_write, uint8_t *buf, size_t buf_size, uint32_t block_no)
{
    // PLSS table 4.20 - SDSC cards use byte addresses rather than block addresses
    if(!edev->card_supports_emmchc)
        block_no *= 512;

    // This is as per HCSS 3.7.2.1
    if(buf_size < edev->block_size)
        return -1;

    edev->blocks_to_transfer = buf_size / edev->block_size;
    if(buf_size % edev->block_size)
        return -1;
    edev->buf = buf;

    // Decide on the command to use
    int command;
    if(is_write)
    {
        if(edev->blocks_to_transfer > 1)
            command = WRITE_MULTIPLE_BLOCK;
        else
            command = WRITE_BLOCK;
    }
    else
    {
        if(edev->blocks_to_transfer > 1)
            command = READ_MULTIPLE_BLOCK;
        else
            command = READ_SINGLE_BLOCK;
    }

    int retry_count = 0;
    int max_retries = 3;
    while(retry_count < max_retries)
    {
#ifdef SDMA_SUPPORT
        // use SDMA for the first try only
        if((retry_count == 0) && emmc_suitable_for_dma(buf))
            edev->use_emmcma = 1;
        else
            edev->use_emmcma = 0;
#else
        edev->use_emmcma = 0;
#endif

        emmc_issue_command(edev, command, block_no, 5000000);

        if(SUCCESS(edev))
            break;
        else
            retry_count++;
    }
    if(retry_count == max_retries)
    {
        edev->card_rca = 0;
        return -1;
    }

    return 0;
}

int emmc_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no)
{
    // Check the status of the card
    struct emmc_block_dev *edev = (struct emmc_block_dev *)dev;
    if(emmc_ensure_data_mode(edev) != 0)
        return -1;

    if(emmc_do_data_command(edev, 0, buf, buf_size, block_no) < 0)
        return -1;

    return buf_size;
}

#ifdef SD_WRITE_SUPPORT
int emmc_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no)
{
    // Check the status of the card
    struct emmc_block_dev *edev = (struct emmc_block_dev *)dev;
    if(emmc_ensure_data_mode(edev) != 0)
        return -1;

    if(emmc_do_data_command(edev, 1, buf, buf_size, block_no) < 0)
        return -1;

    return buf_size;
}
#endif
