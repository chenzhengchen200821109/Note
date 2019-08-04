# lvaule && rvalue
old C: 左值（lvalue）是出现在一个语句（statement）中左边或者右边的表达式（expression）。 
c++：左值（lvalue）是一个引用一段内存位置（memory location）的表达式（expression）。它  允许我们通过&操作符（& operator）拿到那段内存的位置。右值是一个不是左值的表达式。  

# rvalue reference  
```
If X is any type, then X&& is called an rvalue reference to X. For better distinction, the ordinary reference X& is now also called an lvalue reference.
```
右值引用的出现是为了解决两个问题：
1. 实现移动语义（move sematics）
右值引用虽然是右值，但是我们可以取得它的内存地址。因此移动语义就可以实现通过交换左值与右值引用的内存指针来实现管理资源的互换。 std::move()实际上是把它的实参变成右值。
```
An rvalue reference is a type that behaves much like the ordinary reference X&, with several exceptions. The most important one is that when it comes to function overload resolution, lvalues prefer old-style lvalue references, whereas rvalues prefer the new rvalue references。
```
```
void foo(X& x);  // lvalue reference overload
void foo(X&& x); // rvalue reference overload

X x;
X foobar();
foo(x); //argument is lvalue, calls foo(X&)
foo(foobar()); //argument is rvalue, calls foo(X&&)
```
为什么？因为
```
Things that are declared as rvalue reference can be lvalues or rvalues. The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue
```
2. 实现完美转发（perfect forwarding）  
（待续）

# move 语义和编译器优化（RVO)
假设我们有如下代码
```
X foo()
{
	X x;
	// perhaps do something to x
	return x;
}
```
你也许会想用移动语义，如下
```
X foo()
{
	X x;
	// perhaps do something to x
	return std::move(x);  //making it worse!
}
```
为什么？
因为编译器会做***RVO（return value optimization）***优化，没有必要在这里使用移动语义。  在**GCC 9**中可以使用**-Wpressimizing**编译选项识别这种情况。


# copy elision  
Copy elision(or Copy omission)是一种编译器的优化手段。
如下代码
```
#include <iostream>
using namespace std;
class A
{
	public:
		A(const char* str = "\0") //default constructor
		{
			cout << "Constructor called" << endl;
		}
		A(const A& a) // copy constructor
		{
			cout << "Copy constructor" << endl;
		}
};
int main()
{
	A obj = "copy me";
	return 0;
}
```
结果输出：
```
Constructor called
```
为什么？因为编译器做的优化。加上**-fno-elide-constructors**可以禁止此种优化。结果输出：
```
Constructor called
Copy constructor called
```

# two-stage overload resolution





# 参考
[1]: http://www.thbecker.net/articles/rvalue_references/section_01.html	"C++ Rvalue References Explained"

