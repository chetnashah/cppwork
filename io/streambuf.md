std::basic_streambuf is the low-level buffering hook for all standard and custom streams.

## usage with `stream.rdbuf()`


`rdbuf()` returns a pointer to the stream's internal buffer. This is useful for low-level I/O operations, such as reading or writing directly to the buffer.

Think of it as a **Redirect buffer** which allows you to hijack the buffer of a stream.


### Common use case - cout/cin highjacking with `rdbuf()`


```cpp
namespace HelloWorldTests
{
   TEST_CLASS(HelloWorldTests)
   {
   public:

      TEST_METHOD(TestMethod)
      {
         std::string expected = "Hello World!\n";

         std::stringstream buffer;
         std::streambuf* originalbuf = std::cout.rdbuf(); // Save cout's buffer
         std::cout.rdbuf(buffer.rdbuf()); // Redirect cout to the stringstream buffer

         // Call main() in your test
         int result = main();// main just does "std::cout << "Hello World!\n";"

         // When finished, redirect cout to the original buffer 
         std::cout.rdbuf(originalbuf);
         std::cout << "std original buffer: \n";
         std::cout << buffer.get();

         // Test
         Assert::AreEqual(expected, buffer.str());
      }
   };
}
```

