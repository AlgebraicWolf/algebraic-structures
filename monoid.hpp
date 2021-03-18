#ifndef _MONOID_HPP_
#define _MONOID_HPP_

#include "semigroup.hpp"

// Monoid is a semigroup with an identity element

template<typename T>
concept Monoid = Semigroup<T> &&  // Monoid must be a semigroup...
                 HasIdentity<T>;  // And have an identity element (substituted by the function Unit -> Id)

// As with semigroup, for a monoid M we can consider T -> M to be a monoid type as well. The identity element of such monoid is a function that maps all
// values of type T to the identity element of the monoid

template<typename T, Monoid M>
class MonoidFunctionLift : public SemigroupFunctionLift<T, M> {
    public:
    MonoidFunctionLift(Unit) : SemigroupFunctionLift<T, M>([](T) { return M(Unit()); }) {} // Add constructor for unit type that constructs id 
    MonoidFunctionLift(std::function<M(T)> f) : SemigroupFunctionLift<T, M>(f) {} // Constructor for function
}; // Now we can lift functions into monoids to monoids themselves

template<typename T, Monoid M>
MonoidFunctionLift<T, M> operator*(MonoidFunctionLift<T, M> f, MonoidFunctionLift<T, M> g) {
    return MonoidFunctionLift<T, M>([f, g](T x) {return f(x) * g(x);}); // Appropriate lifting 
}

template<Monoid M, typename InputIt>
M mconcat(InputIt begin, InputIt end) {
    return std::accumulate(begin, end, M(Unit{}), [](M a, M b) { return a * b; });
}

#endif 