#include <iostream>

class A
{
public:
    virtual void func() = 0;

private:
    int *p;
};

int main()
{
    std::cout << sizeof(A) << std::endl;
}