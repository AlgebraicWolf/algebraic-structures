// Implementation of FizzBuzz via semigroups together with a wrapper around it
// Allows to easily extend the functionality via adding new monoid elements
#include "../semigroup.hpp"
#include <string>
#include <iostream>

std::string operator*(std::string a, std::string b)
{
    if(a.length() && b.length()) return a + " " + b; // Operation on strings are concatenations that add a space between the two elements
    else if (a.length()) return a;
    else return b;
}

// Function that passes number through if function combination acting upon it returned empty string
std::string numberPassthrough(int x, SemigroupFunctionLift<int, std::string> f)
{
    auto result = f(x);
    if (result.length() != 0)
        return result;

    return std::to_string(x); // Pass value through
}

int main()
{
    SemigroupFunctionLift<int, std::string> f1([](int x) { return x % 3 == 0 ? "Fizz" : ""; });
    SemigroupFunctionLift<int, std::string> f2([](int x) { return x % 5 == 0 ? "Buzz" : ""; });

    auto f = f1 * f2;

    int N = 0;
    std::cout << "Iterate from 1 to ";
    std::cin >> N;

    for(int i = 1; i <= N; i++) {
        std::cout << numberPassthrough(i, f) << "\n";
    }
}