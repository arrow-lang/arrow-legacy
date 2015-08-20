// Identitiy, negation, bitwise not, dereference, and address-of [2]
+x;
-x;
!x;
*x;
&x;

// Multiplication, division, and remainder [3]
-x * y;
y / +x;
!y % x;

// Addition and subtraction [4]
y + x * z;
y - x % z;
(a - b) / y;

// Bitwise AND [5]
a & b;
a & b * x;
(a & b) * x;

// Bitwise XOR [6]
a ^ b;
b ^ c & d;

// Bitwise OR [7]
a ^ b | c;
b | c & b ^ a;

// Greater than; and, greater than or equal to [8]
x > y;
x >= y;

// Less than; and, less than or equal to [8]
y > x;
y >= x;

// Equal to; and, not equal to [10]
x == y;

// do_logical NOT [11]
not x == y;
not y > x;

// do_logical AND [12]
not x < y and false;

// do_logical OR [13]
not x < y or false;
y > x or x < y and y > 1;

// Assignment [15]
x = y or z;
