#ifndef _MONOID_HPP_
#define _MONOID_HPP_

#include "assert.h"
#include "semigroup.hpp"

// Monoid is a semigroup with an identity element

template <typename T>
concept Monoid = Semigroup<T>&&  // Monoid must be a semigroup...
    HasIdentity<T>;              // And have an identity element (substituted by the function Unit -> Id)

// As with semigroup, for a monoid M we can consider T -> M to be a monoid type as well. The identity element of such monoid is a function that maps all
// values of type T to the identity element of the monoid

template <typename T, Monoid M>
class MonoidFunctionLift : public SemigroupFunctionLift<T, M> {
   public:
    MonoidFunctionLift(Unit) : SemigroupFunctionLift<T, M>([](T) { return M(Unit{}); }) {}  // Add constructor for unit type that constructs id
    MonoidFunctionLift(std::function<M(T)> f) : SemigroupFunctionLift<T, M>(f) {}           // Constructor for function
};                                                                                          // Now we can lift functions into monoids to monoids themselves

// Appropriate operation on monoids of fuctions
template <typename T, Monoid M>
MonoidFunctionLift<T, M> operator*(MonoidFunctionLift<T, M> f, MonoidFunctionLift<T, M> g) {
    return MonoidFunctionLift<T, M>([f, g](T x) { return f(x) * g(x); });  // Appropriate lifting
}

// Idea: Perhaps we can implement monoids of functions on many-variable functions with the same types of the arguments?
// Nah, one can just use functions that take std::tuple

// Given a container of monoids [a1, a2, ..., an], we can apply operation on the container:
// a = id * a1 * a2 * ... * an
template <Monoid M, typename InputIt>
M mconcat(InputIt begin, InputIt end) {
    return std::accumulate(begin, end, M(Unit{}), [](M a, M b) { return a * b; });
}

// Idea: Should create a wrapper for monoid of endomorphisms on a category with a binary operation that is a composition of functions

template <typename T>
class Endo {
   public:
    Endo(std::function<T(T)> f) : f(f) {}     // Construct endomorphism from a function
    Endo(Unit) : f([](T x) { return x; }) {}  // Identity element of such

    T operator()(T x) {  // Make thing callable
        return f(x);
    }

   private:
    std::function<M(M)> T;  // Endomorphism itself
};

// Define composition on monoid Endo<T>
template <typename T>
Endo<T> operator*(Endo<T> f, Endo<T> g) {
    return Endo<T>([f.g](T x) { return f(g(x)); });
}

static_assert(Monoid<Endo<int>> == true);  // Verify that, for example, Endo<int> (functions int -> int) is indeed a monoid

#endif