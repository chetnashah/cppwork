#include <iostream>;

int main() {
    float* f = new float[1 << 11];
    std::cout << f[1 << 4] << std::endl;
    delete[] f;
    delete[] f;
    return 0;
}