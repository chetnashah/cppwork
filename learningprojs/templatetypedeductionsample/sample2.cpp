template <typename T>
class TD; // Type displayer

int main()
{
    const int k = 1;
    auto a = k;
    TD<decltype(a)> td; // TD<int> - deduced type is int

    const int &kr = k;
    auto &b = kr;
    TD<decltype(b)> td2; // TD<const int&> - deduced type is const int&
}
