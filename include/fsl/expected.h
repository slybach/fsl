#ifndef FSL_EXPECTED_H
#define FSL_EXPECTED_H

#include "exception.h"
#include "type_traits.h"
#include "utility.h"

namespace fsl
{

    // pretty much verbatime what the standard specifies
    struct unexpect_t
    {
        explicit unexpect_t() = default;
    };

    inline constexpr unexpect_t unexpect {};


    template <typename E>
    class bad_expected_access;

    template <>
    class bad_expected_access<void> : public exception
    {
    protected:
        bad_expected_access() noexcept = default;
        bad_expected_access( bad_expected_access const& ) noexcept = default;
        bad_expected_access( bad_expected_access&& ) noexcept = default;
        bad_expected_access& operator=( bad_expected_access const& ) noexcept = default;
        bad_expected_access& operator=( bad_expected_access&& ) noexcept = default;
        ~bad_expected_access() = default;

    public:
        char const* what() const noexcept override { return "fsl::bad_expected_access - tried to access the value of an expected object holding an error"; }
    };

    template <typename E>
    class bad_expected_access : public bad_expected_access<void>
    {
    public:
        //explicit bad_expected_access( E error ) : unex( fsl::move( error ) ) {}
        explicit bad_expected_access( E error );    // fsl::move not yet implemented >:)

        // TODO: override what when E is formattable, when format is implemented... >:)
        // anyway .what is only specified to return an implementation-defined NTBS so leaving it to the void specialization is permissible

        E& error() & noexcept { return unex; }
        E const& error() const& noexcept { return unex; }

        // fsl::move not yet implemented >:)
        E&& error() && noexcept;
        E const&& error() const&& noexcept;

    private:
        E unex;
    };


    template <typename E>
    class unexpected;

    namespace details
    {
        template <typename T>
        struct is_unexpected_specialization : false_type {};
        template <typename E>
        struct is_unexpected_specialization<unexpected<E> > : true_type {};

        // note: incomplete!
        template <typename E>
        struct is_valid_unexpected_argument : negation<conjunction
            // todo: non-object types
            < is_array<E>
            , is_unexpected_specialization<E>
            , is_const<E>
            , is_volatile<E>
        > > {};
    }

    template <typename E>
    class unexpected
    {
        // https://eel.is/c++draft/expected#un.general-2
        static_assert( details::is_valid_unexpected_argument<E>::value, "fsl::unexpected error: cannot instantiate for non-object type, arrays, specialization of unexpected, or cv-qualified types" );

    public:

        constexpr unexpected( unexpected const& ) = default;
        constexpr unexpected( unexpected&& ) = default;

        template <typename Err = E>
        //    requires
        //    ( !is_same_v<remove_cvref_t<Err>, unexpected>
        //      && !is_same_v<remove_cvref_t<Err>, in_place_t>
        //      && is_constructible_v<E, Err> )
        //constexpr explicit unexpected( Err&& e ) : unex( forward<Err>( e ) ) {}
        constexpr explicit unexpected( Err&& e );   // fsl::forward not implemented yet >:)

        template <typename ... ArgsT>
            //requires ( constructible_v<E, ArgsT...> )
        //constexpr explicit unexpected( in_place_t, ArgsT&&... args ) : unex( forward<ArgsT>( args )... ) {}
        constexpr explicit unexpected( in_place_t, ArgsT&&... args );   // fsl::forward not implemented yet >:)

        //template <typename U, typename ... ArgsT>
        //    requires ( constructible_v<E, ::std::initializer_list<U>&, ArgsT...> )
        //constexpr explicit unexpected( in_place_t, ::std::initializer_list<U> il, ArgsT&&... args ) : unex( il, forward<ArgsT>( args )... ) {}


        constexpr unexpected& operator=( unexpected const& ) = default;
        constexpr unexpected& operator=( unexpected&& ) = default;


        constexpr E& error() & noexcept { return unex; }
        constexpr E const& error() const& noexcept { return unex; }

        // fsl::move not yet implemented >:)
        constexpr E&& error() && noexcept;
        constexpr E const&& error() const&& noexcept;


        constexpr void swap( E& other ) /*noexcept(is_nothrow_swappable_v<E>)*/
        {
            //static_assert( is_swappable_v<E> );
            //using ::fsl::swap;
            swap( unex, other.unex );
        }

        friend constexpr void swap( unexpected& lhs, unexpected& rhs ) noexcept( noexcept( lhs.swap( rhs ) ) )
        {
            //static_assert( is_swappable_v<E> );
            lhs.swap( rhs );
        }


        template <typename E2>
        friend constexpr bool operator==( unexpected const& lhs, unexpected<E2> const& rhs );   // TODO

    private:
        E unex;
    };

    template <typename E>
    unexpected( E ) -> unexpected<E>;
}

#endif  // FSL_EXPECTED_H
