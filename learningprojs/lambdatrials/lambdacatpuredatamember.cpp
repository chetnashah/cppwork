
class Widget
{
public:
    Widget() = default;
    void doSomething() const;

private:
    int member;
};

void Widget::doSomething() const
{
    auto f = [member]()
    { return member * 2; };
    f();
}

int main()
{
    return 0;
}