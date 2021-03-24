#ifndef _APPLICATIVE_HPP_
#define _APPLICATIVE_HPP_

#include <functor.hpp>

// Applicative is a generalization of functor that has two additional functions:
// pure : a -> f a -- wrap value into an applicative

// + : f (a -> b) -> f a -> f b -- in a sense, function can be "unwrapped" out of a functor
// Note that since C++ does not have automatic function currying, we will instead implement the function `unwrap` that
// is defined on an applicative with a function and returns function that takes and returns an applicative

// Note that applicative must satisfy axioms:
// (pure id) + v = v
// TODO the rest

// Note that every applicative is a functor with fmap defined as:
// fmap : (a -> b) -> f a -> f b
// fmap f = \x -> (pure f) + x -- indeed, the type is (a -> b) -> f a -> f b. We just wrap function into an applicative and then apply it

// We don't really need to require the applicative to be the functor

// template<template <typename> typename A>
// concept Applicative = Functor<A>; // Applicative must be a functor

// Let's just require the bare minimum for applicative

template <template <typename> typename A>
struct has_pure {
   private:
    struct dummy {};

    template <typename C, typename P>
    static auto test(P *p) -> decltype(C::pure(*p));
    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<A<dummy>, dummy>(nullptr)) type;

   public:
    static constexpr bool value = std::is_same<type, A<dummy>>;
};

template <template <typename> typename A>
struct has_unwrap {
   private:
    struct dummy1 {};
    struct dummy2 {};

    template <typename C, typename P>
    static auto test(P *p) -> decltype(C::unwrap(*p));
    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<A<std::function<dummy2(dummy1)>>, A<dummy1>>) type;

    public:
    static constexpr bool value = std::is_same<type, A<dummy1>>::value;
};

template <template <typename> typename A>
concept Applicative = has_pure<A>::value && has_unwrap<A>::value; // Require pure and value to be implemented 

// Uncurried operator+ on Applicative:
template <typename U, typename V, template <typename> typename A>
A<U> operator+(A<std::function<U(V)>> f, A<V> val) {
    return A << std::function<U(V)> > ::unwrap(f)(val);
}

// Instead of requiring fmap implementation, we will create a helper class which we need to inherit from in order to enable functor functionality

template <template <typename> typename A>
class functor_from_applicative {
   public:
    template <typename U, typename V>
    // fmap expressed via pure and operator+
    static std::function<A<U>(A<V>)> fmap(std::function<U(V)> f) requires Applicative<A> {
        return [f](V v) { return A::pure(f) + x };
    }
};

#endif