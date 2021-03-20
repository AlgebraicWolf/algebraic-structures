#ifndef _FUNCTOR_HPP_
#define _FUNCTOR_HPP_

#include <concepts>
#include <functional>

// An implementation of functors
// Functors are mappings between categories.
// In a practical sense, functors are types that allow us to lift functions with fmap:
// fmap: (a -> b) -> F a -> F b

// An instance of a functor is a vector with fmap being the function that applies the function
// to the each element of the container
// for f: U -> V it yields fmap f: vector<U> -> vector<V> with semantics
// (fmap f)([a1, a2, ..., an]) = [f a1, f a2, ..., f an]

// TODO think of the way to neatly interact with template classes via concepts
// TODO can a functor class be parametrized by more than one template argument? (it is high time
// for a variadic template hell i guess)

// Use SFINAE to check for presense of template-parametrized function fmap
// We want to check whether or not class has an fmap function with the signature
// fmap : (a -> b) -> F a -> F b
// Or, in C++ notation
// std::function<F<a>(F<b>)> F<b>::fmap(std::function<a(b)>);

template <template <typename> typename F>
struct has_fmap {
   private:
    struct dummy1 {
    };
    struct dummy2 {
    };

    template <typename C, typename P>
    static auto test(P *p) -> decltype(C::fmap(*p));

    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<F<dummy1>, std::function<dummy2(dummy1)>>(nullptr)) type;

   public:
    static constexpr bool value = std::is_same<type, std::function<F<dummy2>(F<dummy1>)>>::value;
};

// F is a functor iff it has fmap
template <template <typename> class F>
concept Functor = has_fmap<F>::value;

template <typename U, typename V, template <typename> typename F>
std::function<F<V>(F<U>)> fmap(std::function<V(U)> f) requires Functor<F> {
    return F<U>::fmap(f);
}

// Simple wrap class for testing functor functionality
template <typename T>
class Wrap {
   public:
    Wrap(T val) : val(val) {}

    operator T() {
        return val;
    }

    template <typename C>
    static std::function<Wrap<C>(Wrap<T>)> fmap(std::function<C(T)> f) {  // fmap impl
        return [f](Wrap<T> x) { return Wrap<C>(f(x)); };
    }

   private:
    T val;
};

#endif