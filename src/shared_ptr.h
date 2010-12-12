#pragma once

// Include file for shared pointers, which are not yet standardized.
// (You may need to change this for other compilers.)

#ifdef SHARED_PTR_MEMORY
#include <memory>
#else
#include <tr1/memory>
#endif

#ifdef SHARED_PTR_STD
using std::shared_ptr;
using std::weak_ptr;
#else
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
#endif
