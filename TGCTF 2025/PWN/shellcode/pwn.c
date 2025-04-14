#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <linux/filter.h>
#include <seccomp.h>
#include <errno.h>
void execute(void *shellcode) {
    asm volatile (
        "xor %%rax, %%rax\n"
        "xor %%rbx, %%rbx\n"
        "xor %%rcx, %%rcx\n"
        "xor %%rdx, %%rdx\n"
        "xor %%rsi, %%rsi\n"
        "xor %%r8, %%r8\n"
        "xor %%r9, %%r9\n"
        "xor %%r10, %%r10\n"
        "xor %%r11, %%r11\n"
        "xor %%r12, %%r12\n"
        "xor %%r13, %%r13\n"
        "xor %%r14, %%r14\n"
        "xor %%r15, %%r15\n"
        "xor %%rbp, %%rbp\n"
        "xor %%rsp, %%rsp\n"
        "mov %0, %%rdi\n"
        "jmp *%0\n"
        :
        : "r"(shellcode)
        : "rax", "rbx", "rcx", "rdx", "rsi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
    );
}
int main() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    puts("hello hacker");
    puts("try to show your strength ");
    char *buf;
    buf = mmap(0, 0x1000uLL, 7, 34, -1, 0LL);
    read(0, buf, 18);
    mprotect(buf, 0x1000uLL, 4);
    execute(buf);
    return 0;
}
