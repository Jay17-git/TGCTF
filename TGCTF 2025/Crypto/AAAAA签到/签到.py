text = "TGCTF{WO0O!Y04_5R3_GOOD_AT_MOVE}"

offset = 25

for i in text:
    if not i.isalpha():
        print(i, end = '')
    else:
        print(chr((ord(i) - offset - ord('A')) % 26 + ord('A')), end = '')
    offset += 1



