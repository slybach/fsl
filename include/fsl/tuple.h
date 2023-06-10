#ifndef FSL_TUPLE_H
#define FSL_TUPLE_H

//#include "fsl_config.h"
#include "cstddef.h"
#include "type_traits.h"
#include "utility.h"

namespace fsl
{

    namespace details
    {
        template <size_t Index, typename T>
        struct tup_element
        {
            using element_type = T;
            static constexpr size_t index_value = Index;

            tup_element() = default;
            //template <typename ... Ts>
            //constexpr tup_element(Ts&& ... args) : m_element(forward<Ts>(args)...) {}
            constexpr tup_element(T const& args) : m_element(args) {}

            element_type m_element;

            template <size_t I>
            constexpr enable_if_t<(I == Index), element_type&> get() { return m_element; }

            template <size_t I>
            constexpr enable_if_t<(I == Index), element_type const&> get() const { return m_element; }
        };

        template <typename T>
        struct is_tup_element : false_type {};
        template <size_t Index, typename T>
        struct is_tup_element<tup_element<Index, T> > : true_type {};
        template <typename T>
        inline constexpr bool is_tup_element_v = is_tup_element<T>::value;

        template <typename ... Ts>
        struct tuple_impl
        {
            static_assert(sizeof...(Ts) < 0, "forbidden specialization");
        };

        template <size_t ... Is, typename ... Ts>
        struct tuple_impl<tup_element<Is, Ts>...> : tup_element<Is, Ts>...
        {
            tuple_impl() = default;

            constexpr tuple_impl(Ts const& ... iArgs)
                // todo: constraint for copyability, check noexcept specification
                : tup_element<Is, Ts>(iArgs)...
            {}

            template <size_t I>
            friend constexpr decltype(auto) get(tuple_impl const& t) { return (t.template get<I>()); }

            template <size_t I>
            friend constexpr decltype(auto) get(tuple_impl& t) { return (t.template get<I>()); }
        };


        template <typename ... Ts>
        struct tuple_impl_builder
        {
            template <size_t ... Is>
            static auto helper(index_sequence<Is...>) -> tuple_impl<tup_element<Is, Ts>...>;

            using type = decltype(helper(index_sequence_for<Ts...>{}));
        };
        template <typename ... Ts>
        using tuple_impl_builder_t = typename tuple_impl_builder<Ts...>::type;
    }

    template <typename ... Ts>
    struct tuple : details::tuple_impl_builder_t<Ts...>
    {
    private:
        using base_type = details::tuple_impl_builder_t<Ts...>;
    public:
        using base_type::base_type;
    };
}

#endif
