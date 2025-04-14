#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<time.h>

void sub_5s5s5s() {
	int seed = time(NULL);
	srand(seed);
}

void init(char *s) {
	sub_5s5s5s();
	strcpy(s, "JustDoIt!");
}

int init_sbox(unsigned char *s, unsigned char *key) {
	int i = 0, j = 0;
	char k[300] = { 0 };
	unsigned char tmp = 0;
	int rand_num = rand();						//���ɵ������
	
	for (i = 0; i <= 256; i++) {
		s[i] = i;
		k[i] = key[i % 9] ^ rand_num;			//ħ�ĵĵط���������sbox��ʱ��key�������һ����
	}
	for (i = 0; i <= 256; i++) {
		j = (j + s[i] + k[i]) % 257;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
	return 0;
}

int rc4(unsigned char* Data, unsigned long Len_D, unsigned char* key, unsigned long Len_k)
{

	unsigned char sbox[300];
	init_sbox(sbox, key);
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;

	for (k = 0; k < Len_D; k++) {
		i = (i + 1) % 257;
		j = (j + sbox[i]) % 257;
		sbox[i] = sbox[j];						//ħ�ĵĵط����ѽ���ȥ����
		sbox[j] = sbox[i];
		t = (sbox[i] + sbox[j]) % 257;
		Data[k] = Data[k] ^ sbox[t];
	}
	return 0;
}


int main() {
	char flag[42];
	unsigned char sentence[10];

	init(sentence);								//��ʼ��α���������
	printf("Welcome to HZNUCTF!!!\n");
	printf("%s", sentence);
	printf("Plz input the flag:\n");
	scanf("%s", flag);							//����flag

	rc4(flag, sizeof(flag), sentence, 9);
	for (int i = 0; i < sizeof(flag); i++) {
		printf("%x ", flag[i] & 0xff);
	}



	return 0;
}
//HZNUCTF{489b88-1305-411e-b1f4-88a3070a73}