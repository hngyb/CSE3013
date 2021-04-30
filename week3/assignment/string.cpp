#include "string.h"
#include <iostream>
#include <cstring>
using namespace std;

Str::Str(int leng)
{
    if (leng < 0) // leng < 0 이면 에러 메시지 출력
    {
        cout << "Length of String cannot be negative!" << endl;
    }
    else
    {
        str = new char[leng + 1]; // null 문자로 인해 leng + 1을 한다.
        len = leng;
    }
}

Str::Str(char *neyong)
{
    len = strlen(neyong);
    str = new char[len + 1];
    strcpy(str, neyong);
}

Str::~Str()
{
    delete[] str;
}

int Str::length(void)
{
    return len;
}

char *Str::contents(void)
{
    return str;
}

int Str::compare(class Str &a)
{
    return strcmp(str, a.contents());
}

int Str::compare(char *a)
{
    return strcmp(str, a);
}

void Str::operator=(char *a)
{
    delete[] str;
    len = strlen(a);
    str = new char[len + 1];
    strcpy(str, a);
}

void Str::operator=(class Str &a)
{
    delete[] str;
    len = a.length();
    str = new char[len + 1];
    strcpy(str, a.contents());
}