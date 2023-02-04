/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_main.cc"
#include "stdafx.h"
#include "fpconfig.hh"
#include "fparser.hh"

#include "fpoptimizer_codetree.hh"
#include "fpoptimizer_grammar.hh"
// line removed
// line removed

using namespace FUNCTIONPARSERTYPES;

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Grammar;

void FunctionParser::Optimize() {
    CopyOnWrite();

    //PrintByteCode(std::cout);

    CodeTree tree;
    tree.GenerateFrom(data->ByteCode, data->Immed, *data);

    while (ApplyGrammar(pack.glist[0], tree)) {
        // intermediate
        //std::cout << "Rerunning 1\n";
        FixIncompleteHashes(tree);
    }

    while (ApplyGrammar(pack.glist[1], tree)) {
        // final1
        //std::cout << "Rerunning 2\n";
        FixIncompleteHashes(tree);
    }

    while (ApplyGrammar(pack.glist[2], tree)) {
        // final2
        //std::cout << "Rerunning 3\n";
        FixIncompleteHashes(tree);
    }

    std::vector<unsigned> byteCode;
    std::vector<double> immed;
    size_t stacktop_max = 0;
    tree.SynthesizeByteCode(byteCode, immed, stacktop_max);

    /*std::cout << std::flush;
    std::cerr << std::flush;
    fprintf(stderr, "Estimated stacktop %u\n", (unsigned)stacktop_max);
    fflush(stderr);*/

    if (data->StackSize != stacktop_max) {
        data->StackSize = stacktop_max; // note: gcc warning is meaningful
        data->Stack.resize(stacktop_max);
    }

    data->ByteCode.swap(byteCode);
    data->Immed.swap(immed);

    //PrintByteCode(std::cout);
}

#endif
