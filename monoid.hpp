#ifndef _MONOID_HPP_
#define _MONOID_HPP_

#include "semigroup.hpp"

// Monoid is a semigroup with an identity element

template<typename T>
concept Monoid = Semigroup<T> &&  // Monoid must be a semigroup...
                 HasIdentity<T>;  // And have an identity element (substituted by the function Unit -> Id)

#endif 