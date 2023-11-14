# Fake Standard Library - FSL


## Disclaimer

This is a joke project, if you unironically use this in an actual project, you're
99% trolling.


## Context

This project is intended as a personal exercise, to test my current knowledge of
the C++ language. The "goal" is to reimplement as many features as possible from
the standard library, in a portable way and without invoking undefined behaviour.
As of writing this, I expect my future self to pepper the code with many comments
about why I elected to do things a certain way, possibly with references or
quotes to the standard. Only my future self will know whether I'll stay true to
that.


## Structure

The namespace `fsl` is expected to mirror whatever feature from the standard
library that FSL implements. I might provide a utility header `fsl/fsl.h` to
quickly include all of FSL at once, which could be a suitable precompilation
target when using precompiled headers. To use FSL's vector, assuming vector makes
it into FSL, you are expected to `#include "fsl/vector.h"`. That should give you
a clue as to how to setup FSL if you're a lunatic wanting to use it, and if you
don't get it you definitely shouldn't be using it.


## FAQ - Falsifiedly Asked Questions

> Can FSL be used as a drop in replacement for the standard library?

No (never), but trying to become as close to one as possible? Some standard
library feature can only be provided by the implementation and with compiler
support, so features will be missing. For the supported feature, then yes it's
intended to be a drop in replacement, but unless you know for sure you're only
using features that FSL reimplemented, you can't just switch out the `std`
namespace for `fsl`. So no.


> Should this be used as a replacement for the standard library?

No? Should have been pretty clear from the [disclaimer](#disclaimer) honestly.
The self-imposed constraints pretty much make it impossible to outperform the
runtime performance of an actual standard library implementation, and I'm one
random guy doing the implementation and maintenance of this thing.

The one context I can imagine in which you might want to *use* something somewhat
similar to FSL, is if you have no standard library available but want to use
standard library features. Beware of licensing (>:)) and do note that this
project has no pretention to actually contain working code, and look at a better
different source.


> Will this project be maintained?

Well. No.
