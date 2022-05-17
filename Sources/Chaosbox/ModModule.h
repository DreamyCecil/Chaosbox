#pragma once

#include "GameMP/SessionProperties.h"

// [Cecil] Module for initializing a certain mod
class DECL_DLL CModModule {
  public:
    // Declare mod symbols
    virtual void DeclareSymbols(void) {};
    
    // Reset symbols to their initial values
    virtual void ResetSymbols(void) {};
    
    // Set mod options
    virtual void SetOptions(CSessionProperties &sp) {};
};

// [Cecil] Stack of all mod modules
extern CDynamicContainer<CModModule> _aModModules;

// [Cecil] Initialization method
#define MODULE_INIT(_ModulePrefix) \
  static C##_ModulePrefix##Module *_p##_ModulePrefix##Module = NULL; \
  extern void Init##_ModulePrefix(void) { \
    _p##_ModulePrefix##Module = new C##_ModulePrefix##Module(); \
    /* Declare module symbols and add it to the list */ \
    _p##_ModulePrefix##Module->DeclareSymbols(); \
    _aModModules.Add(_p##_ModulePrefix##Module); \
  };