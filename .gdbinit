set architecture arm
file kernel.elf
target remote localhost:1234
b k_start
continue
