// clang++ main.cpp -std=c++2b
#include <print>

[[nodiscard]] int add(int a, int b) {
	return a+b;
}

int main (int argc, char *argv[]) {
	int i = 2;
	std::print("\n{}", (i>2) ? "yes":"no");
	add(2,3);

	std::array<int, 3> arr { 9, 8, 7 };
	std::println("Array size = {}", arr.size());
	std::println("2nd element = {}", arr[1]);

	std::pair myPair { "hello", 5 };
	// destructuring
	auto [theString, theInt] { myPair };  // Decompose using structured bindings.
	std::println("theString: {}", theString);
	std::println("theInt: {}", theInt);


	return 0;
}
