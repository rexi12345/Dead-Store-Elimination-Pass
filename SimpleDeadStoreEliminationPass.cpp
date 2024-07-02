#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <map>
#include <set>
#include <vector>

using namespace llvm;

namespace {
struct DeadStoreEliminationPass : public FunctionPass {
  std::map<Value*, bool> isFirst;
  std::vector<Instruction*> instructionsToRemove;
  static char ID; // Pass identification, replacement for typeid

  DeadStoreEliminationPass() : FunctionPass(ID) {}

  bool findDeadStores(Function &F) {
    bool changed = false;
    for(BasicBlock &BB : F) {
      for(auto it = BB.rbegin(); it != BB.rend(); ++it) {
        Instruction &I = *it;
        if(isa<StoreInst>(&I)) {
          if(isFirst.find(I.getOperand(1)) == isFirst.end()) {
            isFirst[I.getOperand(1)] = true;
          } else {
            instructionsToRemove.push_back(&I);
          }
        }
      }
    }
    if(instructionsToRemove.size() > 0)
      changed = true;
    for(Instruction *Instr : instructionsToRemove) {
      Instr->eraseFromParent();
    }

    return changed;
  }


  bool runOnFunction(Function &F) override {
    return findDeadStores(F);
  }

};
} // end anonymous namespace

char DeadStoreEliminationPass::ID = 0;
static RegisterPass<DeadStoreEliminationPass> X("dse-pass", "Simple DSE Pass",
                                              false /* Only looks at CFG */,
                                              false /* Analysis Pass */);

