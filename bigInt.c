/*
    CPL Assignment 1: 1024 bit integer (bigInt) implementation
    Author: Atharva Joshi
    Enrollment number: BT23CSE020
*/

#include "bigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SetX(bigInt* bint_ptr, uint32 X) {
    bint_ptr->ints[0] = X;
    int i = 1;
    while( i < 32 ) {
        bint_ptr->ints[i] = 0;
        i = i + 1;
    }
}
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

bigInt* bigIntX(uint32 X) {
    bigInt* bint_ptr;
    InitialiseBigInt(bint_ptr);
    bint_ptr->ints[0] = X;
    return bint_ptr;
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
    str_len = strlen(hex);
    str_idx = str_len - 1;
    uint32 prev_str_idx = str_len; // to handle the overflow condition
    while( ints_idx < 32 && (str_idx >= start) && (prev_str_idx > str_idx ) ) {
        nibble_idx = 0; 
        while( nibble_idx < 8 && str_idx >= start && (prev_str_idx > str_idx )) { // there are 8 nibbles in one uint32 variable
            ints_ptr[ints_idx] += (HexChar2Num(hex[str_idx]) << (nibble_idx*4));
            nibble_idx += 1;
            prev_str_idx = str_idx;
            str_idx -= 1;
        }
        ints_idx += 1;
    }
}

