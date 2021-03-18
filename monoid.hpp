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
    MonoidFunctionLift(Unit) : SemigroupFunctionLift([](T) { return M(Unit()); }) {} // Add constructor for unit type that constructs id 
}; // Now we can lift functions into monoids to monoids themselves

#endif 