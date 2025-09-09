# `std::optional<T&>`


## Optional Over References

Steve Downey

Bloomberg

© 2025 Bloomberg Finance L.P. All rights reserved.


# Colophon

-   Slideware: [reveal.js](https://revealjs.com/)
-   Slide Preperation: [org-re-reveal](https://gitlab.com/oer/org-re-reveal)
-   Fonts: [Atkinson Hyperlegible](https://www.brailleinstitute.org/freefont/) Next and Mono
-   Color Themes: [Modus](https://github.com/protesilaos/modus-themes) Vivendi and Operandi Tinted

Intended to conform to [Web Content Accessibility Guidelines Level AAA](https://www.w3.org/WAI/WCAG2AAA-Conformance)

<div class="notes" id="org406fcf0">
<p>
Please try to be considerate when making presentations. Accessibility helps everyone.
</p>

<p>
Try to present working  code, even in slideware.
</p>

</div>


# Standardising Optionals over References

Optionals were first proposed for C++ in 2005.

Optional<T>, where T is constrained not to be a reference, was added in 2017.

Optionals for lvalue references were voted in at the Sofia meeting in June, 2025

<div class="notes" id="org9e1c762">
<p>

</p>

<p>
This talk will discuss the early history, starting with Boost.Optional and <a href="#citeproc_bib_item_1">“N1878: A Proposal to Add an Utility Class to Represent Optional Objects (Revision 1)”</a>, and what the early concerns were for the reference specialization.  <a href="#citeproc_bib_item_3">“P1175R0: A Simple and Practical Optional Reference for c++”</a> ,  reproposed reference support for C++20, which was not adopted. <a href="#citeproc_bib_item_4">“P1683R0: References for Standard Library Vocabulary Types - an Optional Case Study”</a>,  in 2020 surveyed existing behavior of optional references in the wild, and pointed out the trap of assingment behaviour being state dependent. <a href="#citeproc_bib_item_2">“P2988R0: Std:Optional”</a> picked up the torch again in 2023, of which revision 9 is the proposal which is design approved by the Library Evolution Working Group, and R13 was adopted.
</p>

<p>
In 2024, the proposal to make optional a range, <a href="#citeproc_bib_item_5">“P3168R0: Give Std:Optional Range Support”</a>, as opposed to having a separate range of zero or one, was adopted. The reference implementation for <code>optional&lt;T&amp;&gt;</code> and the test cases for <code>views::maybe</code> were used to vet the additional interfaces for optional range support. This merged implementation became one of the first Beman libraries, where the library and the optional reference proposal  benefited immensly from the visibility and feedback.
</p>

</div>


## Why so long?

-   What were the concerns that made the process take so long?
-   How were concerns addressed?
-   What did we end up with?
-   What remains to be done?

<div class="notes" id="orgfb00ca7">
<p>

</p>

<p>
The core of the difficulty has been that references are not values  and  types containing a reference do not have value semantics. References do not fit comfortably in the C++ type system. The core value semantic type that also has reference semantics is a pointer, but pointers have underconstrained and unsafe semantics. The long discussion has been a proxy for what reference semantic types should look like in value semantic types in the standard library, particularly for "sum" types, like <code>expected</code> and <code>variant</code>.
</p>

</div>


# Quick overview of optional<T>

-   An owning type,
-   with value semantics,
-   with one additional out-of-band value.

<div class="notes" id="org83d65bc">

</div>


## Algebraically

`T + 1`

`std::variant<T, std::monostate>`

<div class="notes" id="org1fc4ab4">

</div>


## Core Use Case

```cpp
    int size;
    if (optional<int> s = readConfigValue("Size")) {
        size = *s;
    } else {
        size = 0;
    }
    return size;
```


### C++26 Range Version

```cpp
    int size = 0;
    for (int s : readConfigValue("Size")) {
        size = s;
    }
    return size;
```


## Default Optional Parameter

```cpp
constexpr int optParam(int a, optional<int> b = {}) {
    if (b) {
        return a + *b;
    }
    return a;
}
```

```cpp
    const auto t1 = optParam(3);
    const auto t2 = optParam(3, 4);
    static_assert(t1 != t2);
```

<div class="notes" id="orgc81c4e0">

</div>


# Quick overview of optional<T&>

-   A non-owning type,
-   with reference and value semantics,
-   with one additional value representing the empty state.

<div class="notes" id="orgb14f90f">

</div>


## Core Use Case

Looking up something for modification.


### Today

```cpp
    auto i = map.find("one");
    auto j = map.find("two");
    auto k = map.find("three");
    assert(i->second == 1);
    assert(j->second == 2);
    assert(k == map.end());
```

<div class="notes" id="orge485750">

</div>


### With optional<int&>

```cpp
    optional<int&> i = findInMap("one");
    optional<int&> j = findInMap("two");
    optional<int&> k = findInMap("three");
    EXPECT_TRUE(*i == 1);
    EXPECT_TRUE(*j == 2);
    EXPECT_TRUE(!k);
    *(findInMap("one")) = 3;
    EXPECT_TRUE(*i == 3);
```

<div class="notes" id="orgcdcfd44">

</div>


## Optional Reference Parameter

Instead of a pointer:

```cpp
void doSomething(std::string const& data,
                 optional<Logger&> logger = {}) {
    for (auto l : logger) {
        l.log(data);
    }
    return;
}
```

<div class="notes" id="orgdda5f21">

</div>


# Existing Problems with Optional

Construction and assignment from the underlying T or a type convertable to a T produces inevitable surprises and deep complication in the implementation.

Reasoning about overload sets is difficult for humans.

<div class="notes" id="org0222a48">

</div>


# The Design Problems for References


## Assign or Rebind?

```cpp
Cat fynn;
Cat loki;
optional<Cat&> maybeCat1;
optional<Cat&> maybeCat2{fynn};
maybeCat1 = fynn;
maybeCat2 = loki;
```

What do those assignments do?

Ought they be allowed?

State independence won out, eventually.

**ALWAYS** rebind.

<div class="notes" id="org490572e">
<p>
What assignment does is not dependent on the state of the optional.
</p>

<p>
It always rebinds the "reference", which is not possible with a C++ reference in a struct.
</p>

</div>


## Non-generic template

`optional<T&>` violates genericity.

The "`vector<bool>`" problem only for an entire value category.

Reference categories are weird and non-generic.

<div class="notes" id="orgc48fcc2">

</div>


# Design Choices


## `make_optional()`

`make_optional()` was largely supplanted by CTAD.

`make_optional<T&>()` creates an `optional<T>`.

Doing otherwise would have been worse.

<div class="notes" id="org669d7a6">

</div>


## Value Category's Affect on `optional<T&>::value() &&`

What should `optional<T&>::value() &&;` return?

Choose to model pointers, a reference semantic value type.

The value category of the object does not affect value category of the referent.

Otherwise, an rvalue `optional<T&>` could enable moves from the referent.

<div class="notes" id="org4383194">

</div>


## Shallow vs. Deep `const`

What should `optional<T&>::value() const;` return?

Choose to model pointers, a reference semantic value type.

A `const` pointer is not a pointer to `const`.

All langauge references are `const`.

An `optional<T&>` is a reference semantic type.

Not a reference.

<div class="notes" id="org060894d">

</div>


## Conditional Explicit

Is a spelled out `optional<T&>(x)` required to construct an `optional<T&>`?

Or can an `optional<T&>` be constructed implicitly from a `t`?

I would have preferred requiring explicit, but it was too painful in practice.

Lack of `explicit` makes the type exponentially more complex.

There are more interactions between member functions.

<div class="notes" id="org42839f4">
<p>
However, lack of explicit makes the type exponentially more complex, as there are more interactions between member functions.
</p>

</div>


## `value_or()`

What should `optional<T&>::value_or(U &&u);` return?

What is the "value type" for an optional?

All choices are surprising to someone.

Chose to return T, as that seems least dangerous.

It is what `optional<T>` does.

<div class="notes" id="org50f3a5e">
<p>
Future work: generic <code class="src src-cpp"><span class="org-variable-use">nullable</span></code> functions.
</p>

</div>


## `in_place_t` construction

There is no "place" to construct in to.

<div class="notes" id="org00d0e84">

</div>


## Converting assignment

Avoid conversions that produce temporaries.

Avoid confusion with `optional<U&>` or `optional<T>` constructors.

Large *overload sets* are difficult to reason about.

<div class="notes" id="org8b5f5e8">

</div>


# Reification Principles


## Construction from temporary

Avoid taking references to temporaries.

Rules out some safe cases, disallows many dangerous cases.

<div class="notes" id="org3fe88cd">

</div>


## Deleting dangling overloads

Delete, rather than remove via `concept`, function overloads that produce dangling references.

<div class="notes" id="orgfc46076">

</div>


## Assignment of `optional<T&>`

Assignment of an optional<T&> is equivalent to a pointer copy.

All assignments are through the single function.

<div class="notes" id="org96ea5d5">

</div>


# Project Beman


## Began last year at C++Now 2024

Not a requirement for Standardization.

Details matter.

<div class="notes" id="org7c9daed">
<p>
LEWG is getting better at asking for implementation of exact proposal.
</p>

</div>


## Pre-existing smd::optional

Confirmed at Tokyo, live, that the range-ification would work for my test cases for `views::maybe`.

Unfortunately `smd::optional` used early-Modern CMake.

This meant rework to bring it to current standards.

<div class="notes" id="org1847a0c">

</div>


## The ref-stealing bug found

```cpp
Cat fynn;
std::optional<Cat&> maybeCatRef{fynn};
std::optional<Cat> maybeCat;
maybeCat = std::move(maybeCatRef);
// fynn is moved from
```

Now fixed.


### The fix

Don't move the result of operator\*, move the rhs and apply operator\*().

```cpp
//instead of
*std::move(rhs)
// use
std::move(*rhs)
```

Because

```cpp
std::optional<T&>::operator*() && -> T&; // overload not actually present
```

does not return an rvalue reference.

<div class="notes" id="org0f1e3ed">
<p>

</p>

</div>


# The Modal Use-Case

The expected most common use is for looking up something and failure is not exceptional.

```cpp
constexpr optional<mapped_type&> get(const key_type& k);
```

We plan to add this to associative containers for C++29.


## P3091 by Pablo Halpern

Better Lookups for `map`, `unordered_map`, and `flat_map`


# Questions?

Remember a question starts with:

-   who
-   what
-   when
-   where
-   how
-   why

or

-   **A propositional statement:** a statement that has a truth value, either true or false, but not both.

and goes up at the end.

> "More of a comment than a question &#x2026;"

Is a propositional statement, but hold them for a moment.


# Questions?


# Comments?


# Thank You!


# Bibliography

<style>.csl-entry{text-indent: -1.5em; margin-left: 1.5em;}</style><div class="csl-bib-body">
  <div class="csl-entry"><a id="citeproc_bib_item_1"></a>Cacciola, Fernando. 2005. “N1878: A Proposal to Add an Utility Class to Represent Optional Objects (Revision 1).” https://wg21.link/n1878; WG21.</div>
  <div class="csl-entry"><a id="citeproc_bib_item_2"></a>Downey, Steve. 2023. “P2988R0: Std:Optional.” https://wg21.link/p2988r0; WG21.</div>
  <div class="csl-entry"><a id="citeproc_bib_item_3"></a>Meneide, JeanHeyd. 2018. “P1175R0: A Simple and Practical Optional Reference for c++.” https://wg21.link/p1175r0; WG21.</div>
  <div class="csl-entry"><a id="citeproc_bib_item_4"></a>———. 2020. “P1683R0: References for Standard Library Vocabulary Types - an Optional Case Study.” https://wg21.link/p1683r0; WG21.</div>
  <div class="csl-entry"><a id="citeproc_bib_item_5"></a>Sankel, David, Marco Foco, Darius Neațu, and Barry Revzin. 2024. “P3168R0: Give Std:Optional Range Support.” https://wg21.link/p3168r0; WG21.</div>
</div>