void CopyBigInts(bigInt *b1_ptr, bigInt *b2_ptr) {
    b1_ptr->sign = b2_ptr->sign;
    int i = 0;
    while( i < 32 ) {
        (b1_ptr->ints)[i] = (b2_ptr->ints)[i];
        i = i + 1;
    }
}
void SetBigIntDec(char* dec, bigInt* bint_ptr) {
    bigInt digit, pot, res, addend; // pot = power of ten, res = current result
    InitialiseBigInt(&digit); InitialiseBigInt(&pot); InitialiseBigInt(&res); InitialiseBigInt(&addend);
    bigInt ten, carry, res_copy; 
    InitialiseBigInt(&ten); InitialiseBigInt(&carry); InitialiseBigInt(&res_copy);
    uint32 dig;
    SetX(&ten, 10);
    SetX(&res, 0); 
    SetX(&pot, 1);
    SetX(&carry, 0);

    int i = strlen(dec) - 1;
    while( i >= 0 && ((dec[i] != '-') && (dec[i] != '+')) ) {
        dig = (uint32)(dec[i] - '0');
        SetX(&digit, dig);
        addend = MultiplyBigInts(digit, pot);
        CopyBigInts(&res_copy, &res);
        AddBigInts(res_copy, addend, &carry, &res); /* passing res instead of res_copy with copy the memory address of res.ints, it will still refer to the same memory location. */
        pot = MultiplyBigInts(pot, ten);
        i = i - 1;
    }
    if( dec[0] == '-' ) {
        res.sign = NEGATIVE;
    }
    else {
        res.sign = POSITIVE;
    }
    CopyBigInts(bint_ptr, &res);
    // *bint_ptr = res; // memory leak ( copies the address of res.ints to bint_ptr->ints, losing reference to the memory block previously pointed to by bint_ptr->ints )
    FreeBigInt(&digit); FreeBigInt(&pot); FreeBigInt(&addend); FreeBigInt(&res); 
    FreeBigInt(&ten); FreeBigInt(&carry); FreeBigInt(&res_copy);
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
    char* s = (char*)malloc(sizeof(char)*258);
    
    uint32* ints_ptr = bint_ptr->ints;
    uint32 ints_idx = 0, str_idx = 0; uint32 num, digit_idx, part;
    char dec2hex[] = "0123456789ABCDEF"; /* dictionary implementation using string for decimal to hex digit conversion */
    uint32 first_non_zero_idx = 257;
    while( ints_idx < 32 && str_idx < 256 ) {
        num = ints_ptr[ints_idx]; 
        part = 0;
        while( part < 8 && str_idx < 256) {
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

    // removal of leading zeros - shifting approach
    uint32 i = 1; Boolean found = FALSE;
    while( (s[i] != '\0') && !found ) {
        if( s[i] != '0' ) {
            found = TRUE;
            first_non_zero_idx = i;
        }
        else {
            i = i + 1;
        }
    }
    i = 1;
    while( (i + first_non_zero_idx - 1) < 257) { // s[i + first_non_zero_idx - 1] != '\0'
        s[i] = s[i + first_non_zero_idx - 1];
        i = i + 1;
    }
    s[i] = '\0';
    if( i == 1 ) {
        s[0] = '0';
        s[1] = '\0';
    } // to stay consistent with code convention (in case of any unexpected error), use +, and to stay consistent with math, skip it (and make changes in code where necessary)
    // string printing and return 
    printf("%s\n", s);
    return s;
}

void FreeBigInt(bigInt* bint_ptr) {
    uint32* ints_ptr = bint_ptr->ints;
    free(ints_ptr);
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
void AddBigInts(bigInt, bigInt, bigInt*, bigInt*);

// returns true if b1 < b2 else returns false
Boolean CompareBigInts(bigInt b1, bigInt b2) {
    int ints_idx = 31; Boolean ret_val;
    if( b1.sign == NEGATIVE && b2.sign == POSITIVE ) {
        ret_val = TRUE;
    }
    else if( b1.sign == POSITIVE && b2.sign == NEGATIVE ) {
        ret_val = FALSE;
    }
    else if( b1.sign == NEGATIVE && b2.sign == NEGATIVE ) {
        b1.sign = POSITIVE;
        b2.sign = POSITIVE;
        ret_val = CompareBigInts(b2, b1); 
        // inverting and comparing: b1 <= b2 => -b2 <= -b1
        // and b1 > b2 => -b2 > -b1
    }
    else if( b1.sign == POSITIVE && b2.sign == POSITIVE ) {
        Boolean done = FALSE; 
        ret_val = FALSE;
        while( ints_idx >= 0 && !done ) {
            if( b1.ints[ints_idx] != b2.ints[ints_idx] ) {
                done = TRUE;
                ret_val = (b1.ints[ints_idx] < b2.ints[ints_idx]); // ret_val = TRUE if b1 < b2 else if b1 > b2 it is FALSE (equality not possible due to if condition)
            }
            else {
                ints_idx = ints_idx - 1;
            }
        }
    }
    return ret_val;
}
bigInt SubtractBigInts(bigInt b1, bigInt b2) {
    bigInt ret_val, carry;
    InitialiseBigInt(&ret_val);
    InitialiseBigInt(&carry);

    if( CompareBigInts(b1, b2) == TRUE ) { /* ie b1 < b2 */
        ret_val = SubtractBigInts(b2, b1);
        ret_val.sign = NEGATIVE; 
    }
    else { /* b1 >= b2 */
        if( b1.sign == POSITIVE && b2.sign == NEGATIVE ) {
            b2.sign = POSITIVE;
            AddBigInts(b1, b2, &carry, &ret_val);
        }
        else if( b1.sign == NEGATIVE && b2.sign ==  NEGATIVE ) {
            b1.sign = POSITIVE;
            b2.sign = POSITIVE;
            ret_val = SubtractBigInts(b2, b1);
        }
        else if( b1.sign == NEGATIVE && b2.sign == POSITIVE ) { // obsolete possibility - does not occur given the use of CompareBigInts() in the if-else block
            b1.sign = POSITIVE; b2.sign = POSITIVE;
            AddBigInts(b1, b2, &carry, &ret_val);
            ret_val.sign = NEGATIVE;
        }
        else { /* both positive */
            uint32 i, borrow;
            i = 0, borrow = 0;
            while( i < 32 ) {
                ret_val.ints[i] = b1.ints[i] - b2.ints[i] - borrow;
                if( (borrow && (ret_val.ints[i] >= b1.ints[i])) || (!borrow && (ret_val.ints[i] > b1.ints[i])) ) {
                    borrow = 1;
                }
                else {
                    borrow = 0;
                }
                i = i + 1;
            }
        }
    } /* endif: b1 > b2 */
    return ret_val;
}
void HalfAdder(uint64 a, uint64 b, uint64* res_ptr, uint64* carry_ptr) {
    uint64 carry = 0;
    *res_ptr = a + b + carry;
    // function is not made to handle the case of carry other than 0 or 1
    if( (!carry && (*res_ptr < a)) || (carry && (*res_ptr <= a)) ) { // second part of || is written for completeness sake, the first part is evaluated as carry is always set to 0
        *carry_ptr += 1;
    }
    else {
        *carry_ptr += 0;
    }
}
void AddBigInts(bigInt b1, bigInt b2, bigInt* carry_ptr, bigInt* result_ptr) {
    Sign bs1 = b1.sign, bs2 = b2.sign;
    if( !bs1 && !bs2 ) {
        /* root case - adding two positive numbers */
        uint32 i, carry;
        i = 0; carry = 0;
        while( i < 32 ) {
            result_ptr->ints[i] = b1.ints[i] + b2.ints[i] + carry;
            if( (!carry && (result_ptr->ints[i] < b1.ints[i])) || (carry && (result_ptr->ints[i] <= b1.ints[i])) ) {
                carry = 1;
            }
            else {
                carry = 0;
            }
            i = i + 1;
        }
        result_ptr->sign = POSITIVE;
        bigInt temp;
        InitialiseBigInt(&temp);
        SetX(&temp, carry);
        bigInt dummy;
        InitialiseBigInt(&dummy);

        if( carry != 0 ) {
            AddBigInts(*carry_ptr, temp, &dummy, carry_ptr);
        }

    }
    else if( !bs1 && bs2 ) {
        b2.sign = POSITIVE;
        *result_ptr = SubtractBigInts(b1, b2);
    }
    else if( bs1 && !bs2 ) {
        b1.sign = POSITIVE;
        *result_ptr = SubtractBigInts(b2, b1);
    }
    else if( bs1 && bs2 ) {
        b1.sign = POSITIVE; b2.sign = POSITIVE;
        AddBigInts(b1, b2, carry_ptr, result_ptr);
        result_ptr->sign = NEGATIVE;
    }
}
Sign MultSign(Sign s1, Sign s2) {
    Sign ret_val = POSITIVE;
    if( (!s1 && !s2) || (s1 && s2) ) {
        ret_val = POSITIVE;
    }
    else if( (!s1 && s2) || (s1 && !s2) ) {
        ret_val = NEGATIVE;
    }
    return ret_val;
}
bigInt MultiplyBigInts(bigInt b1, bigInt b2) {
    // init result matrix
    bigInt result;
    InitialiseBigInt(&result);

    // initialise product matrix
    uint64** products = (uint64**)malloc(sizeof(uint64*)*32);
    int i = 0;
    while( i < 32 ) {
        products[i] = (uint64*)malloc(sizeof(uint64)*32);
        i = i + 1;
    }

    int j = 0; i = 0;
    while( i < 32 ) {
        j  = 0;
        while( j < 32 ) {
            products[i][j] = ((uint64)(b1.ints[i]))*((uint64)(b2.ints[j]));
            j = j + 1;
        }
        i = i + 1;
    }
    // product matrix initialised

    uint64 carry, sum, prev = 0;
    uint32 val, next = 0;
    carry = 0, sum = 0;

    i = 0; j = 0; int total = 0;

    while( total < 32 ) {
        i = total; j = 0;
        HalfAdder(0, prev, &sum, &carry);
        while( j <= total ) {
            HalfAdder(products[i][j], sum, &sum, &carry);
            i = i - 1;
            j = j + 1;
        }
        result.ints[total] = (uint32)(sum & 0x00000000ffffffff);
        prev = (sum & 0xffffffff00000000); // explicit masking for clarity
        prev = prev >> 32;
        carry = carry << 32;
        prev = prev + carry;

        sum = 0; carry = 0;
        total = total + 1;
    }

    // freeing the allocated space
    i = 0; j = 0;
    while( i < 32 ) {
        free(products[i]); products[i] = NULL;
        i = i + 1;
    }
    free(products); products = NULL;

    result.sign = MultSign(b1.sign, b2.sign);
    return result;
}

