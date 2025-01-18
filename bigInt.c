/*
    1024 bit integer (bigInt) implementation
    Author: Atharva Joshi
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum{POSITIVE, NEGATIVE} Sign;

typedef int int32;
typedef long long int int64;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
typedef struct int1024_tag {
    Sign sign;
    uint32* ints;
}bigInt;

void InitialiseBigInt(bigInt* bint_ptr) {
    uint32* ret_ptr = (uint32*)malloc(32*sizeof(uint32));
    uint32 i = 0;
    while( i < 32 ) {
        ret_ptr[i] = 0;
        i = i + 1;
    }
    bint_ptr->sign = POSITIVE;
    bint_ptr->ints = ret_ptr;
}

uint32 HexChar2Num(char c) {
    uint32 ret_val;
    if( '0' <= c && c <= '9' ) {
        ret_val = (uint32)(c - '0');
    }
    else if( 'a' <= c && c <= 'f' ) {
        ret_val = 10 + (uint32)(c - 'a');
    }
    else if( 'A' <= c && c <= 'F' ) {
        ret_val = 10 + (uint32)(c - 'A');
    }
    else {
        ret_val = -1; // denotes error condition
    }
    return ret_val;
}

void SetBigIntHex(char* hex, bigInt* bint_ptr) {
    uint32 str_len, start, ints_idx, nibble_idx, hex_digit_cnt, str_idx, sum, prev_sum;
    Sign num_sign;

    str_len = strlen(hex);

    uint32* ints_ptr = bint_ptr->ints;

    if( hex[0] == '-' ) {
        num_sign = NEGATIVE;
        start = 1;
    }
    else if( hex[0] == '+' ) {
        num_sign = POSITIVE;
        start = 1;
    }
    else {
        num_sign = POSITIVE;
        start = 0;
    }
    bint_ptr->sign = num_sign;

    ints_idx = 0;
    str_idx = str_len - 1;
    while( ints_idx < 32 && str_idx >= start ) {
        nibble_idx = 0; 
        while( nibble_idx < 8 && str_idx >= start ) { // there are 8 nibbles in one uint32 variable
            ints_ptr[ints_idx] += (HexChar2Num(hex[str_idx]) << (nibble_idx*4));
            nibble_idx += 1;
            str_idx -= 1;
        }
        ints_idx += 1;
    }
}


void revstr(char* s) {
    if( s != NULL ) {
        int i = 0;
        int j = strlen(s) - 1;
        char temp;
        while( i < j ) {
            temp = s[i];
            s[i] = s[j];
            s[j] = temp;
            i = i + 1; j = j - 1;
        }
    }
}
char* Print_bigInt_hex(bigInt* bint_ptr) {
    char* s = (char*)malloc(sizeof(char)*66);
    
    uint32* ints_ptr = bint_ptr->ints;
    uint32 ints_idx = 0, str_idx = 0; uint32 num, digit_idx, part;
    char dec2hex[] = "0123456789ABCDEF"; /* dictionary implementation using string for decimal to hex digit conversion */

    while( ints_idx < 32 && str_idx < 64 ) {
        num = ints_ptr[ints_idx]; 
        part = 0;
        while( part < 8 && str_idx < 64) {
            digit_idx = num & 15; /* BITMASKING: extracting last four bits for hex digit */
            s[str_idx] = dec2hex[digit_idx];
            part = part + 1;
            str_idx += 1; 
            num = num >> 4; /* for next hex digit */
        }
        ints_idx += 1;
    }
    s[str_idx] = '+'; /* default init */
    s[str_idx + 1] = '\0'; /* null termination */
    revstr(s);
    if( bint_ptr->sign == POSITIVE ) {
        s[0] = '+';
    }
    else if( bint_ptr->sign == NEGATIVE) {
        s[0] = '-';
    }
    else {
        printf("ERROR - Sign undefined\n");
    }

    char* str_start = s;

    // removal of leading zeros
    /*
    char num_sign = s[0];
    s = s + 1;
    while( *s == '0' ) {
        str_start = s;
        s = s + 1;
    }
    *str_start = num_sign;
    if( str_start[1] == '\0' ) {
        str_start = str_start - 1;
    }
    */

    // string printing and return 
    printf("%s\n", str_start);
    return s;
}

void FreeBigInt(bigInt* bint_ptr) {
    uint32 i = 0;
    uint32* ints_ptr = bint_ptr->ints;
    while( i < 32 ) {
        free(ints_ptr + i);
        i = i + 1;
    }
    bint_ptr->ints = NULL;
}

