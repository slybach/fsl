#ifndef FSL_TYPE_TRAITS_H
#define FSL_TYPE_TRAITS_H

#include "fsl_config.h"
#include "cstddef.h"
#include "utility.h"    // declval, type_identity (C++20)

#ifdef FSL_PULL_STD
#include <type_traits>
namespace fsl
{
    // kinda cannot even be more specific than this, because
    // iirc implementations have the right to define classes/templates
    // differently as long as the template name behaves as expected
    // when provided with the arguments (could have more hidden
    // parameters to leverage sfinae for example)
    // I'll look up the quote for that if I don't forget to make
    // sure this is a thing
    // ended up finding this: http://eel.is/c++draft/requirements#member.functions-2
    // but no clue if there's any mention for template class and
    // their template arguments
    using namespace std;
}
#endif

namespace fsl
{
    // helper class
    template <typename T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant<T, v>;

        static constexpr T value = v;

        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    template <bool B>
    using bool_constant = integral_constant<bool, B>;
    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;


    // primary type categories
    template <typename T>
    struct is_void;
    template <typename T>
    inline constexpr bool is_void_v = is_void<T>::value;

    template <typename T>
    struct is_null_pointer;
    template <typename T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    //// impossible to implement in a portable way: cannot know
    //// what implementation-defined integer type exist without
    //// listing and checking a bunch of platforms
    //template <typename T>
    //struct is_integral;

    //// same issue as above, no way to know (without a listing)
    //// what extended floating point types there are
    //// (contrary to what cppreference seems to suggest, the
    //// trait should check the extended floating point types)
    //template <typename T>
    //struct is_floating_point;

    template <typename T>
    struct is_array;
    template <typename T>
    inline constexpr bool is_array_v = is_array<T>::value;

    template <typename T>
    struct is_pointer;
    template <typename T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    template <typename T>
    struct is_lvalue_reference;
    template <typename T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    template <typename T>
    struct is_rvalue_reference;
    template <typename T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    template <typename T>
    struct is_member_object_pointer;
    template <typename T>
    inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

    template <typename T>
    struct is_member_function_pointer;
    template <typename T>
    inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    //// none of those can really be implemented without compiler
    //// support
    //// Arguably there's a way to levarage overload resolution to
    //// implement is_class from is_union (and the other way around
    //// I guess), but we're still kinda stuck there
    //// (only class/union can have pointer to members)
    //// I have no clue about enum though, maybe if it's the last
    //// primary type category left, could be defined as "not any
    //// of the other primary type", but I'm definitely stuck with
    //// integral and floating so
    //template <typename T>
    //struct is_enum;
    //template <typename T>
    //struct is_union;
    //template <typename T>
    //struct is_class;

    template <typename T>
    struct is_function;
    template <typename T>
    inline constexpr bool is_function_v = is_function<T>::value;


    // composite type categories
    template <typename T>
    struct is_reference;
    template <typename T>
    inline constexpr bool is_reference_v = is_reference<T>::value;

    template <typename T>
    struct is_member_pointer;
    template <typename T>
    inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;


    // type properties
    template <typename T>
    struct is_const : false_type {};
    template <typename T>
    struct is_const<const T> : true_type {};
    template <typename T>
    inline constexpr bool is_const_v = is_const<T>::value;

    template <typename T>
    struct is_volatile : false_type {};
    template <typename T>
    struct is_volatile<volatile T> : true_type {};
    template <typename T>
    inline constexpr bool is_volatile_v = is_volatile<T>::value;


    // type property queries


    // type relations
    template <typename T, typename U>
    struct is_same : false_type {};
    template <typename T>
    struct is_same<T, T> : true_type {};
    template <typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    // missing here: is_base_of

    template <typename From, typename To>
    struct is_convertible;
    template <typename From, typename To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;


    // const volatile transformations
    template <typename T>
    struct remove_cv { using type = T; };
    template <typename T>
    struct remove_cv<T const> { using type = T; };
    template <typename T>
    struct remove_cv<T volatile> { using type = T; };
    template <typename T>
    struct remove_cv<T const volatile> { using type = T; };
    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <typename T>
    struct remove_const { using type = T; };
    template <typename T>
    struct remove_const<T const> { using type = T; };
    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    template <typename T>
    struct remove_volatile { using type = T; };
    template <typename T>
    struct remove_volatile<T volatile> { using type = T; };
    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;


    // reference modifications

    // add_rvalue_reference in utility for declval


    // sign modifications


    // array modifications


    // pointer modifications


    // other transformations
    template <bool B, typename T = void>
    struct enable_if {};
    template <typename T>
    struct enable_if<true, T>
    {
        using type = T;
    };
    template <bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template <bool B, typename T, typename F>
    struct conditional { using type = F; };
    template <typename T, typename F>
    struct conditional<true, T, F> { using type = T; };
    template <bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    template <typename ... Ts>
    using void_t = void;


