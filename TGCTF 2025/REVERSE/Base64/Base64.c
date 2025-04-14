#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define LEN 41
#define ENC "AwLdOEVEhIWtajB2CbCWCbTRVsFFC8hirfiXC9gWH9HQayCJVbB8CIF="

int sub_159753(int num) {
	int numnum = num + 24;
	if (numnum > 64) {
		numnum -= 64;
	}
	return numnum;
}

char* encrypt(const char* str0)
{
	unsigned char* str = (unsigned char*)str0;
	const unsigned char base64_map[] = "GLp/+Wn7uqX8FQ2JDR1c0M6U53sjBwyxglmrCVdSThAfEOvPHaYZNzo4ktK9iebI";
	long len;
	long str_len;
	long flag;
	unsigned char* res;
	str_len = strlen((const char*)str);
	switch (str_len % 3)
	{
	case 0:
		flag = 0; len = str_len / 3 * 4;
		break;
	case 1:
		flag = 1; len = (str_len / 3 + 1) * 4;
		break;
	case 2:
		flag = 2; len = (str_len / 3 + 1) * 4;
		break;
	}
	res = (unsigned char*)malloc(sizeof(unsigned char) * len + 1);
	for (int i = 0, j = 0; j < str_len - flag; j += 3, i += 4)
	{
		res[i] = base64_map[sub_159753(str[j] >> 2)];
		res[i + 1] = base64_map[sub_159753((str[j] & 0x3) << 4 | str[j + 1] >> 4)];
		res[i + 2] = base64_map[sub_159753((str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6))];
		res[i + 3] = base64_map[sub_159753(str[j + 2] & 0x3f)];
	}
	switch (flag)
	{
	case 0:
		break;
	case 1:
		res[len - 4] = base64_map[sub_159753(str[str_len - 1] >> 2)];
		res[len - 3] = base64_map[sub_159753((str[str_len - 1] & 0x3) << 4)];
		res[len - 2] = res[len - 1] = '='; break;
	case 2:
		res[len - 4] = base64_map[sub_159753(str[str_len - 2] >> 2)];
		res[len - 3] = base64_map[sub_159753((str[str_len - 2] & 0x3) << 4 | str[str_len - 1] >> 4)];
		res[len - 2] = base64_map[sub_159753((str[str_len - 1] & 0xf) << 2)];
		res[len - 1] = '=';
		break;
	}
	res[len] = '\0';
	return res;
}

int cmp(char* enc) {
	if (!strcmp(enc, ENC)) {
		return 1;
	}
	return 0;
}

int main() {
	printf("Welcome to HZNUCTF!!!\n");
	printf("Plz input your flag:\n");
	char* Buf = (char*)malloc((LEN + 1) * sizeof(char));
	scanf_s("%s", Buf, LEN + 1);
	if (strlen(Buf) == LEN) {
		char head[9];
		strncpy_s(head, sizeof(head), Buf + 0, 8);
		head[8] = '\0';
		if (!strcmp(head, "HZNUCTF{")) {
			char* encode = encrypt(Buf);
			if (cmp(encode)) {
				printf("Congratulation!!!");
				free(encode);
				exit(1);
			}
			else {
				printf("wrong_wrong!!!");
				free(encode);
				exit(1);
			}
		}
		else {
			printf("wrong head!!!");
			free(Buf);
			exit(1);
		}
	}
	else {
		printf("wrong len!!!");
	}

	free(Buf);
	return 0;
}