void PrintAsItIs(bigInt* bint_ptr) {
    int i = 0;
    while( i < 32 ) {
        printf("i = %u: ints[i] = %u\n", i, bint_ptr->ints[i]);
        i = i + 1;
    }
}

// function prototypes
bigInt SubtractBigInts(bigInt, bigInt);
bigInt AddBigInts(bigInt, bigInt);

bigInt SubtractBigInts(bigInt b1, bigInt b2) {
    bigInt ret_val;
    InitialiseBigInt(&ret_val);
    if( b1.sign == POSITIVE && b2.sign == NEGATIVE ) {
        b2.sign = POSITIVE;
        ret_val = AddBigInts(b1, b2);
    }
    else if( b1.sign == NEGATIVE && b2.sign ==  NEGATIVE ) {
        b1.sign = POSITIVE;
        b2.sign = POSITIVE;
        ret_val = SubtractBigInts(b2, b1);
    }
    else if( b1.sign == NEGATIVE && b2.sign == POSITIVE ) {
        b1.sign = POSITIVE; b2.sign = POSITIVE;
        ret_val = AddBigInts(b1, b2);
        ret_val.sign = NEGATIVE;
    }
    else { /* both positive */
    }
    return ret_val;
}

bigInt AddBigInts(bigInt b1, bigInt b2) {
    Sign bs1 = b1.sign, bs2 = b2.sign;
    bigInt ret_val;
    InitialiseBigInt(&ret_val);
    if( !bs1 && !bs2 ) {
        /* root case - adding two positive numbers */
        uint32 i, carry;
        i = 0; carry = 0;
        while( i < 32 ) {
            ret_val.ints[i] = b1.ints[i] + b2.ints[i] + carry;
            if( ret_val.ints[i] < b1.ints[i] ) {
                carry = 1;
            }
            else {
                carry = 0;
            }
            i = i + 1;
        }
        ret_val.sign = POSITIVE;

    }
    else if( !bs1 && bs2 ) {
        b2.sign = POSITIVE;
        ret_val = SubtractBigInts(b1, b2);
    }
    else if( bs1 && !bs2 ) {
        b1.sign = POSITIVE;
        ret_val = SubtractBigInts(b2, b1);
    }
    else if( bs1 && bs2 ) {
        b1.sign = POSITIVE; b2.sign = POSITIVE;
        ret_val = AddBigInts(b1, b2);
        ret_val.sign = NEGATIVE;
    }
    return ret_val;
}

int main() {
    char input1_dec[] = "+3902384390938023884"; /* 30573382 dec = 1D28346 hex, +3902384390938023884 dec = +36280DED86E3D7CC hex */
    char input1_hex[] = "+36280DED86E3D7CC";
    char input2_dec[] = "8902384390968597266";
    char input2_hex[] = "7B8B9F6FCDAA5B12";
    char addition_dec[] = "12804768781906621150";
    char addition_hex[] = "B1B3AD5D548E32DE";
    bigInt bint1, bint2, result, dummy; char* output;
    char *str_bint1, *str_bint2, *str_result, *str_dummy;

    // initialising bigInts:
    InitialiseBigInt(&bint1); InitialiseBigInt(&bint2);

    // printing each uint32 field after initialisation:
    printf("bint1:\n"); PrintAsItIs(&bint1); printf("bint2:\n"); PrintAsItIs(&bint2);

    // printing hex string after init for verification
    printf("before set: \n");
    str_bint1 = Print_bigInt_hex(&bint1); str_bint2 = Print_bigInt_hex(&bint2);
    free(str_bint1); str_bint1 = NULL;
    free(str_bint2); str_bint2 = NULL;

    // setting the bigInts to desired input value
    SetBigIntHex(input1_hex, &bint1);
    SetBigIntHex(input2_hex, &bint2);

    // printing after setting:
    printf("after setting:\n");
    str_bint1 = Print_bigInt_hex(&bint1); str_bint2 = Print_bigInt_hex(&bint2);
    free(str_bint1); str_bint1 = NULL;
    free(str_bint2); str_bint2 = NULL;

    // adding the bigInts
    result = AddBigInts(bint1, bint2); 
    printf("Addition result:\n"); 
    str_result = Print_bigInt_hex(&result);

    // comparing return string with target value:
    if( strcmp(str_result, addition_hex) == 0 ) {
        printf("strings are same\n");
    }
    else {
        printf("strings differ\n");
    }

    free(str_result); str_result = NULL;
    
    // freeing dynamically allocated space for bigInts:
    FreeBigInt(&bint1); FreeBigInt(&bint2); FreeBigInt(&result);
    return 0;
}


