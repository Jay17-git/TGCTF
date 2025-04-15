

cipherplaint = "UGBRC{RI0G!O04_5C3_OVUI_DV_MNTB}"

# 只移动字母
flag0 = ""
flag1 = ""
offset0 = 1
offset1 = 1
for c in cipherplaint:
    if c.isalpha():
        flag0 += chr((ord(c)-ord("A")-offset0)%26+ord("A"))
        flag1 += chr((ord(c)-ord("A")-offset1)%26+ord("A"))
        offset1 -= 1
    else:
        flag0 += c
        flag1 += c
    offset0 -= 1


# 移动字母、数字
flag2 = ""
flag3 = ""
offset2 = 1
offset3 = 1
for c in cipherplaint:
    if c.isalpha():
        flag2 += chr((ord(c)-ord("A")-offset2)%26+ord("A"))
        flag3 += chr((ord(c)-ord("A")-offset3)%26+ord("A"))
        offset3 -= 1
    elif c.isdigit():
        flag2 += chr((ord(c)-ord("0")-offset2)%10+ord("0"))
        flag3 += chr((ord(c)-ord("0")-offset3)%10+ord("0"))
        offset3 -= 1
    else:
        flag2 += c
        flag3 += c
    offset2 -= 1

print(f"{flag0 = }")
print(f"{flag1 = }")
print(f"{flag2 = }")
print(f"{flag3 = }")


# flag0 = 'TGCTF{WO0O!Y04_5R3_GOOD_AT_MOVE}'
# flag1 = 'TGCTF{VN0M!V04_5K3_XFFU_QJ_BDKT}'
# flag2 = 'TGCTF{WO7O!Y16_9R9_GOOD_AT_MOVE}'
# flag3 = 'TGCTF{VN6N!W94_6O6_CKKZ_VO_GIPY}'