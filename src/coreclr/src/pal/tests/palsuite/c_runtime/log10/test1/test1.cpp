// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*=============================================================================
**
** Source: test1.c
**
** Purpose: Test to ensure that log10 returns correct values.
** 
** Dependencies: PAL_Initialize
**               PAL_Terminate
**               Fail
**               fabs
**               _isnan
** 
**===========================================================================*/

#include <palsuite.h>

// binary64 (double) has a machine epsilon of 2^-52 (approx. 2.22e-16). However, this 
// is slightly too accurate when writing tests meant to run against libm implementations
// for various platforms. 2^-50 (approx. 8.88e-16) seems to be as accurate as we can get.
//
// The tests themselves will take PAL_EPSILON and adjust it according to the expected result
// so that the delta used for comparison will compare the most significant digits and ignore
// any digits that are outside the double precision range (15-17 digits).

// For example, a test with an expect result in the format of 0.xxxxxxxxxxxxxxxxx will use
// PAL_EPSILON for the variance, while an expected result in the format of 0.0xxxxxxxxxxxxxxxxx
// will use PAL_EPSILON / 10 and and expected result in the format of x.xxxxxxxxxxxxxxxx will
// use PAL_EPSILON * 10.
#define PAL_EPSILON 8.8817841970012523e-16

#define PAL_NAN     sqrt(-1.0)
#define PAL_POSINF -log(0.0)
#define PAL_NEGINF  log(0.0)

/**
 * Helper test structure
 */
struct test
{
    double value;     /* value to test the function with */
    double expected;  /* expected result */
    double variance;  /* maximum delta between the expected and actual result */
};

/**
 * validate
 *
 * test validation function
 */
void __cdecl validate(double value, double expected, double variance)
{
    double result = log10(value);

    /*
     * The test is valid when the difference between result
     * and expected is less than or equal to variance
     */
    double delta = fabs(result - expected);

    if (delta > variance)
    {
        Fail("log10(%g) returned %20.17g when it should have returned %20.17g",
             value, result, expected);
    }
}

/**
 * validate
 *
 * test validation function for values returning NaN
 */
void __cdecl validate_isnan(double value)
{
    double result = log10(value);

    if (!_isnan(result))
    {
        Fail("log10(%g) returned %20.17g when it should have returned %20.17g",
             value, result, PAL_NAN);
    }
}

/**
 * main
 * 
 * executable entry point
 */
int __cdecl main(int argc, char **argv)
{
    struct test tests[] = 
    {
        /* value                       expected               variance */
        {  0,                          PAL_NEGINF,            0 },
        {  0.00072178415907472774,    -3.1415926535897932,    PAL_EPSILON * 10 },   // expected: -(pi)
        {  0.0019130141022243176,     -2.7182818284590452,    PAL_EPSILON * 10 },   // expected: -(e)
        {  0.0049821282964407206,     -2.3025850929940457,    PAL_EPSILON * 10 },   // expected: -(ln(10))
        {  0.026866041001136132,      -1.5707963267948966,    PAL_EPSILON * 10 },   // expected: -(pi / 2)
        {  0.036083192820787210,      -1.4426950408889634,    PAL_EPSILON * 10 },   // expected: -(log2(e))
        {  0.038528884700322026,      -1.4142135623730950,    PAL_EPSILON * 10 },   // expected: -(sqrt(2))
        {  0.074408205860642723,      -1.1283791670955126,    PAL_EPSILON * 10 },   // expected: -(2 / sqrt(pi))
        {  0.1,                       -1,                     PAL_EPSILON * 10 },   // expected: -(1)
        {  0.16390863613957665,       -0.78539816339744831,   PAL_EPSILON },        // expected: -(pi / 4)
        {  0.19628775993505562,       -0.70710678118654752,   PAL_EPSILON },        // expected: -(1 / sqrt(2))
        {  0.20269956628651730,       -0.69314718055994531,   PAL_EPSILON },        // expected: -(ln(2))
        {  0.23087676451600055,       -0.63661977236758134,   PAL_EPSILON },        // expected: -(2 / pi)
        {  0.36787944117144232,       -0.43429448190325183,   PAL_EPSILON },        // expected: -(log10(e))
        {  0.48049637305186868,       -0.31830988618379067,   PAL_EPSILON },        // expected: -(1 / pi)
        {  1,                          0,                     PAL_EPSILON },
        {  2.0811811619898573,         0.31830988618379067,   PAL_EPSILON },        // expected:  1 / pi
        {  2.7182818284590452,         0.43429448190325183,   PAL_EPSILON },        // expected:  log10(e)           value: e
        {  4.3313150290214525,         0.63661977236758134,   PAL_EPSILON },        // expected:  2 / pi
        {  4.9334096679145963,         0.69314718055994531,   PAL_EPSILON },        // expected:  ln(2)
        {  5.0945611704512962,         0.70710678118654752,   PAL_EPSILON },        // expected:  1 / sqrt(2)
        {  6.1009598002416937,         0.78539816339744831,   PAL_EPSILON },        // expected:  pi / 4
        {  10,                         1,                     PAL_EPSILON * 10 },
        {  13.439377934644400,         1.1283791670955126,    PAL_EPSILON * 10 },   // expected:  2 / sqrt(pi)
        {  25.954553519470081,         1.4142135623730950,    PAL_EPSILON * 10 },   // expected:  sqrt(2)
        {  27.713733786437790,         1.4426950408889634,    PAL_EPSILON * 10 },   // expected:  log2(e)
        {  37.221710484165167,         1.5707963267948966,    PAL_EPSILON * 10 },   // expected:  pi / 2
        {  200.71743249053009,         2.3025850929940457,    PAL_EPSILON * 10 },   // expected:  ln(10)
        {  522.73529967043665,         2.7182818284590452,    PAL_EPSILON * 10 },   // expected:  e
        {  1385.4557313670111,         3.1415926535897932,    PAL_EPSILON * 10 },   // expected:  pi
        {  PAL_POSINF,                 PAL_POSINF,            0 },
    };


    if (PAL_Initialize(argc, argv) != 0)
    {
        return FAIL;
    }

    for (int i = 0; i < (sizeof(tests) / sizeof(struct test)); i++)
    {
        validate(tests[i].value, tests[i].expected, tests[i].variance);
    }
    
    validate_isnan(PAL_NEGINF);
    validate_isnan(PAL_NAN);

    PAL_Terminate();
    return PASS;
}
