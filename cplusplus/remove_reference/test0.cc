#include <iostream>

//Implementation of std::remove_reference<T>
template <typename T>
class remove_reference
{
    public:
        typedef T type;
};

template<typename T>
class remove_reference<T&>
{
    public:
        typedef T type;
};

template<typename T>
class remove_reference<T&&>
{
    public:
        typedef T type;
};

int main()
{
    int a[] = {1, 2, 3};
    //decltype(*a) b = a[0];
    int& b = a[0];
    remove_reference<int&>::type c = a[0]; //just like int c = a[0];
    remove_reference<int&&>::type d = a[0]; //just like int d = a[0];
    a[0] = 4;
    
    std::cout << b << std::endl; //output: 4
    std::cout << c << std::endl; //output: 1 --- remove reference int&
    std::cout << d << std::endl; //output: 1 --- remove reference int&&
    return 0;
}
