/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_grammar.cc"
#include "stdafx.h"
#include "fpconfig.hh"
#include "fparser.hh"

#include "fpoptimizer_grammar.hh"
#include "fpoptimizer_opcodename.hh"

// #include <cctype>

// line removed
using namespace FPoptimizer_Grammar;
using namespace FUNCTIONPARSERTYPES;

namespace FPoptimizer_Grammar {
bool ParamSpec_Compare(const void* aa, const void* bb, SpecialOpcode type) {
    switch (type) {
    case ParamHolder: {
        ParamSpec_ParamHolder& a = *(ParamSpec_ParamHolder*)aa;
        ParamSpec_ParamHolder& b = *(ParamSpec_ParamHolder*)bb;
        return a.constraints == b.constraints && a.index == b.index && a.depcode == b.depcode;
    }
    case NumConstant: {
        ParamSpec_NumConstant& a = *(ParamSpec_NumConstant*)aa;
        ParamSpec_NumConstant& b = *(ParamSpec_NumConstant*)bb;
        return FloatEqual(a.constvalue, b.constvalue);
    }
    case SubFunction: {
        ParamSpec_SubFunction& a = *(ParamSpec_SubFunction*)aa;
        ParamSpec_SubFunction& b = *(ParamSpec_SubFunction*)bb;
        return a.constraints == b.constraints &&
               a.data.subfunc_opcode == b.data.subfunc_opcode &&
               a.data.match_type == b.data.match_type &&
               a.data.param_count == b.data.param_count &&
               a.data.param_list == b.data.param_list &&
               a.data.restholder_index == b.data.restholder_index &&
               a.depcode == b.depcode;
    }
    }
    return true;
}

unsigned ParamSpec_GetDepCode(const ParamSpec& b) {
    switch (b.first) {
    case ParamHolder: {
        const ParamSpec_ParamHolder* s = (const ParamSpec_ParamHolder*)b.second;
        return s->depcode;
    }
    case SubFunction: {
        const ParamSpec_SubFunction* s = (const ParamSpec_SubFunction*)b.second;
        return s->depcode;
    }
    default: break;
    }
    return 0;
}

void DumpParam(const ParamSpec& parampair, std::ostream& o) {
    static const char ParamHolderNames[][2] = {"%", "&", "x", "y", "z", "a", "b", "c"};

    //o << "/*p" << (&p-pack.plist) << "*/";
    unsigned constraints = 0;
    switch (parampair.first) {
    case NumConstant: {
        const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*)parampair.second;
        o.precision(12);
        o << param.constvalue;
        break;
    }
    case ParamHolder: {
        const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*)parampair.second;
        o << ParamHolderNames[param.index];
        constraints = param.constraints;
        break;
    }
    case SubFunction: {
        const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*)parampair.second;
        constraints = param.constraints;
        if (param.data.match_type == GroupFunction) {
            if (param.data.subfunc_opcode == cNeg) {
                o << "-";
                DumpParams(param.data.param_list, param.data.param_count, o);
            } else if (param.data.subfunc_opcode == cInv) {
                o << "/";
                DumpParams(param.data.param_list, param.data.param_count, o);
            } else {
                std::string opcode = FP_GetOpcodeName(param.data.subfunc_opcode).substr(1);
                for (size_t a = 0; a < opcode.size(); ++a)
                    opcode[a] = (char)std::toupper(opcode[a]);
                o << opcode << "( ";
                DumpParams(param.data.param_list, param.data.param_count, o);
                o << " )";
            }
        } else {
            o << '(' << FP_GetOpcodeName(param.data.subfunc_opcode) << ' ';
            if (param.data.match_type == PositionalParams)
                o << '[';
            if (param.data.match_type == SelectedParams)
                o << '{';
            DumpParams(param.data.param_list, param.data.param_count, o);
            if (param.data.restholder_index != 0)
                o << " <" << param.data.restholder_index << '>';
            if (param.data.match_type == PositionalParams)
                o << "]";
            if (param.data.match_type == SelectedParams)
                o << "}";
            o << ')';
        }
        break;
    }
    }
    switch (ImmedConstraint_Value(constraints & ValueMask)) {
    case ValueMask:
        break;
    case Value_AnyNum:
        break;
    case Value_EvenInt:
        o << "@E";
        break;
    case Value_OddInt:
        o << "@O";
        break;
    case Value_IsInteger:
        o << "@I";
        break;
    case Value_NonInteger:
        o << "@F";
        break;
    case Value_Logical:
        o << "@L";
        break;
    }
    switch (ImmedConstraint_Sign(constraints & SignMask)) {
    case SignMask:
        break;
    case Sign_AnySign:
        break;
    case Sign_Positive:
        o << "@P";
        break;
    case Sign_Negative:
        o << "@N";
        break;
    }
    switch (ImmedConstraint_Oneness(constraints & OnenessMask)) {
    case OnenessMask:
        break;
    case Oneness_Any:
        break;
    case Oneness_One:
        o << "@1";
        break;
    case Oneness_NotOne:
        o << "@M";
        break;
    }
}

void DumpParams(unsigned paramlist, unsigned count, std::ostream& o) {
    for (unsigned a = 0; a < count; ++a) {
        if (a > 0)
            o << ' ';
        const ParamSpec& param = ParamSpec_Extract(paramlist, a);
        DumpParam(param, o);
        unsigned depcode = ParamSpec_GetDepCode(param);
        if (depcode != 0)
            o << "@D" << depcode;
    }
}
} // namespace FPoptimizer_Grammar
