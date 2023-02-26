#ifndef FSL_CSTDDEF_H
#define FSL_CSTDDEF_H

#include "fsl_config.h"

// offsetof is part of cstddef and a priori cannot quite be implemented
// without compiler support
//
// about potential UB definitions
//
// #define offsetof(st, m) ((size_t)&(((st *)0)->m))
// It's unclear whether "dereferencing" a pointer at address 0 should be well-defined
// or provide the "expected" result. A priori this relies on the null pointer value
// having actual address 0, but internal pointer representation needs not behave that
// way. And this really is about the null pointer and not "a pointer converted from
// integer value 0 so with address zero", because 0 is a valid "null pointer constant",
// and such a constant always converts to a "null pointer value".
// (relevant C++11 quote https://timsong-cpp.github.io/cppwp/n3337/conv.ptr#1, still
// relevant in the working draft for C++23 [at the time of writing])
// Carrying on, "dereferencing" that null pointer value to do things with it is a
// priori UB, though here it isn't quite clear as this should be ultimately be
// "unevaluated context" and produce a compile-time constant extracting some form of
// "type information". Except that the blessed standard way to extract this specific
// information is via the standard defined macro offsetof, not via pointer
// shenanigans.
//
// #define offsetof(st, m) ((size_t)((char *)&((st *)0)->m - (char *)0))
// This is another common alternative, has the same weirdness about dereferencing
// to get to the member, but at least it should be more robust(?) to internal
// pointer representation introducing arbitrary offsets from zero. Another weird
// factor is determining whether the difference here is actually well-defined.
// A priori pointer arithmetics is only well-defined when the pointers point to
// array elements (with an exception for single objects and past-the-end pointers).
// I am enclined to think this is probably well-defined (granted the involved
// pointer values are considered to have been legally obtained), as objects are
// considered to be in an array of sizeof T bytes of "storage", so the arithmetics
// can be considered well defined on the array of bytes used for storage.
// Just gonna roughly point to this clause but it's honestly fairly fuzzy to me
// http://eel.is/c++draft/intro.object#3
#ifdef FSL_PULL_STD
#include <cstddef>
// no need to "inject" the std namespace into fsl, offsetof is a macro
#endif


// assume that anyone defining NULL defines it in a standard conforming way
#if defined(FSL_DEFINE_NULL) and !defined(NULL)
// valid C++11 definition, but likely to trigger "incompatible" macro redefinition errors
//#define NULL nullptr
// friendlier definition, some compilers don't warn on redefinition if the redef is the same
#define NULL 0
#endif

namespace fsl
{
    using size_t = decltype(sizeof 0);  // http://eel.is/c++draft/expr.sizeof#5
    using nullptr_t = decltype(nullptr);
    using ptrdiff_t = decltype((int*)nullptr - (int*)nullptr);  // http://eel.is/c++draft/expr.add#5

    // ok well this byte thing is actually not implementable either
    // the main utility of byte is not just about being an actual byte
    // type with safe explicit conversions and operations, it's also
    // about being permitted to alias other objects, and that's not
    // something a user-defined type can ever hope to achieve, so this
    // thing here is really just for show
    //enum class byte: unsigned char {};
    //// TODO: byte operations
    //// TODO: check namespace, the overloads are probably in global namespace?
    //// might do some type_traits stuff before that to properly constrain the relevant templates
    //// update: well constraining the byte operations to only take integer types is a little
    //// doomed because extended integer types are a thing
    //constexpr byte operator|(byte l, byte r) noexcept
    //{
    //    return static_cast<byte>(static_cast<unsigned char>(l) | static_cast<unsigned char>(r));
    //}
    //constexpr byte& operator|=(byte& l, byte r) noexcept
    //{
    //    return l = l | r;
    //}
    //constexpr byte operator&(byte l, byte r) noexcept
    //{
    //    return static_cast<byte>(static_cast<unsigned char>(l) & static_cast<unsigned char>(r));
    //}
    //constexpr byte& operator&=(byte& l, byte r) noexcept
    //{
    //    return l = l & r;
    //}
    //constexpr byte operator^(byte l, byte r) noexcept
    //{
    //    return static_cast<byte>(static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r));
    //}
    //constexpr byte& operator^=(byte& l, byte r) noexcept
    //{
    //    return l = l ^ r;
    //}
    //constexpr byte operator~(byte b) noexcept
    //{
    //    return static_cast<byte>(~static_cast<unsigned char>(b));
    //}

    // http://eel.is/c++draft/support.types.layout#5
    // specifies that max_align_t should have an alignment at least as great as
    // that of every scalar types. Scalar types include all arithmetic types,
    // arithmetic types include all integral types, integral types include all
    // signed/unsigned integer types, signed/unsigned integer types include all
    // extended signed/unsigned integer types, and I'm not going to deal with
    // any extended signed/unsigned integer types.
    // -> max_align_t has to be pulled from the standard library
#ifdef FSL_PULL_STL
    using namespace std;
#endif
}

#endif
