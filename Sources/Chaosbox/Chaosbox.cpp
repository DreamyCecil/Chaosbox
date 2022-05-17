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
  extern void InitShuffled(void);
  InitRnd();
  InitShuffled();
};

// [Cecil] End Chaosbox
void ChaosboxEnd(void) {
};
