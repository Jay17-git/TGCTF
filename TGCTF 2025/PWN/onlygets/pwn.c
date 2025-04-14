#include <stdio.h>
int main()
{
    char buf[0x10];
    gets(buf);
    return 0;
}
__asm__("add %esi,%ebx; ret");
