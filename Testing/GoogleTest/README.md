https://google.github.io/googletest/primer.html

**It supports any kinds of tests, not just unit tests.**

## Terminology

### GTest's "Test Case" = actually a "Test Suite", i.e a collection of test functions
GTest is deprecating the term "Test Case" in favor of "Test Suite". A test suite is a collection of test functions that are grouped together. Each test function within a test suite can be thought of as a separate test case.

A test program can contain multiple test suites.

### GTest's "Test" = actually a single test function

### Assertions

GoogleTest assertions are macros that resemble function calls. You test a class or function by making assertions about its behavior. When an assertion fails, GoogleTest prints the assertion’s source file and line number location, along with a failure message. You may also supply a custom failure message which will be appended to GoogleTest’s message.

#### ASSERT_EQ (Fatal) vs EXPECT_EQ (non fatal)

`ASSERT_*` versions generate fatal failures when they fail, and abort the current function.`EXPECT_*` versions generate nonfatal failures, which don’t abort the current function. 


Usually `EXPECT_*` are preferred, as they allow more than one failure to be reported in a test. However, you should use `ASSERT_*` if it doesn’t make sense to continue when the assertion in question fails

## Test structure

```cpp
TEST(TestSuiteName, TestName) {
  // ... test body with expectations and assertions...
}
```
e.g.
```cpp
int Factorial(int n);  // Returns the factorial of n

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
  EXPECT_EQ(Factorial(8), 40320);
}
```
