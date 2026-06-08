#include <iostream>
#include <cassert>
#include <cctype>
using namespace std;

int strcmp_case_insensitive(string s1, string s2) 
{
    for (int i = 0; i < s1.length() || i < s2.length(); i++) 
    {
        char c1 = tolower(s1[i]);
        char c2 = tolower(s2[i]);
        
        if (c1 != c2) 
        {
            return c1 - c2;
        }
    }
    return 0;
}

void test()
{
    assert(strcmp_case_insensitive("Hi", "hi") == 0);
    assert(strcmp_case_insensitive("hello", "hello") == 0);
    assert(strcmp_case_insensitive("car", "plane") < 0);
    assert(strcmp_case_insensitive("plane", "car") > 0);
    assert(strcmp_case_insensitive("hi", "hello") > 0);
    
    cout << "Tests passed!" << endl;
}

int main() {
    
  test();

    return 0;
}