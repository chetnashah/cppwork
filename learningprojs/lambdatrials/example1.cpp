
#include <stdio.h>

int g = 10;
auto kitten = [=]() { return g+1; };
auto cat = [g=g]() { return g + 1; };

int main() {
  g = 20;
  printf("%d %d", kitten(), cat());
}
