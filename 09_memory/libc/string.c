#include "string.h"

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[], int size) {
   memory_set( (u8 *) str, 0, size);
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

// Convert the numeric value n to a character string of 0x#...# where # is hexadecimal values
// Size is the size of the char str[] array so number of hex digits is 1 less than size
// because you need a terminating 0
// the char str[] is erased with 0 values at start.
void hex_to_ascii(int n, char str[], int size) {
   memory_set( (u8 *) str, 0, size);
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    s32 tmp;
    int i;

    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp >= 0xA) append(str, tmp - 0xA + 'A');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'A');
    else append(str, tmp + '0');
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

// not K&R 
int sstrlen( char s[], u32 size)
{
   if( size <= 0) {
      return(0);
   }
   u32 i = 0;
   while ( (s[i] != '\0') && ( i < size)) { ++i; }
   return( i);
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* K&R 
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

// Not K&R
// Compare strings for first size characters
// if size > both string sizes compare whole string
int strncmp( char s1[], char s2[], u32 size)
{
   u32 i =  0;
   while( (s1[i] != '\0') && (s2[i] != '\0') && (i < size) && (s1[i] == s2[i])) {
      ++i;
   }
   if(i >= size) {  // match exceeded size, they are same up to size chars
      return(0);
   } else if ( (s1[i] == s2[i])) { // match ended before size are they same? (includeing NULL matches)
      return(0);
   } // else don't match return diff
   return( s1[i] - s2[i]);
}
   
