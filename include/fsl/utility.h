#ifndef FSL_UTILITY_H
#define FSL_UTILITY_H

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

}   // namespace fsl

#endif
