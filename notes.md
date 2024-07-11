### Bitwise operators

Bitwise operators compare objects bit by bit. For example, 2 short integers 
of 8 bits will result in a third 8 bit integer when precessed by a bitwise operator.

12 & 23 will compare 0000 1100 with 0001 0111, and will result in 0000 0100, which is 4

12 | 23 will result in 0001 1111, which is 31

12 ^ 23 (^ represents a XOR operation) will result in 0001 1011

~ 23 (~ represents the complement) will result in 1110 1000

23 << 1 will result in 0010 1110

23 >> 1 will result in 0000 1011



### Pointers can represent lists

*(valuesPtr+3) is the same as  values[3]

valuesPtr = values is the same as valuesPtr = &values[0]