#include <iostream>
#include "complex.h"

using namespace std; // do not torment your self ^ ^


ostream& operator<<(ostream& os,const complex& x)
{
    return os << '(' << real(x) << ',' << imag(x) << ')';
}

#ifdef DEBUG
void test_sum()
{
    complex c1(100, 200);
    complex c2(32, 1024);
    int sum = c2.sum(c1);
    cout << sum << endl;
}

void test_out()
{
    complex c1(100, 200);
    complex c2(32, 1024);
    cout << c1 << endl;
    cout << c2 << endl;
}

void test_friend()
{
    complex c2(100, 200);
    complex *c3 = new complex(5, 10);
    *c3 += c2;
    cout << "addr = " << c3 << ", val = " << *c3 << endl;
}

void test_operator()
{
    complex c1(1, 2);
    complex c2;
    cout << c1 << endl;
    cout << c2 << endl;

    c2 = c1 + 5;    cout << c2 << endl;
    c2 = 7 + c1;    cout << c2 << endl;
    c2 = c1 + c2;   cout << c2 << endl;
    c2 += c1;       cout << c2 << endl;
    c2 += 3;        cout << c2 << endl;
    c2 = -c1;       cout << c2 << endl;

    cout << (c1 == c2) << endl;
    cout << (c1 != c2) << endl;
    cout << conj(c1) << endl;
}

void test_tmpObject()
{
    complex(4,5); // 临时对象
    complex(); // 临时对象
    // complex c1();  // Error!
    complex c2(1,2);
    cout << c2 << endl;
}
#endif

int main()
{
    cout << "start successful!" << endl;
#ifdef DEBUG
    test_operator();
#endif
    cout << "end successful!" << endl;
}