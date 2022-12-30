#ifndef FSL_MEMORY_H
#define FSL_MEMORY_H

#include "cstddef.h"    // ptrdiff_t
#include "type_traits.h"

namespace fsl
{
    // pointer traits
    template <typename Ptr>
    struct pointer_traits;

    template <typename T>
    struct pointer_traits<T*>
    {
        using pointer = T*;
        using element_type = T;
        using different_type = ptrdiff_t;

        template <typename U>
        using rebind = U*;

        // pointer_to for this specialization is specified to be constexpr in C++20, which sort of requires constexpr addressof, which sort of requires compiler support
        // >:)
        // might have to start thinking about how to specify the language level to target
        static constexpr pointer pointer_to(element_type& r) noexcept = delete;
    };

    // pointer conversion (added in 20?)
    // pointer safety (removed in 23)


    // pointer alignment


    // allocator argument tag
    struct allocator_arg_t { explicit allocator_arg_t() = default; };
    inline constexpr allocator_arg_t allocator_arg{};


    // uses_allocator
    namespace details
    {
        template <typename T, typename SFINAE = void>
        struct has_allocator_type : false_type {};
        template <typename T>
        struct has_allocator_type<T, void_t<typename T::allocator_type> > : true_type {};
        template <typename T>
        inline constexpr bool has_allocator_type_v = has_allocator_type<T>::value;

        template <typename T, typename Alloc, typename SFINAE = void>
        struct uses_allocator_helper
        {
            using type = false_type;
        };

        template <typename T, typename Alloc>
        struct uses_allocator_helper<T, Alloc, enable_if_t<has_allocator_type_v<T> && is_convertible_v<Alloc, typename T::allocator_type> > >
        {
            using type = true_type;
        };
    }
    template <typename T, typename Alloc>
    struct uses_allocator : details::uses_allocator_helper<T, Alloc>::type {};
    template <typename T, typename Alloc>
    inline constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;


    // allocator_traits
    template <typename Alloc>
    struct allocator_traits;

}   // namespace fsl

#endif
