/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_opcodename.hh"
// line removed
#include <string>

#include "fpoptimizer_grammar.hh"

const std::string FP_GetOpcodeName(FPoptimizer_Grammar::SpecialOpcode opcode, bool pad = false);
const std::string FP_GetOpcodeName(FUNCTIONPARSERTYPES::OPCODE opcode, bool pad = false);
