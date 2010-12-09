#pragma once

// Include file for shared pointers, which are not yet standardized.
// (You may need to change this for other compilers.)

#ifdef STD_SHARED_PTR
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
#endif
