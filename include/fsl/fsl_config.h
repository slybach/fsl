#ifndef FSL_CONFIG_H
#define FSL_CONFIG_H

// FSL_PULL_STD
// Variable to conditionally fill in the gaps in FSL by using the actual
// standard library.
// This variable is undefined by default, uncomment the below (or use your
// build system) to define the variable.
// Note that using a fsl header that provides no functionality without this
// variable defined may trigger a compile error.
//#define FSL_PULL_STD 1


// FSL_DEFINE_NULL
// Variable to control whether FSL defines the NULL macro, as it may conflict
// with other definition pulled from other headers.
// This variable is undefined by default, uncomment the below (or use your
// build system) to define the variable.
//#define FSL_DEFINE_NULL 1

#endif
