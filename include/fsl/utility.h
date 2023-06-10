#ifndef FSL_UTILITY_H
#define FSL_UTILITY_H

#include "cstddef.h"
//#include "type_traits.h"

namespace fsl
{

    template <typename T>
    struct type_identity { using type = T; };

    namespace details
    {
        template <typename T>
        typename type_identity<T&&>::type try_add_rvalue_reference(int);
        template <typename T>
        typename type_identity<T>::type try_add_rvalue_reference(...);
    }
    template <typename T>
    struct add_rvalue_reference
    {
        using type = decltype(details::try_add_rvalue_reference<T>(0));
    };
    template <typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    template <typename T>
    add_rvalue_reference_t<T> declval() noexcept;


    // circular dependency on remove_reference_t...
    //template <typename T>
    //constexpr T&& forward(remove_reference_t<T>& t) noexcept { return static_cast<T&&>(t); }
    //template <typename T>
    //constexpr T&& forward(remove_reference_t<T>&& t) noexcept { return static_cast<T&&>(t); }


    template <typename T, T ... Ints>
    class integer_sequence
    {
    public:
        using value_type = T;
        static constexpr size_t size() noexcept { return sizeof...(Ints); }
    };

    template <size_t ... Ints>
    using index_sequence = integer_sequence<size_t, Ints...>;

    namespace details
    {
        template <typename T, T ... Ints>
        struct sequence_builder;

        template <typename T, T First, T ... Ints>
        struct sequence_builder<T, First, Ints...>
        {
            // cannot use conditional_t due to circular include
            // -> restructure to allow it or use a workaround...
            //using type = conditional_t<(First == 0), integer_sequence<T, First, Ints...>, typename sequence_builder<T, First - 1, First, Ints...>::type>;
            static auto helper()
            {
                if constexpr (First == 0)
                    return integer_sequence<T, First, Ints...>{};
                else
                    return typename sequence_builder<T, First - 1, First, Ints...>::type{};
            }

            using type = decltype(helper());
        };
    }
    template <typename T, size_t N>
    using make_integer_sequence = typename details::sequence_builder<T, N - 1>::type;
    template <size_t N>
    using make_index_sequence = make_integer_sequence<size_t, N>;
    template <typename ... Ts>
    using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

}   // namespace fsl

#endif
