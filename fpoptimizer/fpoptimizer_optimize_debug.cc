/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_optimize_debug.cc"
#include "stdafx.h"
#include "fpconfig.hh"
#include "fparser.hh"

#include <sstream>

#ifdef DEBUG_SUBSTITUTIONS
using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_Grammar;
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Optimize;

namespace FPoptimizer_Grammar {
void DumpMatch(const Rule& rule,
               const CodeTree& tree,
               const MatchInfo& info,
               bool DidMatch,
               std::ostream& o) {
    static const char ParamHolderNames[][2] = {"%", "&", "x", "y", "z", "a", "b", "c"};

    o << "Found " << (DidMatch ? "match" : "mismatch") << ":\n"
                                                          "  Pattern    : ";
    {
        ParamSpec tmp;
        tmp.first = SubFunction;
        ParamSpec_SubFunction tmp2;
        tmp2.data = rule.match_tree;
        tmp.second = (const void*)&tmp2;
        DumpParam(tmp, o);
    }
    o << "\n"
         "  Replacement: ";
    DumpParams(rule.repl_param_list, rule.repl_param_count, o);
    o << "\n";

    o << "  Tree       : ";
    DumpTree(tree, o);
    o << "\n";
    if (DidMatch) DumpHashes(tree, o);

    for (std::map<unsigned, CodeTree>::const_iterator
             i = info.paramholder_matches.begin();
         i != info.paramholder_matches.end();
         ++i) {
        o << "           " << ParamHolderNames[i->first] << " = ";
        DumpTree(i->second, o);
        o << "\n";
    }

    for (std::multimap<unsigned, CodeTree>::const_iterator
             i = info.restholder_matches.begin();
         i != info.restholder_matches.end();
         ++i) {
        o << "         <" << i->first << "> = ";
        DumpTree(i->second, o);
        o << std::endl;
    }
    o << std::flush;
}

void DumpHashes(const CodeTree& tree,
                std::map<fphash_t, std::set<std::string> >& done,
                std::ostream& o) {
    for (size_t a = 0; a < tree.GetParamCount(); ++a)
        DumpHashes(tree.GetParam(a), done, o);

    std::ostringstream buf;
    DumpTree(tree, buf);
    done[tree.GetHash()].insert(buf.str());
}
void DumpHashes(const CodeTree& tree, std::ostream& o) {
    std::map<fphash_t, std::set<std::string> > done;
    DumpHashes(tree, done, o);

    for (std::map<fphash_t, std::set<std::string> >::const_iterator
             i = done.begin();
         i != done.end();
         ++i) {
        const std::set<std::string>& flist = i->second;
        if (flist.size() != 1) o << "ERROR - HASH COLLISION?\n";
        for (std::set<std::string>::const_iterator
                 j = flist.begin();
             j != flist.end();
             ++j) {
            o << '[' << std::hex << i->first.hash1
              << ',' << i->first.hash2
              << ']' << std::dec;
            o << ": " << *j << "\n";
        }
    }
}
void DumpTree(const CodeTree& tree, std::ostream& o) {
    //o << "/*" << tree.Depth << "*/";
    const char* sep2 = "";
    /*
        o << '[' << std::hex << tree.Hash.hash1
                      << ',' << tree.Hash.hash2
                      << ']' << std::dec;
        */
    switch (tree.GetOpcode()) {
    case cImmed: o << tree.GetImmed(); return;
    case cVar: o << "Var" << (tree.GetVar() - VarBegin); return;
    case cAdd: sep2 = " +"; break;
    case cMul: sep2 = " *"; break;
    case cAnd: sep2 = " &"; break;
    case cOr: sep2 = " |"; break;
    case cPow: sep2 = " ^"; break;
    default:
        o << FP_GetOpcodeName(tree.GetOpcode());
        if (tree.GetOpcode() == cFCall || tree.GetOpcode() == cPCall)
            o << ':' << tree.GetFuncNo();
    }
    o << '(';
    if (tree.GetParamCount() <= 1 && *sep2) o << (sep2 + 1) << ' ';
    for (size_t a = 0; a < tree.GetParamCount(); ++a) {
        if (a > 0) o << ' ';

        DumpTree(tree.GetParam(a), o);

        if (a + 1 < tree.GetParamCount()) o << sep2;
    }
    o << ')';
}

void DumpTreeWithIndent(const CodeTree& tree, std::ostream& o, const std::string& indent) {
    o << '[' << std::hex << (void*)(&tree.GetParams())
      << std::dec
      << ',' << tree.GetRefCount()
      << ']';
    o << indent << '_';

    switch (tree.GetOpcode()) {
    case cImmed:
        o << "cImmed " << tree.GetImmed();
        o << '\n';
        return;
    case cVar:
        o << "cVar " << (tree.GetVar() - VarBegin);
        o << '\n';
        return;
    default:
        o << FP_GetOpcodeName(tree.GetOpcode());
        if (tree.GetOpcode() == cFCall || tree.GetOpcode() == cPCall)
            o << ':' << tree.GetFuncNo();
        o << '\n';
    }
    for (size_t a = 0; a < tree.GetParamCount(); ++a) {
        std::string ind = indent;
        for (size_t p = 0; p < ind.size(); p += 2)
            if (ind[p] == '\\')
                ind[p] = ' ';
        ind += (a + 1 < tree.GetParamCount()) ? " |" : " \\";
        DumpTreeWithIndent(tree.GetParam(a), o, ind);
    }
    o << std::flush;
}
} // namespace FPoptimizer_Grammar
#endif
