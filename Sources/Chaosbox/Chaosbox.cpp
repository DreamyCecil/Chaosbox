#include "StdH.h"
#include "ModModule.h"

// Patcher
#include "Chaosbox/Patcher/patcher.h"

// Random mod names
#define CT_MODNAMES 9
static const char *_astrModNames[CT_MODNAMES] = {
  "~ CHAOSBOX ~",
  "- chaosbox -",
  "= C#40580X =",
  "0x4348414F53424F58",
  "and the Box Of Chaos",
  "- T H E   C H A O S   E N C O U N T E R -",
  "- github.com/DreamyCecil/Chaosbox -",
  "The Chaos: The Box",
  "~ BAOSCHOX ~",
};

// Set options from all modules
void SetChaosboxOptions(CSessionProperties &sp) {
  // Set options from each mod module
  FOREACHINDYNAMICCONTAINER(_aModModules, CModModule, itMod) {
    itMod->SetOptions(sp);
  }
};

// Display active modules
void DisplayActiveModules(CDrawPort *pdp) {
  const FLOAT fScaling = pdp->GetHeight() / 480.0f;

  pdp->SetFont(_pfdDisplayFont);
  pdp->SetTextScaling(fScaling);
  pdp->SetTextCharSpacing(fScaling);

  CTString strModules = "Active modules:";
  
  FOREACHINDYNAMICCONTAINER(_aModModules, CModModule, itMod) {
    // Add color indicating if it's active
    strModules += (itMod->ModuleActive() ? "\n  ^c5fff5f" : "\n  ^caf0000");

    // Add module name
    strModules += itMod->GetName();
  }

  pdp->PutText(strModules, 8 * fScaling, 128 * fScaling, 0xFFFFFFFF);
};

// Original functions
typedef void (CDrawPort::*CCenterTextFunc)(const CTString &, PIX, PIX, COLOR) const;
static CCenterTextFunc _pCenterTextFunc = NULL;

// Drawport patches
class CCecilDrawPort : public CDrawPort {
  public:
    void P_PutTextC(const CTString &strText, PIX pixX0, PIX pixY0, COLOR colBlend = 0xFFFFFFFF) const {
      CTString strPutText = strText;
      COLOR colText = colBlend;

      // [Cecil] Add color to certain text
      if (strText == TRANS("ADVANCED OPTIONS") || strText == TRANS("OPTIONS")) {
        colText = AdjustColor(colBlend, 127, 255);
        strPutText = "> " + strPutText + " <";
      }

      // [Cecil] Call original function
      (this->*_pCenterTextFunc)(strPutText, pixX0, pixY0, colText);
    };
};

// Initialize Chaosbox
void ChaosboxInit(void) {
  // Initialize modules
  extern void InitRnd(void);
  extern void InitSpecimen(void);
  extern void InitSlowdown(void);
  extern void InitShrunk(void);
  extern void InitShuffled(void);
  InitRnd();
  InitSpecimen();
  InitSlowdown();
  InitShrunk();
  InitShuffled();

  // Patch functions
  _pCenterTextFunc = (CCenterTextFunc)&CDrawPort::PutTextC;
  CPatch *pPatch = new CPatch(_pCenterTextFunc, &CCecilDrawPort::P_PutTextC, true, true);

  if (!pPatch->IsValid()) {
    WarningMessage("^cff0000Cannot patch CDrawPort::PutTextC!\n");
  }
};

// End Chaosbox
void ChaosboxEnd(void) {
  // Pick random mod name and resave it
  CTString strModName = _astrModNames[rand() % CT_MODNAMES];
  strModName.Save_t(CTFILENAME("Data\\Var\\ModName.var"));
};
