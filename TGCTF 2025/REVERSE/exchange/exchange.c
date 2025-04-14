#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>
#include <Windows.h>
#define LEN 41
int BBBBB[64] = {0x84, 0x8b, 0x03, 0x22, 0x14, 0xbe, 0xdf, 0x75, 
                0xb3, 0xd5, 0x76, 0x6f, 0xcd, 0x2a, 0x5d, 0xd7, 
                0x4d, 0xb2, 0x5f, 0x06, 0x98, 0x9d, 0x3e, 0xa8, 
                0xf7, 0x23, 0xf2, 0x8b, 0xf2, 0x54, 0x65, 0x7a, 
                0x20, 0xc0, 0x87, 0x55, 0xd6, 0x3b, 0x46, 0x3d, 
                0xf7, 0xb2, 0x7a, 0x9d, 0xc2, 0xcf, 0x1a, 0xae, 
                0x16, 0xc7, 0x15, 0x30, 0x8e, 0xfd, 0x8f, 0x9e, 
                0xaa, 0x39, 0xab, 0xfe, 0x95, 0xa7, 0x1f, 0xf1 };


#define EN0 0   /* MODE == encrypt */
#define DE1 1   /* MODE == decrypt */

typedef struct
{
   unsigned long ek[32];
   unsigned long dk[32];
} des_ctx;

