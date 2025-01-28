/*
    CPL Assignment 1: 1024 bit integer (bigInt) implementation
    Author: Atharva Joshi
    Enrollment number: BT23CSE020
    driver file
    commands for execution: (driver.c, bigInt.c and bigInt.h should be in the same directory)
    gcc -g driver.c bigInt.c -o bi
    ./bi
*/

#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"
#include "string.h"

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
        // 218FAA3C47251A35443AF4949F1A49A2 hex = 44610475097013365513301563148380817826 dec 
        // D34B4D102141CC5FF2D17964EE36CBF41 hex = 4493729478375839892748392839278599388993 dec

        operand 2:
        // 12696B98A50591275884 hex = 86947257947594963769476 dec 
        // 3DAA2426B4D0520078E9AF152DEE6EE hex = 5122895715137865095453046482382284526 dec
        // CAA6F08F31365AE78A3AB0F714ABD9A6ED hex = 68958939020859489385793028485998395893485 dec
        

        respective correct answers:
        // 5124523211330192727213959787153919464700284 dec = 3AD39EC657405AFE8FB0EB1CB65ABAFBF97C hex
        // 81589B4AFF32802A733EACD0FC9E6D1E59FE3DF3F544C1F84C13014714009C hex = 228534811724754206893276854028507452875988991976151191745590016742104760476 dec
        // A743342F7AA0EDC86B11B4C3D972C500A20AAB12614B92058990CCC618398CA352D hex = 309882817075558264594609038919817131806139263555242763746749377705418041309410605 dec
        NOTE: for last output, result is greater than 1024 bits hence the 1024 least significant bits are retained, higher bits are truncated as expected
    */
    char MultInput1[] = "-218FAA3C47251A35443AF4949F1A49A2"; 
    char MultInput2[] = "3DAA2426B4D0520078E9AF152DEE6EE"; 
    char Answer[] = "-81589B4AFF32802A733EACD0FC9E6D1E59FE3DF3F544C1F84C13014714009C"; 

    // resetting bint1 and bint2 with new values
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

    // trying out decimal string input
    char InputDec1[] = "-44610475097013365513301563148380817826";
    char InputDec2[] = "5122895715137865095453046482382284526";

    InitialiseBigInt(&bint1); InitialiseBigInt(&bint2);
    printf("before dec set:\n");
    PrintAsItIs(&bint1);
    SetBigIntDec(InputDec1, &bint1); SetBigIntDec(InputDec2, &bint2);
    PrintAsItIs(&bint1); PrintAsItIs(&bint2);
    printf("first bint:\n");
    str_result = Print_bigInt_hex(&bint1);
    free(str_result); str_result = NULL;
    printf("second bint:\n");
    str_result = Print_bigInt_hex(&bint2);
    free(str_result); str_result = NULL;

    result = MultiplyBigInts(bint1, bint2);
    printf("%s multiplied by %s = ", InputDec1, InputDec2);
    str_result = Print_bigInt_hex(&result);
    free(str_result); str_result = NULL;

    // freeing dynamically allocated space for bigInts:
    FreeBigInt(&bint1); FreeBigInt(&bint2); FreeBigInt(&result);


    return 0;
}