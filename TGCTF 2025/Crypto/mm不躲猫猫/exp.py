
from math import gcd

from Crypto.Util.number import inverse, long_to_bytes

# exp
n_list=[]
c_list=[]
with open('challenge.txt', 'r',encoding='utf-8') as file:
    lines = file.readlines()
    for i in range(3, len(lines), 4):
        # 提取 n 和 c
        n = int(lines[i].split('=')[1].strip())
        c = int(lines[i + 1].split('=')[1].strip())
        n_list.append(n)
        c_list.append(c)

for i in range(len(n_list)):
    for j in range(len(n_list)):
        if (i != j):
            if (gcd(n_list[i], n_list[j]) != 1):
                print(i, j)
                print("p =", gcd(n_list[i], n_list[j]))
                p=gcd(n_list[i], n_list[j])
                n=n_list[i]
                c=c_list[i]
                break

# assert n%p==0
# print(p)
q=n//p
e=65537
phi = (p-1)*(q-1)

e = 65537
d = inverse(e,phi)
m = pow(c,d,n)
print(long_to_bytes(m))



