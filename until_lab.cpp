#include <iostream>
#include <cassert>
#include <functional>
using namespace std;

int until(int x, function<int(int)> f, function<bool(int)> g) {
    while (!g(x)) {
        x = f(x);
    }
    return x;
}

int double_num(int x) {
    return x * 2;
}

bool greater_than_100(int x) {
    return x > 100;
}

int subtract3(int x) {
    return x - 3;
}

bool is_negative(int x) {
    return x < 0;
}

int main() {
    // from the assignment: 1 -> 2 -> 4 -> ... -> 128
    assert(until(1, double_num, greater_than_100) == 128);

    // subtracting: 10 -> 7 -> 4 -> 1 -> -2
    assert(until(10, subtract3, is_negative) == -2);

    assert(until(2, [](int x) { return x * x; }, [](int x) { return x > 1000; }) == 65536);

    // 0 -> 1 -> 2 -> 3 -> 4 -> 5
    assert(until(0, [](int x) { return x + 1; }, [](int x) { return x == 5; }) == 5);

    cout << "all tests passed" << endl;

    return 0;
}