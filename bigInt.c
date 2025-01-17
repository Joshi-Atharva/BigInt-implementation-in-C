/*
    1024 bit integer (bigInt) implementation
    Author: Atharva Joshi
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum{POSITIVE, NEGATIVE} Sign;


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
void SetBigInt(char* s, bigInt* bint_ptr) {
    uint32 len = strlen(s), start = 0;
    uint32 i = len - 1;
    uint32 j = 0;
    uint32* ret_ptr = bint_ptr->ints;
    uint32 sum = 0, prev_sum = 0;
    Sign num_sign;

    if( s[0] == '-' ) {
        num_sign = NEGATIVE;
        start = 1;
    }
    else if( s[0] == '+' ) {
        num_sign = POSITIVE;
        start = 1;
    }
    else {
        num_sign = POSITIVE;
        start = 0;
    }

    int p_10 = 1;
    while( (i >= start) && ( j < 32 )) {
        prev_sum = sum;
        sum = sum + (uint32)(s[i]-'0')*p_10;

        if( prev_sum > sum ) {  
            /* using the concept of overflow to initialise 32 bit parts of bigint */
            ret_ptr[j] = prev_sum; /* <= 32 bit */
            j = j + 1;
            p_10 = 1;
            prev_sum = (uint32)(s[i] - '0')*p_10; /* carry */
            sum = (uint32)(s[i] - '0')*p_10;
        }
        i = i - 1;
        p_10 = p_10*10;
    }
    ret_ptr[j] = sum; /* <= 32 bit */
    j = j + 1;


    bint_ptr->sign = num_sign;

    printf("bigInt initialised\n");
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
    char dec2hex[] = "0123456789abcdef"; /* dictionary implementation using string for decimal to hex digit conversion */

    while( ints_idx < 32 && str_idx < 65 ) {
        num = ints_ptr[ints_idx]; 
        part = 0;
        while( part < 8 ) {
            digit_idx = num & 15; /* extracting last four bits for hex digit */
            s[str_idx] = dec2hex[digit_idx];
            part = part + 1;
            str_idx += 1; 
            num = num >> 4; /* next hex digit */
        }
        ints_idx += 1;
    }
    s[str_idx] = '\0'; /* NULL termination */
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
    printf("%s\n", s);
    return s;
}

void FreeBigInt(bigInt* bint_ptr) {
    uint32 i = 0;
    uint32* ints_ptr = bint_ptr->ints;
    while( i < 32 ) {
        free(ints_ptr + i);
        i = i + 1;
    }
}

void PrintAsItIs(bigInt* bint_ptr) {
    int i = 0;
    while( i < 32 ) {
        printf("i = %d: ints[i] = %d\n", i, bint_ptr->ints[i]);
        i = i + 1;
    }
}

int main() {
    char input[] = "+3902384390938023884"; /* 30573382 dec = 1D28346 hex */
    char answer[] = "+36280DED86E3D7CC";
    bigInt bint; char* output;
    InitialiseBigInt(&bint);
    PrintAsItIs(&bint);
    printf("before set: \n");
    output = Print_bigInt_hex(&bint);
    free(output); output = NULL;

    SetBigInt(input, &bint);
    printf("after setting:\n");
    output = Print_bigInt_hex(&bint);
    printf("after set:\n");
    PrintAsItIs(&bint);
    if( strcmp(output, answer) == 0 ) {
        printf("Successfull");
    }
    else {
        printf("strings differ");
    }
    free(output); output = NULL;
    FreeBigInt(&bint);
    return 0;
}


