#include <stdio.h>
#include <stdlib.h>
char *gets(char *s);
__asm__(
    "pop %rdi;"
    "ret;"
);
int main() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    char name[100];
    puts("As a student who has been learning pwn for half a year\n"
         "basic ROP is an essential skill that everyone should master. \n"
         "Therefore, hurry up and complete the check-in. \n"
         "Welcome to the Hangzhou Normal University CTF competition, please leave your name.");
    gets(&name);
    return 0;
}
