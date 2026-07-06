#include <iostream>
using namespace std;

// just returns whichever one is bigger
template <typename T>
T max_two(T a, T b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

// same thing but for 3 values, just compare one at a time
template <typename T>
T max_three(T a, T b, T c) {
	T biggest = a;

	if (b > biggest) {
		biggest = b;
	}
	if (c > biggest) {
		biggest = c;
	}

	return biggest;
}

// tried naming this just "swap" first but it gave an ambiguous call error
// (guessing bc it conflicts with the std swap, same thing that happened with transform in lecture)
// so renamed it to my_swap and it worked fine
// needs references or it wont actually change the originals
template <typename T>
void my_swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

int main() {

	cout << max_two(5, 9) << endl;
	cout << max_two(20, 3) << endl;
	cout << max_two(3.2, 1.7) << endl;

	cout << max_three(4, 10, 7) << endl;
	cout << max_three(2.5, 9.9, 6.1) << endl;

	int x = 10;
	int y = 20;
	my_swap(x, y);
	cout << x << " " << y << endl;

	double d1 = 1.1;
	double d2 = 9.9;
	my_swap(d1, d2);
	cout << d1 << " " << d2 << endl;

}