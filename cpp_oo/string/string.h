#ifndef __STRING_GUARD__
#define __STRING_GUARD__

#include <cstring>
#include <string>

class String
{
    public:
        String(const char *cstr = 0);
        String(const String &str); // copy construction
        String& operator=(const String &str); // copy assignment
        ~String();
        char* get_c_str()   const { return m_data; }
    private:
        char *m_data;
};

inline String::String(const String &str)
{
    if(str.m_data == NULL)
    {
        m_data = new char[1];
        *m_data = '\0';
    }
    else 
    {
        m_data = new char[strlen(str.m_data) + 1];
        strcpy(m_data, str.m_data);
    }
    std::cout << "construction_copy " << std::endl;
}

inline String::String(const char *cstr) // 这里不可以再制定默认值了，why？默认值之能在声明 or 定义执行，不能都指定
{
    if(cstr) {  
        m_data = new char[strlen(cstr) + 1];
        strcpy(m_data, cstr);
    }
    else {
        m_data = new char[1];
        *m_data = '\0';
    }
    std::cout << "construction " << std::endl;
}

inline String::~String()
{
    if(m_data != NULL) 
    {
        std::cout << "~delete" << ':';
        std::cout << m_data << std::endl;
        delete [] m_data;
    }
    else 
    {
        std::cout << "~delete -- nodata" << std::endl;
    }
}

inline String& String::operator=(const String& str)
{
    if(this == &str)
        return *this;
    if(this->m_data != NULL)
        delete[] this->m_data;
    this->m_data = new char[strlen(str.m_data) + 1];
    strcpy(this->m_data, str.m_data);
    std::cout << "assignment copy " << std::endl;
    return *this;
}

#endif 