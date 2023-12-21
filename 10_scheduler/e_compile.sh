#!/bin/bash
### interrupt.asm has no .c counterpart so it needs to be built first
nasm ./cpu/interrupt.asm -f elf -o interrupt.o

### Alternatively to see assembly output -S
### Generate .s the assembly language output prior to generating .o machine files -S
### gcc -g -m32 -fno-pie -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror ./libc/mem.c ./libc/string.c ./drivers/keyboard.c ./drivers/screen.c ./cpu/isr.c ./cpu/ports.c ./cpu/idt.c ./cpu/timer.c ./libc/globals.c -S hello.c


### Generate .o machine files but do not link -c
gcc -g -m32 -fno-pie -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror ./libc/mem.c ./libc/string.c ./drivers/keyboard.c ./drivers/screen.c ./cpu/isr.c ./cpu/ports.c ./cpu/idt.c ./cpu/timer.c ./libc/globals.c -c --entry start_ hello.c

### Do the final link to create executable "final" and set entry to 0x10000
ld -m elf_i386 -o hey_there -Ttext 0x10000 *.o --oformat binary --entry start_
