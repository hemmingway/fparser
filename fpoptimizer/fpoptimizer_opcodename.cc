/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_opcodename.cc"
#include "stdafx.h"
#include "fpconfig.hh"

#include "fpoptimizer_opcodename.hh"
#include "fpoptimizer_grammar.hh"

#include <string>
#include <sstream>
#include <assert.h>

#include <iostream>

using namespace FPoptimizer_Grammar;
using namespace FUNCTIONPARSERTYPES;

const std::string FP_GetOpcodeName(FPoptimizer_Grammar::SpecialOpcode opcode, bool pad) {
#if 1
    /* Symbolic meanings for the opcodes? */
    const char* p = 0;
    switch (opcode) {
    case NumConstant:
        p = "NumConstant";
        break;
    case ParamHolder:
        p = "ParamHolder";
        break;
    case SubFunction:
        p = "SubFunction";
        break;
    }
    std::ostringstream tmp;
    //if(!p) std::cerr << "o=" << opcode << "\n";
    assert(p);
    tmp << p;
    if (pad)
        while (tmp.str().size() < 12) tmp << ' ';
    return tmp.str();
#else
    /* Just numeric meanings */
    std::ostringstream tmp;
    tmp << opcode;
    if (pad)
        while (tmp.str().size() < 5) tmp << ' ';
    return tmp.str();
#endif
}

const std::string FP_GetOpcodeName(FUNCTIONPARSERTYPES::OPCODE opcode, bool pad) {
#if 1
    /* Symbolic meanings for the opcodes? */
    const char* p = 0;
    switch (opcode) {
    case cAbs:
        p = "cAbs";
        break;
    case cAcos:
        p = "cAcos";
        break;
    case cAcosh:
        p = "cAcosh";
        break;
    case cAsin:
        p = "cAsin";
        break;
    case cAsinh:
        p = "cAsinh";
        break;
    case cAtan:
        p = "cAtan";
        break;
    case cAtan2:
        p = "cAtan2";
        break;
    case cAtanh:
        p = "cAtanh";
        break;
    case cCeil:
        p = "cCeil";
        break;
    case cCos:
        p = "cCos";
        break;
    case cCosh:
        p = "cCosh";
        break;
    case cCot:
        p = "cCot";
        break;
    case cCsc:
        p = "cCsc";
        break;
    case cEval:
        p = "cEval";
        break;
    case cExp:
        p = "cExp";
        break;
    case cExp2:
        p = "cExp2";
        break;
    case cFloor:
        p = "cFloor";
        break;
    case cIf:
        p = "cIf";
        break;
    case cInt:
        p = "cInt";
        break;
    case cLog:
        p = "cLog";
        break;
    case cLog2:
        p = "cLog2";
        break;
    case cLog10:
        p = "cLog10";
        break;
    case cMax:
        p = "cMax";
        break;
    case cMin:
        p = "cMin";
        break;
    case cPow:
        p = "cPow";
        break;
    case cSec:
        p = "cSec";
        break;
    case cSin:
        p = "cSin";
        break;
    case cSinh:
        p = "cSinh";
        break;
    case cSqrt:
        p = "cSqrt";
        break;
    case cTan:
        p = "cTan";
        break;
    case cTanh:
        p = "cTanh";
        break;
    case cImmed:
        p = "cImmed";
        break;
    case cJump:
        p = "cJump";
        break;
    case cNeg:
        p = "cNeg";
        break;
    case cAdd:
        p = "cAdd";
        break;
    case cSub:
        p = "cSub";
        break;
    case cMul:
        p = "cMul";
        break;
    case cDiv:
        p = "cDiv";
        break;
    case cMod:
        p = "cMod";
        break;
    case cEqual:
        p = "cEqual";
        break;
    case cNEqual:
        p = "cNEqual";
        break;
    case cLess:
        p = "cLess";
        break;
    case cLessOrEq:
        p = "cLessOrEq";
        break;
    case cGreater:
        p = "cGreater";
        break;
    case cGreaterOrEq:
        p = "cGreaterOrEq";
        break;
    case cNot:
        p = "cNot";
        break;
    case cAnd:
        p = "cAnd";
        break;
    case cOr:
        p = "cOr";
        break;
    case cDeg:
        p = "cDeg";
        break;
    case cRad:
        p = "cRad";
        break;
    case cFCall:
        p = "cFCall";
        break;
    case cPCall:
        p = "cPCall";
        break;
    case cRPow:
        p = "cRPow";
        break;
#ifdef FP_SUPPORT_OPTIMIZER
    case cVar:
        p = "cVar";
        break;
    case cFetch:
        p = "cFetch";
        break;
    case cPopNMov:
        p = "cPopNMov";
        break;
#endif
    case cDup:
        p = "cDup";
        break;
    case cInv:
        p = "cInv";
        break;
    case cSqr:
        p = "cSqr";
        break;
    case cRDiv:
        p = "cRDiv";
        break;
    case cRSub:
        p = "cRSub";
        break;
    case cNotNot:
        p = "cNotNot";
        break;
    case cRSqrt:
        p = "cRSqrt";
        break;
    case cNop:
        p = "cNop";
        break;
    case VarBegin:
        p = "VarBegin";
        break;
    }
    std::ostringstream tmp;
    //if(!p) std::cerr << "o=" << opcode << "\n";
    assert(p);
    tmp << p;
    if (pad)
        while (tmp.str().size() < 12) tmp << ' ';
    return tmp.str();
#else
    /* Just numeric meanings */
    std::ostringstream tmp;
    tmp << opcode;
    if (pad)
        while (tmp.str().size() < 5) tmp << ' ';
    return tmp.str();
#endif
}
