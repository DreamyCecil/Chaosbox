#include "StdH.h"
#include "ModModule.h"

// Patcher
#include "Chaosbox/Patcher/patcher.h"

// [Cecil] Random mod names
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

// [Cecil] Set options from all modules
void SetChaosboxOptions(CSessionProperties &sp) {
  // Set options from each mod module
  FOREACHINDYNAMICCONTAINER(_aModModules, CModModule, itMod) {
    itMod->SetOptions(sp);
  }
};

static INDEX _iMenuLogoHook = 0;

// [Cecil] Display active modules
void DisplayActiveModules(CDrawPort *pdp) {
  // Hook it up to the menu logo rendering and only display modules in the main menu
  if (_iMenuLogoHook <= 0) {
    return;
  }
  _iMenuLogoHook--;

  const FLOAT fScaling = pdp->GetHeight() / 480.0f;

  pdp->SetFont(_pfdDisplayFont);
  pdp->SetTextScaling(fScaling);

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
typedef void (CDrawPort::*CPutTexFunc)(CTextureObject *, const PIXaabbox2D &, const MEXaabbox2D &, COLOR, COLOR, COLOR, COLOR) const;
static CPutTexFunc _pPutTexFunc = (void (CDrawPort::*)(CTextureObject *, const PIXaabbox2D &, const MEXaabbox2D &, COLOR, COLOR, COLOR, COLOR) const)&CDrawPort::PutTexture;

// [Cecil] Drawport patches
class CCecilDrawPort : public CDrawPort {
  public:
    void P_PutTexture(CTextureObject *pTO, const PIXaabbox2D &boxScreen, const MEXaabbox2D &boxTexture,
                      COLOR colUL, COLOR colUR, COLOR colDL, COLOR colDR) const
    {
      // [Cecil] Check for the menu logo textures
      if (pTO->GetData()->ser_FileName.Matches("Textures\\Logo\\sam_menulogo256a.tex")
       || pTO->GetData()->ser_FileName.Matches("Textures\\Logo\\sam_menulogo256b.tex")) {
        _iMenuLogoHook = 2;
      }

      // [Cecil] Call original function
      (this->*_pPutTexFunc)(pTO, boxScreen, boxTexture, colUL, colUR, colDL, colDR);
    };
};

// [Cecil] Initialize Chaosbox
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
  CPatch *pPatch = new CPatch(_pPutTexFunc, &CCecilDrawPort::P_PutTexture, true, true);

  if (!pPatch->ok()) {
    WarningMessage("^cff0000Cannot patch CDrawPort::PutTexture!\n");
  }
};

// [Cecil] End Chaosbox
void ChaosboxEnd(void) {
  // Pick random mod name and resave it
  CTString strModName = _astrModNames[rand() % CT_MODNAMES];
  strModName.Save_t(CTFILENAME("Data\\Var\\ModName.var"));
};
