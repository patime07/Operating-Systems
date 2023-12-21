#ifndef KERNEL_H
#define KERNEL_H

#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/globals.h"
#include "../libc/mem.h"
#include "../libc/linked.h"

void user_input(char *input);

#endif
