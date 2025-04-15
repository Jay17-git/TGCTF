from hashlib import sha256
from Crypto.Util.number import getPrime, inverse, bytes_to_long, isPrime
from random import randint
import socketserver
from secret import flag, dsa_p, dsa_q

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


class DSA:
    def __init__(self):
        # while True:
            # self.q = getPrime(160)
            # t = 2 * getPrime(1024 - 160) * self.q
            # if isPrime(t + 1):
            #    self.p = t + 1
            #    break
        self.p = dsa_p
        self.q = dsa_q
        self.g = pow(2, (self.p - 1) // self.q, self.p)
        self.x = randint(1, self.q - 1)
        self.y = pow(self.g, self.x, self.p)

    def sign(self, msg, k):
        h = bytes_to_long(sha256(msg).digest())
        r = pow(self.g, k, self.p) % self.q
        s = (inverse(k, self.q) * (h + self.x * r)) % self.q
        return (r, s)

    def verify(self, msg, r, s):
        if not (0 < r < self.q and 0 < s < self.q):
            return False
        h = bytes_to_long(sha256(msg).digest())
        w = inverse(s, self.q)
        u1 = (h * w) % self.q
        u2 = (r * w) % self.q
        v = ((pow(self.g, u1, self.p) * pow(self.y, u2, self.p)) % self.p) % self.q
        return v == r


class Task(socketserver.BaseRequestHandler):
    def _recvall(self):
        BUFF_SIZE = 2048
        data = b''
        while True:
            part = self.request.recv(BUFF_SIZE)
            data += part
            if len(part) < BUFF_SIZE:
                break
        return data.strip()

    def send(self, msg, newline=True):
        if newline:
            msg += b'\n'
        self.request.sendall(msg)

    def recv(self, prompt=b'[-] '):
        self.send(prompt, newline=False)
        return self._recvall()

    def handle(self):
        n = getPrime(128)
        a, b, c, d = [randint(1, n - 1) for _ in range(4)]
        seed1, seed2, seed3 = [randint(1, n - 1) for _ in range(3)]

        lcg = TripleLCG(seed1, seed2, seed3, a, b, c, d, n)
        dsa = DSA()

        self.send(b"Welcome to TGCTF Challenge!\n")
        self.send(f"p = {dsa.p}, q = {dsa.q}, g = {dsa.g}, y = {dsa.y}".encode())

        small_primes = [59093, 65371, 37337, 43759, 52859, 39541, 60457, 61469, 43711]
        used_messages = set()
        for o_v in range(3):
            self.send(b"Select challenge parts: 1, 2, 3\n")
            parts = self.recv().decode().split()

            if '1' in parts:
                self.send(b"Part 1\n")
                for i in range(12):
                    self.send(f"Message {i + 1}: ".encode())
                    msg = self.recv()
                    used_messages.add(msg)
                    k = lcg.next()
                    r, s = dsa.sign(msg, k)
                    self.send(f"r = {r}, ks = {[k % p for p in small_primes]}\n".encode())

            if '2' in parts:
                self.send(b"Part 2\n")
                for _ in range(307):
                    k = lcg.next()
                for i in range(10):
                    self.send(f"Message {i + 1}: ".encode())
                    msg = self.recv()
                    k = lcg.next() % dsa.q
                    r, s = dsa.sign(msg, k)
                    self.send(f"Signature: r = {r}, s = {s}\n".encode())
                    used_messages.add(msg)

            if '3' in parts:
                self.send(b"Part 3\n")
                self.send(b"Forged message: ")
                final_msg = self.recv()
                self.send(b"Forged r: ")
                r = int(self.recv())
                self.send(b"Forged s: ")
                s = int(self.recv())

                if final_msg in used_messages:
                    self.send(b"Message already signed!\n")
                elif dsa.verify(final_msg, r, s):
                    self.send(f"Good! Your flag: {flag}\n".encode())
                else:
                    self.send(b"Invalid signature.\n")
