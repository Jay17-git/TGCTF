#include <stdint.h>

// 字符串数据
const char welcome_msg[] = "Welcome to THE Job Offer Portal..."; // 完整字符串
const char menu_msg[] = "You have a pending job offer..."; // 完整字符串
const char accept_msg[] = "Your job offer have been accepted..."; // 完整字符串
const char feedback_prompt[] = "Submit your feedback: ";
const char feedback_thanks[] = "Thank you for your feedback!";
const char exit_msg[] = "Have a nice day!";
const char invalid_msg[] = "Invalid choice.";

// 保留.text段开头的所有汇编指令
__attribute__((section(".text"))) 
void text_start() {
    asm volatile (
        "xchg %%rax, %%rdi\n"
        "jmp *1(%%rax)\n"
        "mov %%rdi, %%rcx\n"
        "jmp *(%%rcx)\n"
        "xor %%rax, %%rax\n"
        "jmp *(%%rdx)\n"
        "pop %%rsp\n"
        "pop %%rdi\n"
        "pop %%rcx\n"
        "pop %%rdx\n"
        "jmp *1(%%rdi)\n"
        "pop %%rcx\n"
        "or %%bh, %%bh\n"
        "jmp *(%%rdx)\n"
        "mov 0x10(%%rcx), %%rsi\n"
        "jmp *(%%rdx)\n"
        "pop %%rdx\n"
        "jmp *(%%rcx)\n"
        "add %%rdx, %%rax\n"
        "jmp *(%%rcx)\n"
        "pop %%rcx\n"
        "jmp *(%%rdx)\n"
        ::: "rax", "rdi", "rcx", "rdx", "rsi", "bh"
    );
}

int main() {
    // 精确模拟原始栈布局
    char large_buf[0x108]; // 距离rbp 0x108
    char small_buf[0x10];  // 距离rbp 0x10

    
    // 初始输出
    asm volatile (
        "mov %0, %%rsi\n"
        "mov $0x83, %%edx\n"
        "xor %%rax, %%rax\n"
        "xor %%rdi, %%rdi\n"
        "inc %%rax\n"
        "inc %%rdi\n"
        "syscall\n"
        ::"r"(welcome_msg): "rax", "rdi", "rsi", "rdx"
    );

    while(1) {
        // 显示菜单
        asm volatile (
            "mov %0, %%rsi\n"
            "mov $0x65, %%edx\n"
            "xor %%rax, %%rax\n"
            "xor %%rdi, %%rdi\n"
            "inc %%rax\n"
            "inc %%rdi\n"
            "syscall\n"
            ::"r"(menu_msg): "rax", "rdi", "rsi", "rdx"
        );

        // 读取小缓冲区(0x10)
        asm volatile (
            "mov %0, %%rsi\n"
            "mov $0x10, %%edx\n"
            "xor %%rax, %%rax\n"
            "xor %%rdi, %%rdi\n"
            "syscall\n"
            ::"r"(small_buf): "rax", "rdi", "rsi", "rdx"
        );

        // 处理选项
        if(small_buf[0] == '1') {
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x5F, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::"r"(accept_msg): "rax", "rdi", "rsi", "rdx"
            );
        } 
        else if(small_buf[0] == '2') {
            // 读取大缓冲区(0x168)
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x16, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::"r"(feedback_prompt): "rax", "rdi", "rsi", "rdx"
            );
            
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x168, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "syscall\n"
                ::"r"(large_buf): "rax", "rdi", "rsi", "rdx"
            );
            
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x1D, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::"r"(feedback_thanks): "rax", "rdi", "rsi", "rdx"
            );
        }
        else if(small_buf[0] == '3') {
            // 退出
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x11, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::"r"(exit_msg): "rax", "rdi", "rsi", "rdx"
            );
            asm volatile (
                "mov $60, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "syscall\n"
                ::: "rax", "rdi"
            );
        }
        else if(small_buf[0] == '4') {
            // 隐藏选项
            asm volatile (
                "push %%rsp\n"
                "push %%rsp\n"
                "push %%rsp\n"
                "pop %%rsi\n"
                "pop %%rsp\n"
                "pop %%rsp\n"
                "mov $8, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::: "rax", "rdi", "rsi", "rdx"
            );
        }
        else {
            // 无效输入
            asm volatile (
                "mov %0, %%rsi\n"
                "mov $0x10, %%edx\n"
                "xor %%rax, %%rax\n"
                "xor %%rdi, %%rdi\n"
                "inc %%rax\n"
                "inc %%rdi\n"
                "syscall\n"
                ::"r"(invalid_msg): "rax", "rdi", "rsi", "rdx"
            );
        }
    }
    return 0;
}