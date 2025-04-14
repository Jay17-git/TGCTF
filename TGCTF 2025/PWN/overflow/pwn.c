// gcc p3.c -o p3 -m32 -static -fno-stack-protector -g
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// 内嵌pop ecx; ret gadget
__attribute__((used)) 
void pop_ecx_ret() {
    __asm__("pop %ecx; ret");
}
char name[0x100];
int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    puts("could you tell me your name?");
    read(0, name, 0x100);
    char buf[200];
    printf("i heard you love gets,right?\n");
    gets(buf);
    return 0;
}