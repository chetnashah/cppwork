int main() {
    float* f = new float[1 << 11];
    delete[] f;
    f[1 << 4] = 11;
    return 0;
}