#ifndef __COMPLEX__
#define __COMPLEX__

#include <cmath>
#include <iostream>

// [reference](https://www.cnblogs.com/henuzyx/p/9107842.html)

// forward declare class
class complex;

complex& __doapl(complex *ths, const complex &r); // pos before declare complex CLASS

// class declaration
class complex
{
    public:
        complex(double r = 0, double i = 0) : re(r), im(i)  
        {}

        complex& operator+= (const complex &);
        
        double real() const { return re; }
        double imag() const { return im; }
        double imag() { im -= 10; return im; }
        void real(double r) { re = r; }
        void imag(double i) { im = i; }

        int sum(const complex &obj) { return obj.im + obj.re; }
        
    private:
        double re, im; // real part and imaginary part
        friend complex& __doapl(complex *, const complex &); // do assignment plus left
};


// definination class
inline complex& 
__doapl(complex *ths, const complex &r)
{
    ths->re += r.re;
    ths->im += r.im;
    return *ths;
}

inline complex& 
complex::operator+=(const complex &r)
{
    return __doapl(this, r);
}


inline double imag(const complex &x)
{
    return x.imag();
}

inline double real(const complex &x)
{
    return x.real();
}

inline complex operator + (const complex &x)
{
    return x;
}

inline complex operator - (const complex &x)
{
    return complex(-real(x), -imag(x));
}

inline complex operator +(const complex &x, const complex &y)
{
    return complex(real(x) + real(y), imag(x) + imag(y));
}

inline complex operator +(double x, const complex &y)
{
    return complex(x + real(y), imag(x) + imag(y));
}

inline complex operator +(const complex &x, double y)
{
    return complex(real(x) + real(y), imag(x) + y);
}

inline bool operator == (const complex &x, const complex &y)
{
    return real(x) == real(y) && imag(x) == imag(y);
}

inline bool operator == (double x, const complex &y)
{
    return real(y) == x && imag(y) == 0;    
}

inline bool operator == (const complex &x, double y)
{
    return real(x) == y && imag(x) == 0;    
}

inline bool operator != (const complex &x, const complex &y)
{
    return real(x) != real(y) || imag(x) != imag(y);
}

inline bool operator != (double x, const complex &y)
{
    return real(y) != x || imag(y) != 0;
}
inline bool operator != (const complex &x, double y)
{
    return real(x) != y && imag(x) != 0;
}

inline complex conj(const complex &x) // 共轭复数
{
    return complex(real(x), -imag(x));
} 

#endif