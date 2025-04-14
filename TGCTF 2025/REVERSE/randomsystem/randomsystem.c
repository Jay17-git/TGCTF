#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

int seed = 0;
int matrix1[8][8] = { 0 };
unsigned char S[256] = {
		0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
		0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
		0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
		0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
		0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
		0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
		0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
		0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
		0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
		0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
		0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
		0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
		0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
		0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
		0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
		0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
unsigned char base64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#ifdef _M_IX86
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")
#else
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback")
#endif



// �궨�壺���ַ�ת��Ϊʮ����������
#define M(c)((c) >> 4 & 15)[x]
// �궨�壺��ȡ��4λ
#define N(n)(n) & 15
// �궨�壺����4λ
#define O(o)(o) << 4
// �궨�壺��4λ��������ת��Ϊʮ�������ַ�
#define P(p)(p) | 48 | 7 * ((p) > 9) // 48��'0'��ASCIIֵ��7��'A'-'9'-1

// ȫ�ֱ�����ʮ�������ַ���
char* x = "0123456789ABCDEF";

// ��������64λ�������ַ���ת��Ϊ����
void f1(const char* s, uint64_t* r) {
	*r = 0; // ��ʼ�����Ϊ0
	for (int i = 0; i < 64; i++) {
		*r |= (s[i] == 49 ? 1ULL : 0) << (63 - i); // ��λ�����������ַ���
	}
}

// ��������64λ����ת��Ϊʮ�������ַ���
void g(uint64_t v, char* h) {
	for (int i = 15; i >= 0; i--) { // �����λ��ʼ����
		h[i] = P(N(v)); // ����4λת��Ϊʮ�������ַ�
		v >>= 4; // ����4λ��������һ��4λ
	}
}

//��������
void swap(char* x, int num[]) {
	__asm {
		jz lable1
		jnz lable1
		_EMIT 0x89
		lable1 :
	}
	size_t len = strlen(x);
	for (int i = 0; i < len / 2; i++) {
		if (num[i] >= 0 && num[i] < len) {
			char temp = x[i];
			x[i] = x[len - num[i] - 1];
			x[len - num[i] - 1] = temp;
		}
	}
}

void init_matrix1(int arr1[8][8], char y[]) {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			arr1[i][j] = y[count++] - '0';
		}
	}
}

void init_matrix2(int arr2[8][8], char z[]) {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			__asm {
				call label1
				_emit 0xe8
				label1 :
				add dword ptr ss : [esp] , 8//�������Ӷ��ٸ��ݵ�����
					ret
					_emit 0xe8
			}
			arr2[i][j] = z[count++];
		}
	}
}

void mm(int* a, int* b, int* r) {
	__asm {
		jz lable1
		jnz lable1
		_EMIT 0x89
		lable1 :
	}


	_asm {
		jz Label3;
		jnz Label3;
		_emit 0xE8;
	}
Label2:
	_asm {
		jz Label4;
		jnz Label4;
		_emit 0xE8;
	}


Label3:
	_asm {
		jz Label1;
		jnz Label1;
		_emit 0xE9;
	}
Label1:
	_asm {
		jz Label2;
		jnz Label2;
		_emit 0xE9;
	}
Label4:
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			*(r + i * 8 + j) = 0;
			for (int k = 0; k < 8; k++) {
				*(r + i * 8 + j) += *(a + i * 8 + k) * *(b + k * 8 + j);
			}
		}
	}
}

int check(int matrix[8][8], int* encc) {
	int cont = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (matrix[i][j] != encc[cont++]) {
				printf("wrong_wrong!!!\n");
				return 0;
			}
		}
	}
	return 1;
}

void init_key(const char s[], char key[]) {
	int len = strlen(s);
	__asm {
		jz lable1
		jnz lable1
		_EMIT 0x89
		lable1 :
	}


	_asm {
		jz Label3;
		jnz Label3;
		_emit 0xE8;
	}
Label2:
	_asm {
		jz Label4;
		jnz Label4;
		_emit 0xE8;
	}


Label3:
	_asm {
		jz Label1;
		jnz Label1;
		_emit 0xE9;
	}
Label1:
	_asm {
		jz Label2;
		jnz Label2;
		_emit 0xE9;
	}
Label4:
	for (int i = 0; i < len - 1; i += 2) {
		key[i / 2] = ((s[i] - '0') << 4) | (s[i + 1] - '0');
	}
	key[8] = '\0'; // �����ַ���������
}

void NTAPI TLS_CALLBACK(PVOID DllHandle, DWORD dwReason, PVOID Reserved) {
	__asm {
		call label1
		_emit 0xe8
		label1 :
		add dword ptr ss : [esp] , 8//�������Ӷ��ٸ��ݵ�����
			ret
			_emit 0xe8
	}
	_asm {
		call lable4
		__emit 0xc8
		lable4:
		pop eax
	}
	seed = 2025;
	int enc[] = { 1, 1, 0, 1, 0, 0, 1, 0,
				 0, 1, 1, 0, 0, 1, 0, 1,
				 0, 0, 1, 1, 0, 1, 1, 0,
				 0, 0, 0, 1, 0, 1, 0, 1,
				 0, 1, 0, 0, 1, 0, 1, 0,
				 0, 0, 0, 0, 0, 1, 0, 1,
				 0, 0, 0, 0, 0, 0, 1, 1,
				 0, 1, 1, 0, 0, 0, 0, 1 };
	int c_enc = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			matrix1[i][j] = enc[c_enc++];
		}
	}
	
}

