#include <stdio.h>
int magic = 0x114514;
int main()
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    char buf[0x50];
    printf("Welcome TGCTF!\n");
    printf("your gift %p\n", buf);
    printf("please tell me your name\n");
    read(0, buf, 0x30);
    if(magic==0x114514)
    {
        printf(buf);
        magic = 0;
    }
    return 0;
}