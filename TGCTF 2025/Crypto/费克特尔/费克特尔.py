# -*-coding:utf-8-*-
"""
author: MIAO25
date: 2025/4/15
about: 费克特尔

"""
from math import prod

from Crypto.Util.number import *
from flag import FLAG
n=7*2**256+1
e=65537
m=bytes_to_long(FLAG)
c=pow(m,e,n)
print(c)
print(n)
print(m)