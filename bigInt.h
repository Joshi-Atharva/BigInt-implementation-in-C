#ifndef BIGINT_H
#define BIGINT_H

typedef enum{POSITIVE, NEGATIVE} Sign;
typedef enum{FALSE, TRUE} Boolean;
typedef int int32;
typedef long long int int64;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
typedef struct int1024_tag {
    Sign sign;
    uint32* ints;
}bigInt;

void SetX(bigInt* bint_ptr, uint32 X);
void InitialiseBigInt(bigInt* bint_ptr);

bigInt* bigIntX(uint32 X);
uint32 HexChar2Num(char c);
void SetBigIntHex(char* hex, bigInt* bint_ptr);
void CopyBigInts(bigInt *b1_ptr, bigInt *b2_ptr);
void SetBigIntDec(char* dec, bigInt* bint_ptr);
void revstr(char* s);
char* Print_bigInt_hex(bigInt* bint_ptr);
void FreeBigInt(bigInt* bint_ptr);
void PrintAsItIs(bigInt* bint_ptr);
bigInt SubtractBigInts(bigInt, bigInt);
void AddBigInts(bigInt, bigInt, bigInt*, bigInt*);

Boolean CompareBigInts(bigInt b1, bigInt b2);
void HalfAdder(uint64 a, uint64 b, uint64* res_ptr, uint64* carry_ptr);
Sign MultSign(Sign s1, Sign s2);

bigInt MultiplyBigInts(bigInt b1, bigInt b2);

#endif
