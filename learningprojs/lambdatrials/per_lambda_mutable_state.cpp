

#include<iostream>

int main() {

  auto counter = [ctr=0]() mutable { ctr = ctr+1; return ctr; };

  std::cout << "ctr = " << counter() << std::endl;
  std::cout << "ctr = " << counter() << std::endl;

  return 0;
}
