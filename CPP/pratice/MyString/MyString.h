#ifndef _MYSTRING_H_
#define _MYSTRING_H_

#include <iostream>
#include <cstring>

class MyString
{
    public:
        MyString();
        MyString(const char* str);
        MyString(const MyString& refStr);
        MyString& operator=(const char* str);
        MyString& operator=(const MyString& refStr);
        MyString(int n, char);
        virtual ~MyString();

    public:
        char& operator[](int i);
        const char& operator[](int i) const;

        MyString& operator+(MyString& refStr);
        MyString& operator+=(const MyString* refStr);
        MyString& operator+=(const char* str);

    public:
        void Print()
        {
            std::cout << mStr << std::endl;
        }

    private:
        char* mStr = NULL;
};

#endif //_MYSTRING_H_