    // logical operator traits
    namespace details
    {
        // to flatten the inheritance hierarchy of the actual conjunction type
        template <typename ...>
        struct conjunction_helper
        {
            using type = true_type;
        };
        template <typename B1>
        struct conjunction_helper<B1>
        {
            using type = B1;
        };
        template <typename B1, typename ... Bs>
        struct conjunction_helper<B1, Bs...>
        {
            using type = conditional_t<static_cast<bool>(B1::value), conjunction_helper<Bs...>, B1>;
        };
    }
    template <typename ... Bs>
    struct conjunction : details::conjunction_helper<Bs...>::type {};
    template <typename ... Bs>
    inline constexpr bool conjunction_v = conjunction<Bs...>::value;

    namespace details
    {
        // to flatten the inheritance hierarchy of the actual disjunction type
        template <typename ...>
        struct disjunction_helper
        {
            using type = false_type;
        };
        template <typename B1>
        struct disjunction_helper<B1>
        {
            using type = B1;
        };
        template <typename B1, typename ... Bs>
        struct disjunction_helper<B1, Bs...>
        {
            using type = conditional_t<static_cast<bool>(B1::value), B1, disjunction_helper<Bs...> >;
        };
    }
    template <typename ... Bs>
    struct disjunction : details::disjunction_helper<Bs...>::type {};
    template <typename ... Bs>
    inline constexpr bool disjunction_v = disjunction<Bs...>::value;

    template <typename B>
    struct negation : bool_constant<!static_cast<bool>(B::value)> {};


    // member relationships


    //------------------------------------------------------------
    // delayed implementations

    // primary type categories
    template <typename T>
    struct is_void : bool_constant<is_same_v<void, remove_cv_t<T> > > {};

    template <typename T>
    struct is_null_pointer : bool_constant<is_same_v<nullptr_t, remove_cv_t<T> > > {};

    template <typename T>
    struct is_array : false_type {};
    template <typename T>
    struct is_array<T[]> : true_type {};
    template <typename T, size_t N>
    struct is_array<T[N]> : true_type {};

    namespace details
    {
        template <typename T>
        struct is_pointer_helper : false_type {};
        template <typename T>
        struct is_pointer_helper<T*> : true_type {};
    }
    template <typename T>
    struct is_pointer : details::is_pointer_helper<remove_cv_t<T> >::type {};

    template <typename T>
    struct is_lvalue_reference : false_type {};
    template <typename T>
    struct is_lvalue_reference<T&> : true_type {};

    template <typename T>
    struct is_rvalue_reference : false_type {};
    template <typename T>
    struct is_rvalue_reference<T&&> : true_type {};

    template <typename T>
    struct is_member_object_pointer : bool_constant<is_member_pointer_v<T> && !is_member_function_pointer_v<T> > {};

    namespace details
    {
        template <typename T>
        struct is_member_function_pointer_helper
        {
            using type = false_type;
        };

        template <typename T, typename U>
        struct is_member_function_pointer_helper<U T::*>
        {
            using type = conditional_t<is_function_v<U>, true_type, false_type>;
        };
    }
    template <typename T>
    struct is_member_function_pointer : details::is_member_function_pointer_helper<T>::type {};

    // this implementation emits warnings on at least msvc (when instantiated with a function type)
    // check if there is a different, short way to achieve this without warning, or if avoiding
    // warnings == enumerating the incredibly long list of possibilities of function types
    // might just use pragmas to suppress the warnings on a couple of compilers, as unknown
    // pragmas should just be ignored or produce warnings on other compilers
    template <typename T>
    struct is_function : bool_constant<!is_const_v<const T> && !is_reference_v<T> > {};


    // composite type categories
    template <typename T>
    struct is_reference : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T> > {};

    namespace details
    {
        template <typename T>
        struct is_member_pointer_helper : false_type {};
        template <typename T, typename U>
        struct is_member_pointer_helper<U T::*> : true_type {};
    }
    template <typename T>
    struct is_member_pointer : details::is_member_pointer_helper<remove_cv_t<T> >::type {};


    // type relations
    namespace details
    {
        // is_convertible is specified to test whether the return statement in this imaginary function
        // To test()
        // {
        //     return std::declval<From>();
        // }
        // would be "well-formed". There's some wording about declval not being odr-used and whatnot,
        // but essentially if we exclude cases where To is void, this return statement would trigger
        // copy - initialization of an object of type To from an rvalue expression of type From. This
        // also happens to be what passing an rvalue expression of type From into a function that
        // takes an argument of type To by value would do, so that's what we SFINAE on (and we special
        // case void).
        template <typename T>
        false_type convertible_test(...);
        template <typename T>
        true_type convertible_test(T);
    }
    template <typename From, typename To>
    struct is_convertible : conditional_t <
        is_same_v<From, void> && is_same_v<To, void>,
        true_type,
        decltype(details::convertible_test(declval<From>()))
    > {};

}   // namespace fsl

#endif
