#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

template <typename T>
class priorityqueue {
private:
	vector<T> v;
	function<double(T)> priority_func;

public:
	priorityqueue(function<double(T)> func) {
		priority_func = func;
	}

	int size() {
		return v.size();
	}

	void push(T data) {
	v.push_back(data);
	//sort every time we push, biggest priority goes first
	sort(v.begin(), v.end(), [&](T a, T b) {
		return priority_func(a) > priority_func(b);
	});
    }

	T pop() {
		//highest priority is always at front after sort
		T result = v[0];
		v.erase(v.begin());
		return result;
	}

};

int main() {

	priorityqueue<int> pq([](int x) {
		return (double)x;
	});

	pq.push(5);
	pq.push(1);
	pq.push(10);
	pq.push(3);

	//first tried pq.size() directly in the loop condition but that breaks
	//since size changes every time we pop, same issue from the stack/queue lecture
	//so saving the size first before popping everything out
	int n = pq.size();
	for (int i = 0; i < n; i++) {
		cout << pq.pop() << endl;
	}

}