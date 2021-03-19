# Algebraic structures in C++
In this repository I'm playing around with my implementations of different algebraic structures in C++.

## Structures

### Semigroup
*Semigroup* is a pair `(S, *)` consisting of a set `S` together with an associative binary operation `*: S -> S -> S`. Note that for some type `T` all functions of type `T -> S` form a semigroup with an operation induced by the operation on semigroup:
```
f, g : T -> S;
t : T;
(f * g)(t) = f(t) * g(t).
```
This functionality is implemented using `SemigroupFunctionLift` class.

### Monoid
*Monoid* is a pair `(M, *)` consisting of a set `M` together with an associative operation `*: M -> M -> M`. The pair must satisfy semigroup axioms and additionally `M` must contain a neutral element `id` such that:
```
(∀ a : M) a * id = id * a = a.
```
As with semigroups, functions to a monoid do form a monoid with the operation identical to operation on functions to semigroups. The neutral element is a constant function that maps all element to `id : M`.

When we have a container of monoids, we can apply an `mconcat` function that will collapse all the elements into one:
```
mconcat (a1, a2, ..., an) = id * a1 * a2 * ... * an.
```

## Examples

### Fizz Buzz using C++

We can solve classical Fizz Buzz problem in a neat way using monoids. 

Notice that strings together with an operation of concatenation with separator between non-empty strings form a semigroup. Additionally, empty string is a neutral element, so strings with such operation form a monoid. That means that the functions `int -> string` form monoid as well.

That means that we can implement a solution to a Fizz Buzz problem by defining each case as an instance of `MonoidFunctionLift`, throwing them into a container and applying `mconcat` on a container of such functions. After that, we need to wrap everything up into a function that would replace empty string by an original number and apply this function to all elements from 1 to 100.

Such approach allows for great extensibility &ndash; we just need to add new predicate function into a container and it would work.