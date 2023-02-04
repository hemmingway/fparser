/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_optimize_synth.cc"
#include "stdafx.h"
#include "fpconfig.hh"
#include "fparser.hh"

#include "fpoptimizer_optimize.hh"
#include "fpoptimizer_grammar.hh"
#include "fpoptimizer_codetree.hh"

#include <algorithm>
#include <assert.h>

#ifdef FP_SUPPORT_OPTIMIZER

// line removed

namespace FPoptimizer_Optimize {
/* Synthesize the given grammatic parameter into the codetree */
void SynthesizeParam(
    const ParamSpec& parampair,
    CodeTree& tree,
    MatchInfo& info,
    bool inner = true) {
    switch (parampair.first) {
    case NumConstant: {
        const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*)parampair.second;
        tree.SetImmed(param.constvalue);
        if (inner) tree.Rehash(false);
        break;
    }
    case ParamHolder: {
        const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*)parampair.second;
        tree.Become(info.GetParamHolderValue(param.index));
        break;
    }
    case SubFunction: {
        const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*)parampair.second;
        tree.SetOpcode(param.data.subfunc_opcode);
        for (unsigned a = 0; a < param.data.param_count; ++a) {
            CodeTree nparam;
            SynthesizeParam(ParamSpec_Extract(param.data.param_list, a), nparam, info, true);
            tree.AddParamMove(nparam);
        }
        if (param.data.restholder_index != 0) {
            std::vector<CodeTree> trees(info.GetRestHolderValues(param.data.restholder_index));
            tree.AddParamsMove(trees);
            // ^note: this fails if the same restholder is synth'd twice
            if (tree.GetParamCount() == 1) {
                /* Convert cMul <1> into <1> when <1> only contains one operand.
                         * This is redundant code; it is also done in ConstantFolding(),
                         * but it might be better for performance to do it here, too.
                         */
                assert(tree.GetOpcode() == cAdd || tree.GetOpcode() == cMul || tree.GetOpcode() == cMin || tree.GetOpcode() == cMax || tree.GetOpcode() == cAnd || tree.GetOpcode() == cOr);
                tree.Become(tree.GetParam(0));
            }
        }
        if (inner)
            tree.Rehash();
        break;
    }
    }
}

void SynthesizeRule(
    const Rule& rule,
    CodeTree& tree,
    MatchInfo& info) {
    switch (rule.ruletype) {
    case ProduceNewTree: {
        tree.DelParams();
        SynthesizeParam(ParamSpec_Extract(rule.repl_param_list, 0), tree, info, false);
        break;
    }
    case ReplaceParams:
    default: {
        /* Delete the matched parameters from the source tree */
        std::vector<unsigned> list = info.GetMatchedParamIndexes();
        std::sort(list.begin(), list.end());
        for (size_t a = list.size(); a-- > 0;)
            tree.DelParam(list[a]);

        /* Synthesize the replacement params */
        for (unsigned a = 0; a < rule.repl_param_count; ++a) {
            CodeTree nparam;
            SynthesizeParam(ParamSpec_Extract(rule.repl_param_list, a), nparam, info, true);
            tree.AddParamMove(nparam);
        }
        break;
    }
    }
}
} // namespace FPoptimizer_Optimize

#endif