static unsigned long SP1[64] = {
   0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
   0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
   0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
   0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
   0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
   0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
   0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
   0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
   0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
   0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
   0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
   0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
   0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
   0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
   0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
   0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

static unsigned long SP2[64] = {
   0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
   0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
   0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
   0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
   0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
   0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
   0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
   0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
   0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
   0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
   0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
   0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
   0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
   0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
   0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
   0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

static unsigned long SP3[64] = {
   0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
   0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
   0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
   0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
   0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
   0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
   0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
   0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
   0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
   0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
   0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
   0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
   0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
   0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
   0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
   0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

static unsigned long SP4[64] = {
   0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
   0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
   0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
   0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
   0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
   0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
   0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
   0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
   0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
   0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
   0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
   0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
   0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
   0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
   0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
   0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

static unsigned long SP5[64] = {
   0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
   0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
   0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
   0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
   0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
   0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
   0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
   0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
   0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
   0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
   0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
   0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
   0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
   0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
   0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
   0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

static unsigned long SP6[64] = {
   0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
   0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
   0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
   0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
   0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
   0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
   0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
   0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
   0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
   0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
   0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
   0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
   0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
   0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
   0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
   0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

static unsigned long SP7[64] = {
   0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
   0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
   0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
   0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
   0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
   0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
   0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
   0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
   0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
   0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
   0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
   0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
   0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
   0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
   0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
   0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

static unsigned long SP8[64] = {
   0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
   0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
   0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
   0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
   0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
   0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
   0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
   0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
   0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
   0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
   0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
   0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
   0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
   0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
   0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
   0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

#define   CRCSEED 0x7529

static void scrunch(unsigned char*, unsigned long*);
static void unscrun(unsigned long*, unsigned char*);
static void desfunc(unsigned long*, unsigned long*);
static void cookey(unsigned long*);
void usekey(register unsigned long* from);

static unsigned long KnL[32] = { 0L };
//static unsigned long KnR[32] = { 0L };
//static unsigned long Kn3[32] = { 0L };
//static unsigned char Df_Key[24] = {
//  0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
//  0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
//  0x89,0xab,0xcd,0xef,0x01,0x23,0x45,0x67 };

static unsigned short bytebit[8] = {
   0200, 0100, 040, 020, 010, 04, 02, 01 };

static unsigned long bigbyte[24] = {
   0x800000L,  0x400000L,  0x200000L,  0x100000L,
   0x80000L,   0x40000L,   0x20000L,   0x10000L,
   0x8000L,    0x4000L,    0x2000L,    0x1000L,
   0x800L,     0x400L,     0x200L,     0x100L,
   0x80L,      0x40L,      0x20L,      0x10L,
   0x8L,       0x4L,       0x2L,       0x1L };

/* Use the key schedule specified in the Standard (ANSI X3.92-1981). */

static unsigned char pc1[56] = {
   56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
    9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
   62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
   13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3 };

static unsigned char totrot[16] = {                                                 //ħ�ĵĵط�,��׼����1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28
   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

static unsigned char pc2[48] = {
   13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
   22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
   40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
   43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };

void deskey(unsigned char* key, short edf)   /* Thanks to James Gillogly & Phil Karn! */
{
   register int i, j, l, m, n;
   unsigned char pc1m[56], pcr[56];
   unsigned long kn[32];

   for (j = 0; j < 56; j++)
   {
       l = pc1[j];
       m = l & 07;
       pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
   }
   for (i = 0; i < 16; i++)
   {
       if (edf == DE1) m = (15 - i) << 1;
       else m = i << 1;
       n = m + 1;
       kn[m] = kn[n] = 0L;
       for (j = 0; j < 28; j++)
       {
           l = j + totrot[i];
           if (l < 28) pcr[j] = pc1m[l];
           else pcr[j] = pc1m[l - 28];
       }
       for (j = 28; j < 56; j++)
       {
           l = j + totrot[i];
           if (l < 56) pcr[j] = pc1m[l];
           else pcr[j] = pc1m[l - 28];
       }
       for (j = 0; j < 24; j++)
       {
           if (pcr[pc2[j]]) kn[m] |= bigbyte[j];
           if (pcr[pc2[j + 24]]) kn[n] |= bigbyte[j];
       }
   }
   cookey(kn);
   return;
}

static void cookey(register unsigned long* raw1)
{
   register unsigned long* cook, * raw0;
   unsigned long dough[32];
   register int i;

   cook = dough;
   for (i = 0; i < 16; i++, raw1++)
   {
       raw0 = raw1++;
       *cook = (*raw0 & 0x00fc0000L) << 6;
       *cook |= (*raw0 & 0x00000fc0L) << 10;
       *cook |= (*raw1 & 0x00fc0000L) >> 10;
       *cook++ |= (*raw1 & 0x00000fc0L) >> 6;
       *cook = (*raw0 & 0x0003f000L) << 12;
       *cook |= (*raw0 & 0x0000003fL) << 16;
       *cook |= (*raw1 & 0x0003f000L) >> 4;
       *cook++ |= (*raw1 & 0x0000003fL);
   }
   usekey(dough);
   return;
}

void cpkey(register unsigned long* into)
{
   register unsigned long* from, * endp;

   from = KnL, endp = &KnL[32];
   while (from < endp) *into++ = *from++;
   return;
}

void usekey(register unsigned long* from)
{
   register unsigned long* to, * endp;

   to = KnL, endp = &KnL[32];
   while (to < endp) *to++ = *from++;
   return;
}

void des(unsigned char* inblock, unsigned char* outblock)
{
   unsigned long work[2];

   scrunch(inblock, work);
   desfunc(work, KnL);
   unscrun(work, outblock);
   return;
}

static void scrunch(register unsigned char* outof, register unsigned long* into)
{
   *into = (*outof++ & 0xffL) << 24;
   *into |= (*outof++ & 0xffL) << 16;
   *into |= (*outof++ & 0xffL) << 8;
   *into++ |= (*outof++ & 0xffL);
   *into = (*outof++ & 0xffL) << 24;
   *into |= (*outof++ & 0xffL) << 16;
   *into |= (*outof++ & 0xffL) << 8;
   *into |= (*outof & 0xffL);
   return;
}

static void unscrun(register unsigned long* outof, register unsigned char* into)
{
   *into++ = (unsigned char)((*outof >> 24) & 0xffL);
   *into++ = (unsigned char)((*outof >> 16) & 0xffL);
   *into++ = (unsigned char)((*outof >> 8) & 0xffL);
   *into++ = (unsigned char)(*outof++ & 0xffL);
   *into++ = (unsigned char)((*outof >> 24) & 0xffL);
   *into++ = (unsigned char)((*outof >> 16) & 0xffL);
   *into++ = (unsigned char)((*outof >> 8) & 0xffL);
   *into = (unsigned char)(*outof & 0xffL);
   return;
}

static void desfunc(register unsigned long* block, register unsigned long* keys)
{
   register unsigned long fval, work, right, leftt;
   register int round;

   leftt = block[0];
   right = block[1];
   work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
   right ^= work;
   leftt ^= (work << 4);
   work = ((leftt >> 16) ^ right) & 0x0000ffffL;
   right ^= work;
   leftt ^= (work << 16);
   work = ((right >> 2) ^ leftt) & 0x33333333L;
   leftt ^= work;
   right ^= (work << 2);
   work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
   leftt ^= work;
   right ^= (work << 8);
   right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
   work = (leftt ^ right) & 0xaaaaaaaaL;
   leftt ^= work;
   right ^= work;
   leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

   for (round = 0; round < 8; round++)
   {
       work = (right << 28) | (right >> 4);
       work ^= *keys++;
       fval = SP7[work & 0x3fL];
       fval |= SP5[(work >> 8) & 0x3fL];
       fval |= SP3[(work >> 16) & 0x3fL];
       fval |= SP1[(work >> 24) & 0x3fL];
       work = right ^ *keys++;
       fval |= SP8[work & 0x3fL];
       fval |= SP6[(work >> 8) & 0x3fL];
       fval |= SP4[(work >> 16) & 0x3fL];
       fval |= SP2[(work >> 24) & 0x3fL];
       leftt ^= fval;
       work = (leftt << 28) | (leftt >> 4);
       work ^= *keys++;
       fval = SP7[work & 0x3fL];
       fval |= SP5[(work >> 8) & 0x3fL];
       fval |= SP3[(work >> 16) & 0x3fL];
       fval |= SP1[(work >> 24) & 0x3fL];
       work = leftt ^ *keys++;
       fval |= SP8[work & 0x3fL];
       fval |= SP6[(work >> 8) & 0x3fL];
       fval |= SP4[(work >> 16) & 0x3fL];
       fval |= SP2[(work >> 24) & 0x3fL];
       right ^= fval;
   }

   right = (right << 31) | (right >> 1);
   work = (leftt ^ right) & 0xaaaaaaaaL;
   leftt ^= work;
   right ^= work;
   leftt = (leftt << 31) | (leftt >> 1);
   work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
   right ^= work;
   leftt ^= (work << 8);
   work = ((leftt >> 2) ^ right) & 0x33333333L;
   right ^= work;
   leftt ^= (work << 2);
   work = ((right >> 16) ^ leftt) & 0x0000ffffL;
   leftt ^= work;
   right ^= (work << 16);
   work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
   leftt ^= work;
   right ^= (work << 4);
   *block++ = right;
   *block = leftt;
   return;
}

/* Validation sets:
*
* Single-length key, single-length plaintext -
* Key    : 0123 4567 89ab cdef
* Plain  : 0123 4567 89ab cde7
* Cipher : c957 4425 6a5e d31d
*****************************************************************/

void des_key(des_ctx* dc, unsigned char* key)
{
   deskey(key, EN0);
   cpkey(dc->ek);
   deskey(key, DE1);
   cpkey(dc->dk);
}

/*Encrypt several blocks in ECB mode. Caller is responsible for short blocks. */
void des_enc(des_ctx* dc, unsigned char* data, int blocks)
{
   unsigned long work[2];
   int i;
   unsigned char* cp;
   cp = data;
   for (i = 0; i < blocks; i++)
   {
       scrunch(cp, work);
       desfunc(work, dc->ek);
       unscrun(work, cp);
       cp += 8;
   }
}

void des_dec(des_ctx* dc, unsigned char* data, int blocks)
{
   unsigned long work[2];
   int i;
   unsigned char* cp;
   cp = data;

   for (i = 0; i < blocks; i++)
   {
       scrunch(cp, work);
       desfunc(work, dc->dk);
       unscrun(work, cp);
       cp += 8;
   }
}



int des_encrypt(const unsigned char* Key, const unsigned char KeyLen, const unsigned char* PlainContent, unsigned char* CipherContent, const int BlockCount)
{
   des_ctx dc;
   unsigned char  keyEn[8];
   unsigned char  keyDe[8];

   if ((BlockCount == 0) || (Key == NULL) || (CipherContent == NULL) || (PlainContent == NULL))
   {
       return -1;
   }

   if ((KeyLen != 8) && (KeyLen != 16))
   {
       return -1;
   }

   memmove(CipherContent, PlainContent, BlockCount * 8);

   switch (KeyLen)
   {
   case 8:    // DES
       memcpy(keyEn, Key, 8);               //keyEn
       des_key(&dc, keyEn);
       des_enc(&dc, CipherContent, BlockCount);
       break;
   case 16:    // 3DES
       memcpy(keyEn, Key, 8);               //keyEn
       memcpy(keyDe, Key + 8, 8);      //keyDe
       des_key(&dc, keyEn);
       des_enc(&dc, CipherContent, BlockCount);

       des_key(&dc, keyDe);
       des_dec(&dc, CipherContent, BlockCount);

       des_key(&dc, keyEn);
       des_enc(&dc, CipherContent, BlockCount);
       break;

   default:
       return -1;
   }
   return 0;
}

void sub_AAAAA(unsigned char* enc, unsigned char* key) {

   unsigned char cipher[64];
   unsigned char plain[64];
   memcpy((void*)plain, cipher, 64);
   des_encrypt(key, 8, enc, cipher, 8);

   for (int ii = 0; ii < 64; ii++) {
       if (cipher[ii] != BBBBB[ii]) {
           printf("wrong_wrong!!!\n");
           exit(1);
       }
   }
}

int main() {
   unsigned char* Buf = (char*)malloc((LEN + 1) * sizeof(char));
   unsigned char head[9];
   unsigned char trail[2];

   printf("Welcome to HZNUCTF!!!\n");
   printf("Plz input the flag:\n");
   scanf("%s", Buf);

   strncpy_s(head, sizeof(head), Buf + 0, 8);
   strncpy_s(trail, sizeof(trail), Buf + 40, 1);

   if (!strcmp(head, "HZNUCTF{") && !strcmp(trail, "}")) {  // Check format
       unsigned char* content = (char*)malloc((LEN - 5) * sizeof(char));
       strncpy_s(content, LEN - 5, Buf + 8, 32);

       char string[65] = { 0 };
       for (int i = 0; i < 32; i += 2) {
           unsigned char value1 = 0;
           unsigned char value2 = 0;
           unsigned char hex1[5] = { 0 };
           unsigned char hex2[3] = { 0 };

           value1 = content[i];
           sprintf(hex1, "%x", value1);

           value2 = content[i + 1];
           sprintf(hex2, "%x", value2);

           strcat(hex1, hex2);

           unsigned char tmp = hex1[1];
           hex1[1] = hex1[2];
           hex1[2] = tmp;

           strcat(string, hex1);
       }

       sub_AAAAA(string, head);
       printf("Congratulation!!!\n");


       free(content);
   }

   free(Buf);
   return 0;
}


//HZNUCTF{391ds2b9-9e31-45f8-ba4a-4904a2d8}