#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

template <typename T>
vector<T> vec_fill(T elem) {
	vector<T> v(10);
	for (int i = 0; i < 10; i++) {
		v[i] = elem + i;
	}
	return v;
}

template <typename T>
T until(T elem, function<T(T)> func, function<bool(T)> predicate) {
	while (!predicate(elem)) {
		elem = func(elem);
	}
	return elem;
}

template <typename T>
void order(T& a, T& b, T& c) {
	if (a < b) {
		T temp = a;
		a = b;
		b = temp;
	}
	if (b < c) {
		T temp = b;
		b = c;
		c = temp;
	}
	if (a < b) {
		T temp = a;
		a = b;
		b = temp;
	}
}

int main() {

	// vec_fill tests
	assert(vec_fill(5) == vector<int>({ 5,6,7,8,9,10,11,12,13,14 }));

	assert(vec_fill(1.5) == vector<double>({ 1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5 }));

	assert(vec_fill('a') == vector<char>({ 'a','b','c','d','e','f','g','h','i','j' }));

	// until tests, same logic as last week just templated now
	assert(until<int>(1, [](int x) { return x * 2; }, [](int x) { return x > 100; }) == 128);

	assert(until<double>(1.0, [](double x) { return x + 0.5; }, [](double x) { return x >= 5.0; }) == 5.0);

	assert(until<char>('a', [](char x) { return (char)(x + 1); }, [](char x) { return x == 'f'; }) == 'f');

	// order tests
	int a = 1, b = 3, c = 2;
	order(a, b, c);
	assert(a == 3 && b == 2 && c == 1);

	double d1 = 4.4, d2 = 1.1, d3 = 9.9;
	order(d1, d2, d3);
	assert(d1 == 9.9 && d2 == 4.4 && d3 == 1.1);

	string s1 = "banana", s2 = "apple", s3 = "cherry";
	order(s1, s2, s3);
	assert(s1 == "cherry" && s2 == "banana" && s3 == "apple");

	cout << "all tests passed" << endl;

	return 0;
}