EXTERN_C
#ifdef _M_X64
#pragma const_seg (".CRT$XLB")
PIMAGE_TLS_CALLBACK _tls_callback = TLS_CALLBACK;
#else
#pragma data_seg (".CRT$XLB")
PIMAGE_TLS_CALLBACK _tls_callback = TLS_CALLBACK;
#endif


int main() {
	char binaryString[65]; // 64λ�������ַ��� + 1�����ַ�
	uint64_t binaryData;
	char hexString[17] = { 0 }; // 16��ʮ�������ַ� + 1�����ַ�
	char flag[74];
	char head[9];
	char content[65];
	int matrix2[8][8] = { 0 };
	int matrix3[8][8] = { 0 };
	int enc[64] = { 0 };
	char key[9] = "KeYkEy!!";
	enc[0] = 376;
	enc[1] = 356;
	enc[2] = 169;
	enc[3] = 501;
	enc[4] = 277;
	enc[5] = 329;
	enc[6] = 139;
	enc[7] = 342;
	enc[8] = 380;
	enc[9] = 365;
	enc[10] = 162;
	enc[11] = 258;
	enc[12] = 381;
	enc[13] = 339;
	enc[14] = 347;
	enc[15] = 307;
	enc[16] = 263;
	enc[17] = 359;
	enc[18] = 162;
	enc[19] = 484;
	enc[20] = 310;
	enc[21] = 333;
	enc[22] = 346;
	enc[23] = 339;
	enc[24] = 150;
	enc[25] = 194;
	enc[26] = 175;
	enc[27] = 344;
	enc[28] = 158;
	enc[29] = 250;
	enc[30] = 128;
	enc[31] = 175;
	enc[32] = 158;
	enc[33] = 173;
	enc[34] = 152;
	enc[35] = 379;
	enc[36] = 158;
	enc[37] = 292;
	enc[38] = 130;
	enc[39] = 365;
	enc[40] = 197;
	enc[41] = 20;
	enc[42] = 197;
	enc[43] = 161;
	enc[44] = 198;
	enc[45] = 10;
	enc[46] = 207;
	enc[47] = 244;
	enc[48] = 202;
	enc[49] = 14;
	enc[50] = 204;
	enc[51] = 176;
	enc[52] = 193;
	enc[53] = 255;
	enc[54] = 35;
	enc[55] = 7;
	enc[56] = 158;
	enc[57] = 181;
	enc[58] = 145;
	enc[59] = 353;
	enc[60] = 153;
	enc[61] = 357;
	enc[62] = 246;
	enc[63] = 151;

	printf("Welcome to HZNUCTF!!!\n");
	printf("Enter something: \n");
	scanf("%64s", binaryString);

	f1(binaryString, &binaryData); // ���ú���f1�����������ַ���ת��Ϊ����
	g(binaryData, hexString);		// ���ú���g��������ת��Ϊʮ�������ַ���

	//���key�Ƿ���ȷ��ʮ��������ʽ
	if (hexString[0] != '5' || hexString[1] != '2' || hexString[2] != '6' || hexString[3] != '5') {
		printf("wrong_wrong!!!\n");
		return 0;
	}
	if (hexString[4] != '5' || hexString[5] != '6' || hexString[6] != '6' || hexString[7] != '5') {
		printf("wrong_wrong!!!\n");
		return 0;
	}
	if (hexString[8] != '5' || hexString[9] != '2' || hexString[10] != '6' || hexString[11] != '5') {
		printf("wrong_wrong!!!\n");
		return 0;
	}
	if (hexString[12] != '5' || hexString[13] != '3' || hexString[14] != '6' || hexString[15] != '5') {
		printf("wrong_wrong!!!\n");
		return 0;
	}
	printf("good_job!!!\n");
	printf("So, Plz input the flag:\n");
	scanf("%73s", flag);
	strncpy_s(content, 65, flag + 8, 64);
				
	srand(seed);					//����32�������
	int rand_num[32] = { 0 };
	for (int i = 0; i < 32; i++) {
		int num;
		bool is_unique;
		do {
			num = rand() % 32;
			is_unique = true;
			for (int j = 0; j < i; j++) {
				if (rand_num[j] == num) {
					is_unique = false;
					break;
				}
			}
		} while (!is_unique); // ����ظ����������������
		rand_num[i] = num; // �����ظ����������������
	}

	/*init_matrix1(matrix1, binaryString);*/
	//printf("matrix1: \n");
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		printf("%d ", matrix1[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");

	swap(content, rand_num);
	init_matrix2(matrix2, content);
	init_key(hexString, key);

	//printf("matrix2: \n");
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		printf("%d ", matrix2[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");

	mm((int*)matrix1, (int*)matrix2, (int*)matrix3);    //����˷�����

	int ccc = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			matrix3[i][j] ^= key[ccc++ % strlen(key)];
		}
	}

	/*printf("matrix3: \n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("enc[%d] = %d\n", matrix3[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	if (check(matrix3, enc) == 1) {
		printf("Congratulation!!!\n");
	}
	return 0;
}

//HZNUCTF{3zfb899ac5c256d-7a8r59f0tccd-4fa6b8vfd111-a44ffy4r0-6dce5679da58}