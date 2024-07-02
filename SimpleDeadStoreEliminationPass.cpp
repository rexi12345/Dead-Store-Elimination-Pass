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
      bool firstStoreInstruction = true;
      for(auto it = BB.begin(); it != BB.end(); ++it) {
        Instruction &I = *it;
        if(isa<StoreInst>(&I)) {
          //skipping the initial store instruction
          if(firstStoreInstruction) {
            firstStoreInstruction = false;
            continue;
          }
          Value *value = I.getOperand(1);
          bool isDead = true;
          
          for(auto next = std::next(it); next != BB.end(); ++next) {
            Instruction &I = *next;
              if(isa<LoadInst>(&I) && I.getOperand(0) == value) {
                //value is being used so its live
                isDead = false;
                break;
              }
              else if(isa<StoreInst>(&I) && I.getOperand(1) == value) {
                //value is being overwritten so its not live
                isDead = true;
                break;
              }
          }
          if(isDead) {
            instructionsToRemove.push_back(&I);
            changed = true;
          }
        }
      }
    }
    for(Instruction *Inst : instructionsToRemove) {
      Inst->eraseFromParent();
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

