#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <linux/filter.h>
#include <sys/syscall.h>
#include <linux/audit.h>
#include <errno.h>
#ifndef __NR_openat2
#define __NR_openat2 437
#endif
static void sandbox()
{
    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, arch)),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 8),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, nr)),
        BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 0x40000000, 6, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_open, 5, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_openat, 4, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_execve, 3, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_execveat, 2, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_openat2, 1, 0),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_TRACE)};
    struct sock_fprog prog = {
        .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };
    prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
    prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog);
}
char *chunk = NULL;
void __attribute__((constructor)) nightmare()
{
    sandbox();
    if (!chunk)
    {
        size_t len = 0;
        read(0, &len, 8);
        chunk = malloc(len);
    }
    uint8_t byte = 0;
    size_t offset = 0;
    read(0, &offset, 8);
    read(0, &byte, 1);
    chunk[offset] = byte;
    write(1, "Welcome TGCTF 2025!\n", 20);
    _Exit(0);
}
int main()
{
    _Exit(0);
}