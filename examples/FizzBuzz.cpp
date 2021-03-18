// Implementation of FizzBuzz via monoids together with a wrapper around them
// Allows to easily extend the functionality via adding new monoid elements
#include "../monoid.hpp"
#include <string>
#include <iostream>

// Wrap around std::string that adds identity element
class StringWrap : public std::string
{
public:
    StringWrap(Unit) : std::string("") {}
    StringWrap(const std::string &s) : std::string(s) {}
    StringWrap(const char *s) : std::string(s) {}
};

StringWrap operator*(StringWrap a, StringWrap b)
{
    if (a.length() && b.length())
        return a + StringWrap(" ") + b; // Operation on strings are concatenations that add a space between the two elements
    else if (a.length())
        return a;
    else
        return b;
}

// Function that passes number through if function combination acting upon it returned empty string
StringWrap numberPassthrough(int x, MonoidFunctionLift<int, StringWrap> f)
{
    auto result = f(x);
    if (result.length() != 0)
        return result;

    return std::to_string(x); // Pass value through
}

int main()
{
    MonoidFunctionLift<int, StringWrap> f1([](int x) { return x % 3 == 0 ? "Fizz" : ""; });
    MonoidFunctionLift<int, StringWrap> f2([](int x) { return x % 5 == 0 ? "Buzz" : ""; });
    MonoidFunctionLift<int, StringWrap> f3(Unit{}); // Unit element that does nothing for the purpose of testing 

    std::vector funcs{f1, f2, f3};

    auto f = mconcat<decltype(f1)>(funcs.begin(), funcs.end());

    int N = 0;
    std::cout << "Iterate from 1 to ";
    std::cin >> N;

    for (int i = 1; i <= N; i++)
    {
        std::cout << numberPassthrough(i, f) << "\n";
    }
}