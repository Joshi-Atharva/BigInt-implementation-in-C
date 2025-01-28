/* tester file for bigInt */
#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"
#include "string.h"


int main() {
    bigInt b1, b2;
    InitialiseBigInt(&b1); InitialiseBigInt(&b2);
    char InputDec1[] = "4085028504850";
    char InputDec2[] = "9056968044546436346565645756546946084";
    char InputHex2[] = "6D04F0029B11E534E0BB46C3B581424";

    SetBigIntDec(InputDec1, &b1); SetBigIntDec(InputDec2, &b2);
    printf("b2:\n");
    for( int j = 0; j < 5; j++ ) {
        printf("j = %d, ints[j] = %u \n", j, b2.ints[j]);
    }
    SetX(&b2, 0);
    SetBigIntHex(InputHex2, &b2);

    Print_bigInt_hex(&b1); Print_bigInt_hex(&b2);

    printf("As it is print:\n");
    printf("b2:\n");
    for( int j = 0; j < 5; j++ ) {
        printf("j = %d, ints[j] = %u \n", j, b2.ints[j]);
    }
    return 0;
}