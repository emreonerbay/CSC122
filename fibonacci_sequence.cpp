#include <iostream>
using namespace std;

int fibRecursive(int n)
{
    static int callCount = 0;
    callCount++;
    cout << "called " << callCount << " times so far" << endl;

    // base cases
    if (n == 1)
        return 1;
    if (n == 2)
        return 1;

    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

int fibLoop(int n)
{
    if (n == 1 || n == 2)
        return 1;

    int prev1 = 1;
    int prev2 = 1;
    int answer = 0;
    int iterations = 0;

    for (int i = 3; i <= n; i++)
    {
        answer = prev1 + prev2;
        prev1 = prev2;
        prev2 = answer;
        iterations++;
    }

    cout << "loop ran " << iterations << " times" << endl;
    return answer;
}

int main()
{
    int num = 10;

    cout << "--- recursive ---" << endl;
    int result = fibRecursive(num);
    cout << "fib(10) = " << result << endl;

    cout << endl << "--- loop ---" << endl;
    int result2 = fibLoop(num);
    cout << "fib(10) = " << result2 << endl;

    return 0;
}