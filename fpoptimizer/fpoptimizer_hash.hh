/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_hash.hh"
#ifndef FPoptimizerHashHH
#define FPoptimizerHashHH

#ifdef _MSC_VER

typedef unsigned long long fphash_value_t;
#define FPHASH_CONST(x) x##ULL

#else

#include <stdint.h>
typedef uint_fast64_t fphash_value_t;
#define FPHASH_CONST(x) x##ULL

#endif

namespace FUNCTIONPARSERTYPES {

struct fphash_t {
    fphash_value_t hash1, hash2;

    bool operator==(const fphash_t& rhs) const { return hash1 == rhs.hash1 && hash2 == rhs.hash2; }

    bool operator!=(const fphash_t& rhs) const { return hash1 != rhs.hash1 || hash2 != rhs.hash2; }

    bool operator<(const fphash_t& rhs) const { return hash1 != rhs.hash1 ? hash1 < rhs.hash1 : hash2 < rhs.hash2; }
};

} // namespace FUNCTIONPARSERTYPES

#endif
