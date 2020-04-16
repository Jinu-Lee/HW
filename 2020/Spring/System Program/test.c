#include <stdio.h>
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  int res = x + (~y) + 1;
  int sign_x = (x >> 31) & 1;
  int sign_y = (y >> 31) & 1;
  int sign_res = (res >> 31) & 1;
  
  int overflow = (sign_x ^ sign_y) & !(sign_y ^ sign_res);

  return (!overflow);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  int n8 = n << 3;
  int m8 = m << 3;

  int mask = (((x >> m8) & 0xFF) << n8) | (((x >> n8) & 0xFF) << m8);
  x &= ~(0xFF << m8);
  x &= ~(0xFF << n8);

  return (x | mask);
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  int shifted_x = x >> n;
  int mask = ~(((1 << 31) >> n) << 1);

  return (shifted_x & mask);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  int m = 0x11, temp, cnt;
  m |= (m << 8);
  m |= (m << 16);

  temp = m & x;
  temp += m & (x >> 1);
  temp += m & (x >> 2);
  temp += m & (x >> 3);

  cnt = temp & 0xF;
  cnt += (temp >> 4) & 0xF;
  cnt += (temp >> 8) & 0xF;
  cnt += (temp >> 12) & 0xF;
  cnt += (temp >> 16) & 0xF;
  cnt += (temp >> 20) & 0xF;
  cnt += (temp >> 24) & 0xF;
  cnt += (temp >> 28) & 0xF;

  return cnt;
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  int sub = y + (~x) + 1;
  int sign_x = (x >> 31) & 1;
  int sign_y = (y >> 31) & 1;
  int sign_sub = (sub >> 31) & 1;
  
  int overflow = (sign_y ^ sign_x) & !(sign_x ^ sign_sub);
  int case_1 = overflow & (!sign_x) & sign_y;
  int case_2 = (!overflow) & sign_sub;
  return (case_1 | case_2);
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int k = 33 + (~n);
  int y = (x << k) >> k; 
  
  return !(x ^ y);
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  int k = 32 + (~n);

  int x_1 = x << n;
  int x_2 = (x >> k) >> 1;
  x_2 &= ~(((1 << 31) >> k));

  return (x_1 | x_2);
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  int frac_flag = 0;
  int exp_flag = 1;
  int cnt = 0;
  while(cnt <= 30) {
    int x = (uf >> cnt) & 1;
    if(cnt <= 22) {
      if (x) {
        frac_flag = 1;
      }
    }
    else {
      if(!x) {
        exp_flag = 0;
      }
    }
    cnt += 1;
  }
  if(frac_flag && exp_flag) {
    return uf;
  }
  return uf & ~(1 << 31);
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  int sign_x = 0, first_one_pos = 31, frac, temp1, temp2, flag = 0;

  if(x < 0) {
    sign_x = 1 << 31;
    x = -x;
  }

  if(x == 0) {
    return x;
  }

  while(first_one_pos >= 0) {
    if(x >> first_one_pos) {
      break;
    }
    first_one_pos--;
  }

  frac = ((1 << first_one_pos) - 1) & x;

  if(first_one_pos >= 24) {
    temp1 = 1 << (first_one_pos - 24);
    temp2 = ((temp1 << 1) - 1) & frac;
    if (temp2 > temp1) {
      flag = 1;
    }
    else if(temp2 == temp1) {
      if((frac >> (first_one_pos - 23)) & 1) {
        flag = 1;
      }
    }
    if (flag) {
      frac += temp2;
    }
    frac >>= (first_one_pos - 23);
  }
  else {
    frac <<= (23 - first_one_pos);
  }

  return sign_x + ((127 + first_one_pos) << 23) + frac;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  int exp_all_one = ~((1 << 31) >> 23);
  int inf = (1 << 31) >> 8;
  int exp = (uf >> 23) & exp_all_one;
  int sign = (uf >> 31) & 1;
  
  if(exp == exp_all_one) {
    return uf;
  }
  else if(exp == 0) { 
    uf <<= 1;
    if(sign != (uf >> 31)) {
      uf ^= (sign << 31);
    }
  }
  else {
    exp += 1;
    if(exp == exp_all_one) {
      uf &= inf;
    }
    uf &= ~inf;
    uf |= ((exp << 23) | (sign << 31));
  }

  return uf;
}

int main()
{
    printf("subOK(0x80000000,0x80000000) : %d\n", subOK(0x80000000,0x80000000));
    printf("subOK(0x80000000,0x70000000) : %d\n", subOK(0x80000000,0x70000000));
    printf("--------------------------------------------\n");
    printf("byteSwap(0x12345678, 1, 3) : %x\n", byteSwap(0x12345678, 1, 3));
    printf("byteSwap(0xDEADBEEF, 0, 2) : %x\n", byteSwap(0xDEADBEEF, 0, 2));
    printf("--------------------------------------------\n");
    printf("logicalShift(0x87654321,4) : %x\n", logicalShift(0x87654321,4));
    printf("--------------------------------------------\n");
    printf("bitCount(5) : %d\n", bitCount(5));
    printf("bitCount(7) : %d\n", bitCount(7));
    printf("--------------------------------------------\n");
    printf("isGreater(4,5) : %d\n", isGreater(4,5));
    printf("isGreater(5,4) : %d\n", isGreater(5,4));
    printf("--------------------------------------------\n");
    printf("fitsBits(5,3) : %d\n", fitsBits(5,3));
    printf("fitsBits(-4,3) : %d\n", fitsBits(-4,3));
    printf("--------------------------------------------\n");
    printf("rotateLeft(0x87654321,4) : %x\n", rotateLeft(0x87654321,4));
}