import random
from Crypto.Util.number import *

flag = "TGCTF{🙇🏮🤟_🫡🫡🫡_🚩🚩🚩}"

BASE_ENMOJIIIII = 0xf09f9880
get_random_emojiiiiii = lambda: BASE_ENMOJIIIII + random.randint(0, 63)

