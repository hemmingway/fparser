/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_optimize.cc"
#include "stdafx.h"
#include "fpconfig.hh"
#include "fparser.hh"

#include "fpoptimizer_optimize.hh"
#include "fpoptimizer_grammar.hh"
#include "fpoptimizer_codetree.hh"

#include <stdio.h>

#include <algorithm>
#include <map>
#include <sstream>

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_Grammar;
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Optimize;

namespace {
/* I have heard that std::equal_range() is practically worthless
     * due to the insane limitation that the two parameters for Comp() must
     * be of the same type. Hence we must reinvent the wheel and implement
     * our own here. This is practically identical to the one from
     * GNU libstdc++, except rewritten. -Bisqwit
     */
template <typename It, typename T, typename Comp>
std::pair<It, It>
MyEqualRange(It first, It last, const T& val, Comp comp) {
    size_t len = last - first;
    while (len > 0) {
        size_t half = len / 2;
        It middle(first);
        middle += half;
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else if (comp(val, *middle)) {
            len = half;
        } else {
            // The following implements this:
            // // left = lower_bound(first, middle, val, comp);
            It left(first);
            { ///
                It& first2 = left;
                It last2(middle);
                size_t len2 = last2 - first2;
                while (len2 > 0) {
                    size_t half2 = len2 / 2;
                    It middle2(first2);
                    middle2 += half2;
                    if (comp(*middle2, val)) {
                        first2 = middle2;
                        ++first2;
                        len2 = len2 - half2 - 1;
                    } else
                        len2 = half2;
                }
                // left = first2;  - not needed, already happens due to reference
            } ///
            first += len;
            // The following implements this:
            // // right = upper_bound(++middle, first, val, comp);
            It right(++middle);
            { ///
                It& first2 = right;
                It& last2 = first;
                size_t len2 = last2 - first2;
                while (len2 > 0) {
                    size_t half2 = len2 / 2;
                    It middle2(first2);
                    middle2 += half2;
                    if (comp(val, *middle2))
                        len2 = half2;
                    else {
                        first2 = middle2;
                        ++first2;
                        len2 = len2 - half2 - 1;
                    }
                }
                // right = first2;  - not needed, already happens due to reference
            } ///
            return std::pair<It, It>(left, right);
        }
    }
    return std::pair<It, It>(first, first);
}

/* A helper for std::equal_range */
struct OpcodeRuleCompare {
    bool operator()(const CodeTree& tree, const Rule& rule) const {
        /* If this function returns true, len=half.
             */

        if (tree.GetOpcode() != rule.match_tree.subfunc_opcode)
            return tree.GetOpcode() < rule.match_tree.subfunc_opcode;

        if (tree.GetParamCount() < rule.n_minimum_params) {
            // Tree has fewer params than required?
            return true; // Failure
        }
        return false;
    }
    bool operator()(const Rule& rule, const CodeTree& tree) const {
        /* If this function returns true, rule will be excluded from the equal_range
             */

        if (rule.match_tree.subfunc_opcode != tree.GetOpcode())
            return rule.match_tree.subfunc_opcode < tree.GetOpcode();

        if (rule.n_minimum_params < tree.GetParamCount()) {
            // Tree has more params than the pattern has?
            switch (rule.match_tree.match_type) {
            case PositionalParams:
            case SelectedParams:
                return true; // Failure
            case AnyParams:
                return false; // Not a failure
            }
        }
        return false;
    }
};

/* Test and apply a rule to a given CodeTree */
bool TestRuleAndApplyIfMatch(
    const Rule& rule,
    CodeTree& tree) {
    MatchInfo info;

    MatchResultType found(false, MatchPositionSpecBaseP());

    /*std::cout << "TESTING: ";
        DumpMatch(rule, *tree, info, false);*/

    for (;;) {
        found = TestParams(rule.match_tree, tree, found.specs, info, true);
        if (found.found)
            break;
        if (!&*found.specs) {
            // Did not match
#ifdef DEBUG_SUBSTITUTIONS
            //DumpMatch(rule, tree, info, false);
#endif
            return false;
        }
    }
    // Matched
#ifdef DEBUG_SUBSTITUTIONS
    DumpMatch(rule, tree, info, true);
#endif
    SynthesizeRule(rule, tree, info);
    return true;
}
} // namespace

namespace FPoptimizer_Grammar {
/* Apply the grammar to a given CodeTree */
bool ApplyGrammar(
    const Grammar& grammar,
    CodeTree& tree,
    bool recurse) {
    if (tree.GetOptimizedUsing() == &grammar) {
#ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Already optimized:  ";
        DumpTree(tree);
        std::cout << "\n"
                  << std::flush;
#endif
        return false;
    }

    /* First optimize all children */
    if (recurse) {
        bool changed = false;

        for (size_t a = 0; a < tree.GetParamCount(); ++a)
            if (ApplyGrammar(grammar, tree.GetParam(a)))
                changed = true;

        if (changed) {
            // Give the parent node a rerun at optimization
            tree.Mark_Incompletely_Hashed();
            return true;
        }
    }

    /* Figure out which rules _may_ match this tree */
    typedef const Rule* ruleit;

    std::pair<ruleit, ruleit> range = MyEqualRange(grammar.rule_begin,
                                                   grammar.rule_begin + grammar.rule_count,
                                                   tree,
                                                   OpcodeRuleCompare());

#ifdef DEBUG_SUBSTITUTIONS
    std::cout << "Input (Grammar #"
              << (&grammar - pack.glist)
              << ", " << FP_GetOpcodeName(tree.GetOpcode())
              << "[" << tree.GetParamCount()
              << "]"
                 ", rules "
              << (range.first - pack.glist[0].rule_begin)
              << ".."
              << (range.second - pack.glist[0].rule_begin)
              << ": ";
    DumpTree(tree);
    std::cout << "\n"
              << std::flush;
#endif

    bool changed = false;

    for (; range.first != range.second; ++range.first) {
        /* Check if this rule matches */
        if (TestRuleAndApplyIfMatch(*range.first, tree)) {
            changed = true;
            break;
        }
    }

    if (changed) {
#ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Changed." << std::endl;
        std::cout << "Output: ";
        DumpTree(tree);
        std::cout << "\n"
                  << std::flush;
#endif
        // Give the parent node a rerun at optimization
        tree.Mark_Incompletely_Hashed();
        return true;
    }

    // No changes, consider the tree properly optimized.
    tree.SetOptimizedUsing(&grammar);
    return false;
}
} // namespace FPoptimizer_Grammar

#endif
