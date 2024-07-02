add_llvm_library( LLVMOurPass MODULE
    SimpleDeadStoreEliminationPass.cpp
    
    PLUGIN_TOOL
    opt
    )