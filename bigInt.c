/*
    CPL Assignment 1: 1024 bit integer (bigInt) implementation
    Author: Atharva Joshi
    Enrollment number: BT23CSE020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    uint32 first_non_zero_idx = 65;
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

    // removal of zeros - shifting approach
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
    while( (i + first_non_zero_idx - 1) < 65) { // s[i + first_non_zero_idx - 1] != '\0'
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
    if( (!carry && (*res_ptr < a)) || (carry && (*res_ptr <= a)) ) {
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

int main() {
    char input1_dec[] = "+3902384390938023884"; /* 30573382 dec = 1D28346 hex, +3902384390938023884 dec = +36280DED86E3D7CC hex */
    char input1_hex[] = "+36280DED86E3D7CC";
    char input2_dec[] = "8902384390968597266";
    char input2_hex[] = "7B8B9F6FCDAA5B12";
    char addition_dec[] = "12804768781906621150";
    char addition_hex[] = "+B1B3AD5D548E32DE";
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
    InitialiseBigInt(&dummy);
    InitialiseBigInt(&result);
    AddBigInts(bint1, bint2, &dummy, &result); 
    printf("Addition result:\n"); 
    str_result = Print_bigInt_hex(&result);

    // comparing return string with target value:
    if( strcmp(str_result, addition_hex) == 0 ) {
        printf("Output and target strings match\n");
    }
    else {
        printf("Output and target strings differ\n");
    }

    free(str_result); str_result = NULL;
    
    // subtracting them
    result = SubtractBigInts(bint1, bint2);
    printf("Subtraction result:\n");
    str_result = Print_bigInt_hex(&result);
    free(str_result); str_result = NULL;

    // multiplying two inputs
    /*
        sample inputs:
        operand 1:
        // 331EEE3CC1F9D447F hex = 58938295839287493759 dec
        // D34B4D102141CC5FF2D17964EE36CBF41 hex = 4493729478375839892748392839278599388993 dec

        operand 2:
        // 86947257947594963769476 dec = 12696B98A50591275884 hex
        // CAA6F08F31365AE78A3AB0F714ABD9A6ED hex = 68958939020859489385793028485998395893485 dec

        ans:
        // 5124523211330192727213959787153919464700284 dec = 3AD39EC657405AFE8FB0EB1CB65ABAFBF97C hex
    */
    char MultInput1[] = "331EEE3CC1F9D447F"; // D34B4D102141CC5FF2D17964EE36CBF41 hex = 4493729478375839892748392839278599388993 dec
    char MultInput2[] = "12696B98A50591275884"; // CAA6F08F31365AE78A3AB0F714ABD9A6ED hex = 68958939020859489385793028485998395893485 dec
    char Answer[] = "3AD39EC657405AFE8FB0EB1CB65ABAFBF97C"; // A743342F7AA0EDC86B11B4C3D972C500A20AAB12614B92058990CCC618398CA352D hex = 309882817075558264594609038919817131806139263555242763746749377705418041309410605 dec
    // 34740525889446160134312415869485101144 dec = 0x1A22C82B374E642EB01AB6362332B058 

    printf("before mult set:\n");
    PrintAsItIs(&bint1); PrintAsItIs(&bint2);
    FreeBigInt(&bint1); FreeBigInt(&bint2);
    InitialiseBigInt(&bint1); InitialiseBigInt(&bint2);
    SetBigIntHex(MultInput1, &bint1);
    SetBigIntHex(MultInput2, &bint2);

    printf("after mult set:\n");
    PrintAsItIs(&bint1); PrintAsItIs(&bint2);
    result = MultiplyBigInts(bint1, bint2);
    printf("Multiplication result:\n");
    str_result = Print_bigInt_hex(&result);
    free(str_result); str_result = NULL;


    // freeing dynamically allocated space for bigInts:
    FreeBigInt(&bint1); FreeBigInt(&bint2); FreeBigInt(&result);
    return 0;
}


