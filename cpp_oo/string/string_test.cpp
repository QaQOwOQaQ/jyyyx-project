#include <iostream>
#include "string.h"

#define DEBUG

using namespace std;

ostream& operator <<(ostream &os, const String &str)
{
    return os << str.get_c_str(); 
}

#ifdef DEBUG
void test1()
{   
    String c1("Hello");  
    String c2(c1);
    String c3 = c1;
    String c4;
    c4 = c1;
    cout << c1 << ' ' << c2 << ' ' << c3 << ' ' << c4<< endl;
    cout << &c1 << endl;
    cout << &c2 << endl;
    cout << &c3 << endl;
    cout << &c4 << endl;
}

void test2()
{
    String s1("hello");
    String *s2 = new String("world"); 
    cout << &s1 << ' ' << s1 << ' ' << s2 << ' ' << *s2 << endl;
    delete s2;
}

void test3()
{
    String *s = new String[3];
    delete[] s;
}
#endif 

int main()
{
    cout << "-------start successful!--------" << endl;
#ifdef DEBUG
    test1();
#endif 
    cout << "-------end successful!-------" << endl;
}