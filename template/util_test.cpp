#include <iostream>
#include <string>
#include <mutex>
#include "util_template.h"

using namespace std;
using namespace util;
int main()
{
#ifdef test_tuple
    using mytuple = MyTuple<int, float, string>;
    mytuple my(1, 2.0, "wsr");
    cout << get<0>(my) << endl;
    cout << get<1>(my) << endl;
    cout << get<2>(my) << endl;
#endif

#ifdef test_convertable
    cout << Convertable<int, int>::same << endl;
    cout << Convertable<int, long>::same << endl;
    cout << Convertable<std::basic_string<char>, std::string>::same << endl;
    cout << Convertable<std::string, MyTuple<int, bool, string, char *>>::can << endl;
    cout << Convertable<string, double>::can << endl;
    cout << Convertable<string, int>::can << endl;
    cout << Convertable<string, char *>::can << endl;
#endif

#ifdef test_supersubclass
    class A
    {
    };
    class B : public A
    {
    };
    class C
    {
    };
    cout << SUPERSUBCLASS(A, B) << endl;
    cout << SUPERSUBCLASS(A, C) << endl;
    cout << SUPERSUBCLASS(B, C) << endl;
    cout << SUPERSUBCLASS(B, A) << endl;
    cout << SUPERSUBCLASS(C, B) << endl;
    cout << SUPERSUBCLASS(C, A) << endl;
#endif
    return 0;
}