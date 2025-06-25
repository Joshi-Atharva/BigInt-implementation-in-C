# BigInt Implementation in C

This repository contains a C implementation of arbitrary-precision integers (BigInt) supporting 1024-bit unsigned integers with an additional boolean sign variable. The implementation uses base-32 representation for efficient storage, with numbers stored in an array of 32 integers.

## Implementation Details

The core structure represents large integers using:
- An array of 32 integers (for base-32 storage)
- A boolean sign flag (for signed operations)
- Current implementation includes:
  - Addition
  - Subtraction 
  - Multiplication

The base-32 representation was chosen to optimize storage efficiency while maintaining good computational performance.

## Current Features

- **Basic Arithmetic Operations**:
  - `addBigInt()` - Adds two BigInt numbers
  - `subtractBigInt()` - Subtracts one BigInt from another  
  - `multiplyBigInt()` - Multiplies two BigInt numbers

- **Helper Functions**:
  - Comparison functions
  - Conversion utilities
  - Memory management

## Usage

Include the header file and link against the implementation:

```c
#include "bigint.h"

int main() {
    BigInt num1 = createBigInt(...);
    BigInt num2 = createBigInt(...);
    
    BigInt sum = addBigInt(num1, num2);
    // ... other operations
    
    freeBigInt(num1);
    freeBigInt(num2);
    return 0;
}
```
## Scope for further improvement
- The boolean sign variable, which for simplicity was implemented using the `enum` datatype in C, can be replaced with bitfields so that it becomes more efficient with regards to memory. Since this was a learning project, I did not implement it but it can potentially implemented.
- A more 'complete' libary can be made by introducing division functionality.

## Credits
My code previously required input in hexadecimal format. The algorithm that allowed for decimal input was possible due to generous inputs from the Stack Overflow community on my question : [link to my stack exchange question](https://stackoverflow.com/q/79372582)
