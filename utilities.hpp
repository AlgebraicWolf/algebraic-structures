#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include <concepts>

// Unit type. Used as a dummy class for identity element of algebraic structures
class Unit {
};

// Multiplication of values of presented types is supported
template <typename U, typename V, typename T>
concept Multiplicable = requires(U u, V v) {
    { u* v }
    ->std::same_as<T>;
};

template <typename T>
concept HasIdentity = requires {
    T(Unit());  // Can construct a value from an identity element
};

#endif