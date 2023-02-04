# fparser
Math Function Parser for C++


# Function syntax

The functions understood by the program are very similar to math expressions in the C language.

Arithmetic float expressions can be created from float literals, variables or functions using the following operators in this order of precedence:

	()			expressions in parentheses first
	A^B			exponentiation (A raised to the power B)
	-A			unary minus
	!A			unary logical not (result is 1 if int(A) is 0, else 0)
	A*B A/B A%B		multiplication, division and modulo
	A+B A-B			addition and subtraction
	A=B A!=B A<B
	A<=B A>B A>=B		comparison between A and B (result is either 0 or 1)
	A&B			result is 1 if int(A) and int(B) differ from 0, else 0
	A|B			result is 1 if int(A) or int(B) differ from 0, else 0



Since the unary minus has higher precedence than any other operator, for example the following expression is valid: x*-y

The class supports these functions:

abs(A) : Absolute value of A. If A is negative, returns -A otherwise returns A.

acos(A) : Arc-cosine of A. Returns the angle, measured in radians, whose cosine is A.

acosh(A) : Same as acos() but for hyperbolic cosine.

asin(A) : Arc-sine of A. Returns the angle, measured in radians, whose sine is A.

asinh(A) : Same as asin() but for hyperbolic sine.

atan(A) : Arc-tangent of A. Returns the angle, measured in radians, whose tangent is (A).

atan2(A,B) : Arc-tangent of A/B. The two main differences to atan() is that it will return the right angle depending on the signs of A and B (atan() can only return values betwen -pi/2 and pi/2), and that the return value of pi/2 and -pi/2 are possible.

atanh(A) : Same as atan() but for hyperbolic tangent.

ceil(A) : Ceiling of A. Returns the smallest integer greater than A. Rounds up to the next higher integer.

cos(A) : Cosine of A. Returns the cosine of the angle A, where A is measured in radians.

cosh(A) : Same as cos() but for hyperbolic cosine.

cot(A) : Cotangent of A (equivalent to 1/tan(A)).

csc(A) : Cosecant of A (equivalent to 1/sin(A)).

exp(A) : Exponential of A. Returns the value of e raised to the power A where e is the base of the natural logarithm, i.e. the non-repeating value approximately equal to 2.71828182846.

floor(A) : Floor of A. Returns the largest integer less than A. Rounds down to the next lower integer.

if(A,B,C) : If int(A) differs from 0, the return value of this function is B, else C.

int(A) : Rounds A to the closest integer. 0.5 is rounded to 1.

log(A) : Natural (base e) logarithm of A.

log10(A) : Base 10 logarithm of A.

max(A,B) : If A>B, the result is A, else B.

min(A,B) : If A<B, the result is A, else B.

sec(A) : Secant of A (equivalent to 1/cos(A)).

sin(A) : Sine of A. Returns the sine of the angle A, where A is measured in radians.

sinh(A) : Same as sin() but for hyperbolic sine.

sqrt(A) : Square root of A. Returns the value whose square is A.

tan(A) : Tangent of A. Returns the tangent of the angle A, where A is measured in radians.

tanh(A) : Same as tan() but for hyperbolic tangent.

Additionally, any function name with a number smaller than the current one can be used in the current function. For example, f2(x) can call f1 eg. like this: "2*f1(x*5)"

The constant "pi" is supported.

# Examples of function string

1+2

x-1

-sin(pi*sqrt(x^2))

exp(-x*x) * sin(x*10)

100.0 * cos((2 * pi/180) * x)



