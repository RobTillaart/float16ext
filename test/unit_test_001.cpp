//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2021-11-26
// PURPOSE: unit tests for the float16 data type
//          https://github.com/RobTillaart/float16
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// ----------------------------
// assertEqual(expected, actual);               // a == b
// assertNotEqual(unwanted, actual);            // a != b
// assertComparativeEquivalent(expected, actual);    // abs(a - b) == 0 or (!(a > b) && !(a < b))
// assertComparativeNotEquivalent(unwanted, actual); // abs(a - b) > 0  or ((a > b) || (a < b))
// assertLess(upperBound, actual);              // a < b
// assertMore(lowerBound, actual);              // a > b
// assertLessOrEqual(upperBound, actual);       // a <= b
// assertMoreOrEqual(lowerBound, actual);       // a >= b
// assertTrue(actual);
// assertFalse(actual);
// assertNull(actual);

// // special cases for floats
// assertEqualFloat(expected, actual, epsilon);    // fabs(a - b) <= epsilon
// assertNotEqualFloat(unwanted, actual, epsilon); // fabs(a - b) >= epsilon
// assertInfinity(actual);                         // isinf(a)
// assertNotInfinity(actual);                      // !isinf(a)
// assertNAN(arg);                                 // isnan(a)
// assertNotNAN(arg);                              // !isnan(a)


#include <ArduinoUnitTests.h>

#include "float16ext.h"


unittest_setup()
{
  fprintf(stderr, "FLOAT16EXT_LIB_VERSION: %s\n", (char*) FLOAT16EXT_LIB_VERSION);
}


unittest_teardown()
{
}


unittest(test_constructor)
{
  float16ext zero;
  assertEqualFloat(0.000, zero.toDouble(), 1e-3);
  float16ext one(1);
  assertEqualFloat(1.000, one.toDouble(), 1e-3);

  float16ext e(exp(1));
  assertEqualFloat(1.000, ( 2.7183 / e.toDouble()), 1e-3);
  float16ext pie(PI);
  assertEqualFloat(1.000, (3.142 / pie.toDouble()), 1e-3);

  float goldenRatio = 1.61803398875;
  float16ext gr(goldenRatio);
  assertEqualFloat(1.000, (goldenRatio / gr.toDouble()), 1e-3);

  float16ext minusOne(-1);
  // fprintf(stderr, "MINUSONE: %f\n", minusOne.toDouble());
  assertEqualFloat(1.000, (-1 / minusOne.toDouble()), 1e-3);

  //  TODO
  //  NAN constructor how to test
  //  float16ext nanny(1.0/0.0);
  //  assertNAN(nanny.toDouble());

  // Overflow constructor
  float16ext big(1e6);
  assertEqualFloat(131008, big.toDouble(), 1);

  float16ext bag(-1e6);
  assertEqualFloat(-131008, bag.toDouble(), 1);

  float16ext small(1e-30);
  assertEqualFloat(0.0, small.toDouble(), 1e-3);
  float16ext smaller(-1e-30);
  assertEqualFloat(0.0, smaller.toDouble(), 1e-3);
}


unittest(test_sizeof)
{
  float16ext value(1);
  float16ext arr[10];

  assertEqual(2, sizeof(value));
  assertEqual(20, sizeof(arr));
}


unittest(test_compare_equal)
{
  float16ext a(1);
  float16ext b(1);
  float16ext c(2);
  float16ext d(-1);

  assertTrue(a == a);
  assertTrue(a == b);
  assertFalse(a == c);
  assertFalse(a == d);

  fprintf(stderr, "\n");
  assertFalse(a != a);
  assertFalse(a != b);
  assertTrue(a != c);
  assertTrue(a != d);
}


unittest(test_compare_1nequal)
{
  float16ext a(1);
  float16ext b(1);
  float16ext c(2);
  float16ext d(-2);

  assertFalse(a < a);
  assertTrue(a <= b);
  assertFalse(a > b);
  assertTrue(a >= a);

  fprintf(stderr, "\n");
  assertTrue(a < c);
  assertTrue(a <= c);
  assertFalse(a > c);
  assertFalse(a >= c);

  fprintf(stderr, "\n");
  assertFalse(a < d);
  assertFalse(a <= d);
  assertTrue(a > d);
  assertTrue(a >= d);
}


unittest(test_negation)
{
  float16ext f16(123.456);
  float16ext f17(-f16);
  float16ext f18 = -f16;

  assertEqualFloat(-123.456, f17.toDouble(), 1e-1);
  assertEqualFloat(-123.456, f18.toDouble(), 1e-1);
}


unittest(test_conversion)
{
  for (int i = 0; i < 20; i++)
  {
    float f = random(60000000) * 0.001;
    float16ext f16(f);
    assertEqualFloat(1, (f / f16.toDouble()), 1e-3);
  }
}


unittest(test_toString)
{
  float16ext f16(-123.456);
  fprintf(stderr, "note the limited accuracy (~4 digits).\n");
  assertNotEqual("-123.456", f16.toString(3));
  assertEqual("-123.4", f16.toString(1));
}


unittest(test_all_values)
{
  float16ext a;
  float16ext b;

  //  test both the conversion to and from float.
  //  internal format should be equal. Except for -0 and 0.

  fprintf(stderr, "test all positive patterns\n");
  for (uint32_t x = 0x0000; x < 0x7FFF; x++)
  {
    a.setBinary(x);
    b = a.toDouble();
    if (x != b.getBinary())  //  assert would give 65K lines output!
    {
      fprintf(stderr, "fail at %d != %d\n", x, b.getBinary());
    }
  }

  fprintf(stderr, "test all negative patterns\n");
  fprintf(stderr, "only fails -0\n");
  for (uint32_t x = 0x8000; x < 0xFFFF; x++)
  {
    a.setBinary(x);
    b = a.toDouble();
    if (x != b.getBinary())
    {
      fprintf(stderr, "fail at %d != %d\n", x, b.getBinary());
    }
  }
}


unittest_main()


//  -- END OF FILE --
