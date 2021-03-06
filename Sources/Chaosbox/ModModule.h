#pragma once

#include "GameMP/SessionProperties.h"

// Module for initializing a certain mod
class DECL_DLL CModModule {
  public:
    // Get module name
    virtual CTString GetName(void) {
      static const CTString strUnknown = "<unknown>";
      return strUnknown;
    };

    // Declare mod symbols
    virtual void DeclareSymbols(void) {};
    
    // Reset symbols to their initial values
    virtual void ResetSymbols(void) {};

    // Check if the module will work in the game
    virtual BOOL ModuleActive(void) { return FALSE; };
    
    // Set mod options
    virtual void SetOptions(CSessionProperties &sp) {};
};

// Stack of all mod modules
extern CDynamicContainer<CModModule> _aModModules;

// Initialization method
#define MODULE_INIT(_ModulePrefix) \
  static C##_ModulePrefix##Module *_p##_ModulePrefix##Module = NULL; \
  extern void Init##_ModulePrefix(void) { \
    _p##_ModulePrefix##Module = new C##_ModulePrefix##Module(); \
    /* Declare module symbols and add it to the list */ \
    _p##_ModulePrefix##Module->DeclareSymbols(); \
    _aModModules.Add(_p##_ModulePrefix##Module); \
  };