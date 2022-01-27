

`Test` means `Test case`.
`Test suite` means collections of test cases.

### Basic concepts

When using googletest, you start by writing assertions, which are statements that check whether a condition is true. 
An assertion’s result can be `success`, `nonfatal failure`, or `fatal failure`. 
If a fatal failure occurs, it aborts the current function; otherwise the program continues normally.

A `test suite` contains one or many tests. 
You should group your tests into test suites that reflect the structure of the tested code.
When multiple tests in a test suite need to share common objects and subroutines, you can put them into a test fixture class

A `test program` can contain multiple test suites.

### Assertions

googletest assertions are macros that resemble function calls.

* `ASSERT` for fatal failure - `ASSERT_*` versions generate fatal failures when they fail, and abort the current function
* `EXPECT_*` versions generate nonfatal failures, which don’t abort the current function

Custom error message:
To provide a custom failure message, simply stream it into the macro using the << operator or a sequence of such operators.

```cpp
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```

### create a simple test

To create a test:

Use the `TEST()` macro to define and name a test function. These are ordinary C++ functions that don’t return a value.
In this function, along with any valid C++ statements you want to include, use the various googletest assertions to check values.
The test’s result is determined by the assertions; if any assertion in the test fails (either fatally or non-fatally), or if the test crashes, the entire test fails. Otherwise, it succeeds.

```cpp
TEST(TestSuiteName, TestName) {// Both names must be valid C++ identifiers, and they should not contain any underscores (_)
  ... test body ...
}
```
e.g.
```cpp
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

### Test Fixtures: Using the Same Data Configuration for Multiple Tests

Test Fixtures: 
If you find yourself writing two or more tests that operate on similar data, you can use a test fixture. This allows you to reuse the same configuration of objects for several different tests.

For each test defined with TEST_F(), googletest will create a fresh test fixture at runtime, immediately initialize it via SetUp(), run the test, clean up by calling TearDown(), and then delete the test fixture.

**Note** - googletest always deletes a test fixture before it creates the next one. googletest does not reuse the same test fixture for multiple tests. Any changes one test makes to the fixture do not affect other tests.


```
TEST_F(TestFixtureName, TestName) {
  ... test body ...
}
```

When these tests run, the following happens:

1. googletest constructs a `QueueTest` object (let’s call it t1).
2. `t1.SetUp()` initializes `t1`.
3. The first test (`IsEmptyInitially`) runs on t1.
4. `t1.TearDown()` cleans up after the test finishes.
5. `t1` is destructed.
6. The above steps are repeated on another `QueueTest` object, this time running the `DequeueWorks` test.


```cpp
template <typename E>  // E is the element type.
class Queue {
 public:
  Queue();
  void Enqueue(const E& element);
  E* Dequeue();  // Returns NULL if the queue is empty.
  size_t size() const;
  ...
};

class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
     q1_.Enqueue(1);
     q2_.Enqueue(2);
     q2_.Enqueue(3);
  }

  // void TearDown() override {}

  Queue<int> q0_;
  Queue<int> q1_;
  Queue<int> q2_;
};

TEST_F(QueueTest, IsEmptyInitially) {
  EXPECT_EQ(q0_.size(), 0);
}

TEST_F(QueueTest, DequeueWorks) {
  int* n = q0_.Dequeue();
  EXPECT_EQ(n, nullptr);

  n = q1_.Dequeue();
  ASSERT_NE(n, nullptr);
  EXPECT_EQ(*n, 1);
  EXPECT_EQ(q1_.size(), 0);
  delete n;

  n = q2_.Dequeue();
  ASSERT_NE(n, nullptr);
  EXPECT_EQ(*n, 2);
  EXPECT_EQ(q2_.size(), 1);
  delete n;
}

```

### Test invocation

1. The `::testing::InitGoogleTest()` function parses the command line for googletest flags.
You must call this function before calling RUN_ALL_TESTS(), or the flags won’t be properly initialized.

2. After defining your tests, you can run them with `RUN_ALL_TESTS()`, 
which returns 0 if all the tests are successful, or 1 otherwise.

you should call RUN_ALL_TESTS() only once.

If you write your own `main` function, it should return the value of `RUN_ALL_TESTS()`.

```cpp
#include "this/package/foo.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
     // You can do set-up work for each test here.
  }

  ~FooTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, MethodBarDoesAbc) {
  const std::string input_filepath = "this/package/testdata/myinputfile.dat";
  const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
  Foo f;
  EXPECT_EQ(f.Bar(input_filepath, output_filepath), 0);
}

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```
