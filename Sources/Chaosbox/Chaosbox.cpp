#include "StdH.h"
#include "ModModule.h"

// [Cecil] Set options from all modules
void SetChaosboxOptions(CSessionProperties &sp) {
  // Set options from each mod module
  FOREACHINDYNAMICCONTAINER(_aModModules, CModModule, itMod) {
    itMod->SetOptions(sp);
  }
};

// [Cecil] Initialize Chaosbox
void ChaosboxInit(void) {
  // Initialize modules
  extern void InitRnd(void);
  InitRnd();
};

// [Cecil] End Chaosbox
void ChaosboxEnd(void) {
};
