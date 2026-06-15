#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

    // Assignment and Dereferencing

    int a;
    int b;

    cout << "Enter first integer: ";
    cin >> a;

    cout << "Enter second integer: ";
    cin >> b;

    int* ptr_a = &a;
    int* ptr_b = &b;

    cout << "Value of a: " << *ptr_a << endl;
    cout << "Value of b: " << *ptr_b << endl;


    // Maximum in an Array

    int arr[] = {12, 45, 7, 89, 23, 4, 56};

    int* max_ptr = arr;
    int* p = arr;

    while (p < arr + 7) 
    {

        if (*p > *max_ptr) {
            max_ptr = p;
        }

        p++;
    }

    cout << "Maximum value: " << *max_ptr << endl;


    // Length of a C-Style String

    char str[] = "Hello World";

    char* ch = str;
    int length = 0;

    while (*ch != '\0') 
    {
        length++;
        ch++;
    }

    cout << "String length: " << length << endl;


    // Reverse a C-Style String

    char str2[] = "Pointer";
    char* rev = str2;

    while (*rev != '\0') 
    {
        rev++;
    }

    rev--;

    cout << "Reverse string: ";

    
    while (rev >= str2) 
    {
        cout << *rev;
        rev--;
    }

    cout << endl;


    // Midpoint in a Vector

    srand(time(0));

    vector<int> vec;

    for (int i = 0; i < 6; i++) 
    {
        int num = rand() % 100;
        vec.push_back(num);
    }

    cout << "Vector values: ";

    
    
    int* print_ptr = &vec[0];

    while (print_ptr < &vec[0] + vec.size()) 
    {
        cout << *print_ptr << " ";
        print_ptr++;
    }

    cout << endl;

    int* left = &vec[0];
    int* right = &vec[0] + vec.size() - 1;

    while (left < right) 
    {
        left++;
        right--;
    }

    cout << "Midpoint index: " << right - &vec[0] << endl;
    cout << "Midpoint value: " << *right << endl;

    return 0;
}