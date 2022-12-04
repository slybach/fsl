#ifndef FSL_CSTDINT_H
#define FSL_CSTDINT_H

#include "fsl_config.h"

#ifdef FSL_PULL_STD
#include <cstdint>

namespace fsl
{
    using namespace std;
}
#else
#error "FSL does not provide cstdint. You may define the variable FSL_PULL_STD to pull the standard library into the fsl namespace."
#endif

#endif
