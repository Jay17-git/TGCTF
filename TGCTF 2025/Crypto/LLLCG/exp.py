from gmpy2 import gcd
from sympy import factorint
from pwn import *
from sympy.ntheory.residue_ntheory import crt
from Crypto.Util.number import *
from hashlib import sha256


class TripleLCG:
    def __init__(self, seed1, seed2, seed3, a, b, c, d, n):
        self.state = [seed1, seed2, seed3]
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.n = n

    def next(self):
        new = (self.a * self.state[-3] + self.b * self.state[-2] + self.c * self.state[-1] + self.d) % self.n
        self.state.append(new)
        return new


re = remote("127.0.0.1", 10001)
re.recvuntil(b"Welcome to TGCTF Challenge!\n")
re.recvuntil(b"= ")
p = int(re.recvuntil(b",").decode()[:-1])
re.recvuntil(b"= ")
q = int(re.recvuntil(b",").decode()[:-1])
re.recvuntil(b"= ")
g = int(re.recvuntil(b",").decode()[:-1])
re.recvuntil(b"= ")
y = int(re.recvuntil(b"\n").decode()[:-1])

re.recvuntil(b"Select challenge parts: 1, 2, 3\n")
re.recvuntil(b"[-] ")
re.sendline(b"1")
k = []
primes = [59093, 65371, 37337, 43759, 52859, 39541, 60457, 61469, 43711]
lll = 12

for i in range(lll):
    re.recvuntil(b"[-] ")
    msg = b"1"
    re.sendline(msg)
    re.recvuntil(b"r = ")
    r = int(re.recvuntil(b",").decode()[:-1])
    re.recvuntil(b"ks = ")
    ks = eval(re.recvuntil(b"\n").decode()[:-1])
    k.append(crt(primes, ks)[0])

s = [0, 0, 0] + k
ss = [s[i + 1] - s[i] for i in range(6, lll)]  # 从6开始，因为前面相减的话式子中会有未知的seed123，以下同理
a0 = [s[i + 1] - s[i] for i in range(3, lll - 3)]
b0 = [s[i + 1] - s[i] for i in range(4, lll - 2)]
c0 = [s[i + 1] - s[i] for i in range(5, lll - 1)]

# 如果模数n已知，下式成立，相减的目的是消掉d，且记录a，b，c的前面的系数，方便后续运算，后面的消除a,b,c的运算同理
# assert all([ss[i] % n == (a * a0[i] + b * b0[i] + c * c0[i]) % n for i in range(len(ss))])

sss = [ss[i + 1] * c0[i] - ss[i] * c0[i + 1] for i in range(len(ss) - 1)]
aa0 = [a0[i + 1] * c0[i] - a0[i] * c0[i + 1] for i in range(len(a0) - 1)]
bb0 = [b0[i + 1] * c0[i] - b0[i] * c0[i + 1] for i in range(len(b0) - 1)]
# assert all([sss[i] % n == (a * aa0[i] + b * bb0[i]) % n for i in range(len(sss))])

ssss = [sss[i + 1] * bb0[i] - sss[i] * bb0[i + 1] for i in range(len(sss) - 1)]
aaa0 = [aa0[i + 1] * bb0[i] - aa0[i] * bb0[i + 1] for i in range(len(aa0) - 1)]
# assert all([ssss[i] % n == a * aaa0[i] % n for i in range(len(ssss))])

all_n = [gcd(ssss[i + 1] * aaa0[i] - ssss[i] * aaa0[i + 1], ssss[i + 2] * aaa0[i] - ssss[i] * aaa0[i + 2]) for i in
         range(len(ssss) - 2)]
for k in range(3, len(all_n) - 1):
    all_n.extend(
        [gcd(ssss[i + 1] * aaa0[i] - ssss[i] * aaa0[i + 1], ssss[i + k] * aaa0[i + 1] - ssss[i + 1] * aaa0[i + k]) for i
         in range(len(ssss) - k)])
nnn = gcd(all_n[0], all_n[1])
for i in range(2, len(all_n)):
    nnn = gcd(nnn, all_n[i])
# gcd求模数n，但是n可能不是素数，但也不比较小了，可以直接分解n拿最大的素数就是我们的模数了
n = max(factorint(nnn))
# 反推a，b，c，d
a = ssss[0] * inverse(aaa0[0], n) % n
b = (sss[0] - a * aa0[0]) * inverse(bb0[0], n) % n
c = (ss[0] - (a * a0[0] + b * b0[0])) * inverse(c0[0], n) % n
d = (s[6] - a * s[3] - b * s[4] - c * s[5]) % n

new_lcg = TripleLCG(k[-3], k[-2], k[-1], a, b, c, d, n)
for _ in range(307):
    k = new_lcg.next()
re.recvuntil(b"Select challenge parts: 1, 2, 3\n")
re.recvuntil(b"[-] ")
re.sendline(b"2")
rs = []
for i in range(10):
    re.recvuntil(b"[-] ")
    msg = b"2"
    re.sendline(msg)
    re.recvuntil(b"r = ")
    r = int(re.recvuntil(b",").decode()[:-1])
    re.recvuntil(b"s = ")
    s = int(re.recvuntil(b"\n").decode()[:-1])
    rs.extend([r, s])
    
h = bytes_to_long(sha256(b"2").digest())

# 计算x
x = [(rs[i + 1] * new_lcg.next() - h) * inverse(rs[i], q) % q for i in range(0, 10, 2)]

#伪造签名
h = bytes_to_long(sha256(b"3").digest())
k = new_lcg.next()
r = pow(g, k, p) % q
s = (inverse(k, q) * (h + x[0] * r)) % q

re.recvuntil(b"Select challenge parts: 1, 2, 3\n")
re.recvuntil(b"[-] ")
re.sendline(b"3")
re.recvuntil(b"Forged message: ")
re.recvuntil(b"[-] ")
re.sendline(b"3")
re.recvuntil(b"Forged r: ")
re.recvuntil(b"[-] ")
re.sendline(str(r).encode())
re.recvuntil(b"Forged s: ")
re.recvuntil(b"[-] ")
re.sendline(str(s).encode())
re.interactive()
