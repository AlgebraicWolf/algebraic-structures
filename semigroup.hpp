#ifndef _SEMIGROUP_HPP_
#define _SEMIGROUP_HPP_

#include <functional>
#include <numeric>

#include "utilities.hpp"

// Semigroup is a pair (S, *) where S is a set and *: S -> S -> S is an _associative_ binary operation
// Since C++ does not provide us with tools to somehow guarante associativity, we just have to trust the user.

template <typename T>
concept Semigroup = Multiplicable<T, T, T>;  // Must have semigroup operation

// We can lift functions into semigroups as well, i. e. if we have a Semigroup S and functions f: T -> S,
// Then these functions also form a semigroup together with an operations acting upon functions f, g: T -> S:
// h := f * g in such a way that h : T -> S and h(a) = f(a) * g(a). Let's implement the lifting structure:

template <typename T, Semigroup S>
class SemigroupFunctionLift {
   public:
    SemigroupFunctionLift(std::function<S(T)> f) : f(f) {}  // Store the function

    S operator()(T x) const {  // Implement callability of the monoid elements
        return f(x);
    }

   private:
    std::function<S(T)> f;  // Function that is being lifted
};

// Define operation on the semigroup of functions into semigroup
template <typename T, Semigroup S>
SemigroupFunctionLift<T, S> operator*(SemigroupFunctionLift<T, S> f, SemigroupFunctionLift<T, S> g) {
    return SemigroupFunctionLift<T, S>([f, g](T x) { return f(x) * g(x); });  // Appropriate lifting
}

// Folding container of semigroup elements into single element
template <Semigroup S, class InputIt>
S fold(InputIt first, InputIt last) {
    S start = *first;
    ++first;  // First element is already taken into account
    return std::accumulate(first, last, start, operator*);
}

#endif