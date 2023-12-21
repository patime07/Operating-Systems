#ifndef STRINGS_H
#define STRINGS_H

#include "../cpu/types.h"
#include "mem.h"

void int_to_ascii(int n, char str[], int size);
void hex_to_ascii(int n, char str[], int size);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
int strncmp(char s1[], char s2[], u32 size);
int sstrlen( char s1[], u32 size);

#endif
