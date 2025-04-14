#include <iostream>
#include <cstdint>
#include <unistd.h>
struct BSSLayout
{
    char bss_buffer[64]; 
    struct
    {
        uint64_t syscall_num; 
        uint64_t arg1;      
        uint64_t arg2;        
        uint64_t arg3;     
        char* message[9]; 
    } target_cmd;
} globals = {
    "",
    {1, 1, 0, 11, 0} 
};
char sh[] = "/bin/sh\x00";
extern "C"
{
    void vulnerable_function();
    void trigger_protection();
}

__attribute__((noreturn)) void trigger_protection()
{
    uint64_t msg_addr = reinterpret_cast<uint64_t>(globals.target_cmd.message);
    asm volatile(
        "mov %[sys_num], %%rax\n\t"
        "mov %[arg1], %%rdi\n\t"
        "mov %[msg], %%rsi\n\t"
        "mov %[len], %%rdx\n\t"
        "syscall\n\t"
        "mov $60, %%rax\n\t" // exit syscall
        "mov $1, %%rdi\n\t"  // exit code 1
        "syscall\n\t"
        :
        : [sys_num] "m"(globals.target_cmd.syscall_num),
          [arg1] "m"(globals.target_cmd.arg1),
          [msg] "r"(globals.target_cmd.message),
          [len] "m"(globals.target_cmd.arg3)
        : "rax", "rdi", "rsi", "rdx", "memory");
    __builtin_unreachable();
}

__attribute__((naked)) void vulnerable_function()
{
    asm volatile(
        "push %%rbp\n"       
        "mov %%rsp, %%rbp\n" 
        "sub $0x40, %%rsp\n" 
        "mov 0x8(%%rbp), %%rax\n"   
        "mov %%rax, 0x28(%%rbp)\n" 

        "xor %%rdi, %%rdi\n"        
        "lea -0x40(%%rbp), %%rsi\n" 
        "mov $0x50, %%rdx\n"        
        "xor %%rax, %%rax\n"     
        "syscall\n"
        "mov 0x28(%%rbp), %%rax\n" 
        "cmp 0x8(%%rbp), %%rax\n" 
        "jne trigger_protection\n"             
        "leave\n"
        "ret\n"
        :
        :
        : "rax", "rdi", "rsi", "rdx", "memory");
}
int main()
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    std::cout << "welcome! could you tell me your name?\n";
    read(0, (void *)&globals, sizeof(globals));
    std::cout << "what dou you want to say?\n";
    vulnerable_function();
    return 0;
}
