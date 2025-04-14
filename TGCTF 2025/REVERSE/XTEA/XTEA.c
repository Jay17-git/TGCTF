#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <winternl.h>
#define LEN 32
uint32_t enc[LEN] = {
   0x8ccb2324, 0x09a7741a, 0xfb3c678d, 0xf6083a79,
   0xf1cc241b, 0x39fa59f2, 0xf2abe1cc, 0x17189f72
};

int IsDebugged() {
   return IsDebuggerPresent();
}

void sub_105897() {
   if (IsDebugged()) {
       srand(2025);
   }
}

void initkey(int* key) {
   for (int i = 0; i < 4; i++) {
       key[i] = rand();
   }
}

void encryption(uint32_t n, uint32_t* v1, uint32_t* v2, const unsigned int key[4]) {
   uint32_t b = *v1, c = *v2, s = 0;
   for (int i = 0; i < 32; i++) {
       b += (((c << 4) ^ (c >> 5)) + c) ^ (s + key[s & 3]);
       s -= n;
       c += (((b << 4) ^ (b >> 5)) + b) ^ (s + key[(s >> 11) & 3]);
   }
   *v1 = b;
   *v2 = c;
}

int main() {
   srand(2024);
   sub_105897();

   printf("Welcome to HZNUCTF!!!\n");
   printf("Plz input the cipher:\n");                  //����delta,2654435769
   unsigned int cipher = 0;
   if (scanf_s("%d", &cipher) != 1) {
       printf("Invalid input.\n");
       return 1;
   }

   printf("Plz input the flag:\n");                    //����flag
   char* Buf = (char*)malloc((LEN + 1) * sizeof(char));
   int* k = (int*)malloc(4 * sizeof(int));

   if (scanf_s("%s", Buf, LEN + 1) != 1) {
       printf("Invalid input.\n");
       free(Buf);
       free(k);
       return 1;
   }

   uint32_t sec[8];
   for (int i = 0; i < 32; i += 4) {
       sec[i / 4] = ((uint32_t)Buf[i] << 24) | 
                    ((uint32_t)Buf[i + 1] << 16) | 
                    ((uint32_t)Buf[i + 2] << 8) | 
                    (uint32_t)Buf[i + 3];
   }

   initkey(k);

   for (int i = 0; i < 7; i++) {
       encryption(cipher, &sec[i], &sec[i + 1], (unsigned int*)k);
   }

   for (int j = 0; j < 8; j++) {
       if (sec[j] != enc[j]) {
           printf("wrong_wrong!!!");
           exit(1);
       }
   }
   printf("Congratulation!!!");

   free(Buf);
   free(k);
   return 0;
}
//HZNUCTF{ae6-9f57-4b74-b423-98eb}
