/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/
// #line 1 "fpoptimizer/fpoptimizer_bytecodesynth.hh"
#include "fpconfig.hh"
#include "fparser.hh"

#include "fpoptimizer_codetree.hh"

#ifdef FP_SUPPORT_OPTIMIZER

#include <vector>
#include <utility>

// line removed

namespace FPoptimizer_ByteCode {
class ByteCodeSynth {
public:
    ByteCodeSynth()
        : ByteCode(), Immed(), StackTop(0), StackMax(0) {
        /* estimate the initial requirements as such */
        ByteCode.reserve(64);
        Immed.reserve(8);
        StackHash.reserve(16);
    }

    void Pull(std::vector<unsigned>& bc,
              std::vector<double>& imm,
              size_t& StackTop_max) {
        ByteCode.swap(bc);
        Immed.swap(imm);
        StackTop_max = StackMax;
    }

    size_t GetByteCodeSize() const { return ByteCode.size(); }
    size_t GetStackTop() const { return StackTop; }

    void PushVar(unsigned varno) {
        ByteCode.push_back(varno);
        SetStackTop(StackTop + 1);
    }

    void PushImmed(double immed) {
        using namespace FUNCTIONPARSERTYPES;
        ByteCode.push_back(cImmed);
        Immed.push_back(immed);
        SetStackTop(StackTop + 1);
    }

    void StackTopIs(const FPoptimizer_CodeTree::CodeTree& hash) {
        if (StackTop > 0) {
            StackHash[StackTop - 1].first = true;
            StackHash[StackTop - 1].second = hash;
        }
    }

    void AddOperation(unsigned opcode, unsigned eat_count, unsigned produce_count = 1) {
        using namespace FUNCTIONPARSERTYPES;
        SetStackTop(StackTop - eat_count);

        if (opcode == cMul && ByteCode.back() == cDup)
            ByteCode.back() = cSqr;
        else
            ByteCode.push_back(opcode);
        SetStackTop(StackTop + produce_count);
    }

    void DoPopNMov(size_t targetpos, size_t srcpos) {
        using namespace FUNCTIONPARSERTYPES;
        ByteCode.push_back(cPopNMov);
        ByteCode.push_back((unsigned)targetpos);
        ByteCode.push_back((unsigned)srcpos);

        SetStackTop(srcpos + 1);
        StackHash[targetpos] = StackHash[srcpos];
        SetStackTop(targetpos + 1);
    }

    void DoDup(size_t src_pos) {
        using namespace FUNCTIONPARSERTYPES;
        if (src_pos == StackTop - 1) {
            ByteCode.push_back(cDup);
        } else {
            ByteCode.push_back(cFetch);
            ByteCode.push_back((unsigned)src_pos);
        }
        SetStackTop(StackTop + 1);
        StackHash[StackTop - 1] = StackHash[src_pos];
    }

    bool FindAndDup(const FPoptimizer_CodeTree::CodeTree& hash) {
        for (size_t a = StackTop; a-- > 0;) {
            if (StackHash[a].first && StackHash[a].second.IsIdenticalTo(hash)) {
                DoDup(a);
                return true;
            }
        }
        return false;
    }

    void SynthIfStep1(size_t& ofs) {
        using namespace FUNCTIONPARSERTYPES;
        SetStackTop(StackTop - 1); // the If condition was popped.

        ofs = ByteCode.size();
        ByteCode.push_back(cIf);
        ByteCode.push_back(0); // code index
        ByteCode.push_back(0); // Immed index
    }
    void SynthIfStep2(size_t& ofs) {
        using namespace FUNCTIONPARSERTYPES;
        SetStackTop(StackTop - 1); // ignore the pushed then-branch result.

        ByteCode[ofs + 1] = unsigned(ByteCode.size() + 2);
        ByteCode[ofs + 2] = unsigned(Immed.size());

        ofs = ByteCode.size();
        ByteCode.push_back(cJump);
        ByteCode.push_back(0); // code index
        ByteCode.push_back(0); // Immed index
    }
    void SynthIfStep3(size_t& ofs) {
        SetStackTop(StackTop - 1); // ignore the pushed else-branch result.

        ByteCode[ofs + 1] = unsigned(ByteCode.size() - 1);
        ByteCode[ofs + 2] = unsigned(Immed.size());

        SetStackTop(StackTop + 1); // one or the other was pushed.
    }

private:
    void SetStackTop(size_t value) {
        StackTop = value;
        if (StackTop > StackMax) {
            StackMax = StackTop;
            StackHash.resize(StackMax);
        }
    }

private:
    std::vector<unsigned> ByteCode;
    std::vector<double> Immed;

    std::vector<
        std::pair<bool /*known*/, FPoptimizer_CodeTree::CodeTree /*hash*/> >
        StackHash;
    size_t StackTop;
    size_t StackMax;
};

struct SequenceOpCode;
extern const SequenceOpCode AddSequence; /* Multiplication implemented with adds */
extern const SequenceOpCode MulSequence; /* Exponentiation implemented with muls */

/* Generate a sequence that multiplies or exponentifies the
     * last operand in the stack by the given constant integer
     * amount (positive or negative).
     */
void AssembleSequence(
    long count,
    const SequenceOpCode& sequencing,
    ByteCodeSynth& synth);
} // namespace FPoptimizer_ByteCode

#endif
