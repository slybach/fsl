#ifndef FSL_EXCEPTION_H
#define FSL_EXCEPTION_H

namespace fsl
{
    namespace details
    {
        // todo: ref counted string implementation for exception
    }

    /**
    * @internal Because "copy semantics" is required to be noexcept, and because
    * copying is meant to produce a distinct object whose .what() null-terminated
    * string compares equivalent, the implementation of exception should use some
    * ref-counted string, to bypass the need for allocation on copy.
    * By extension this class should probably expose a non-standard
    * potentially-throwing constructor that takes in a "string", which would be
    * used by derived class implementation.
    *
    * Note that this "noexcept .what equivalence on copy" only holds when copying
    * between two objects with the same *dynamic type*, so a derived type could
    * override .what to always return the same string literal.
    * See <https://eel.is/c++draft/support.exception#exception-2>.
    */
    class exception
    {
    public:
        /** @{ @name standard required interface */
        constexpr exception() noexcept;
        constexpr exception( exception const& other ) noexcept;

        virtual ~exception();

        exception& operator=( exception const& other ) noexcept;

        virtual char const* what() const noexcept;
        /** @} */   // standard required interface
    };
}

#endif
