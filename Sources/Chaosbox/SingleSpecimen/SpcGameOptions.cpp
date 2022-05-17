#include "StdH.h"

// Option commands
static INDEX spc_iSpecimen = SPCE_NONE;
static INDEX spc_bSetHealth = FALSE;
static FLOAT spc_fMulHealth = 1.0f;

class CSpecimenModule : public CModModule {
  public:
    // Get module name
    virtual CTString GetName(void) {
      return "Single Specimen";
    };

    // Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user INDEX spc_iSpecimen;", &spc_iSpecimen);
      _pShell->DeclareSymbol("persistent user INDEX spc_bSetHealth;", &spc_bSetHealth);
      _pShell->DeclareSymbol("persistent user FLOAT spc_fMulHealth;", &spc_fMulHealth);
    };
    
    // Reset options
    virtual void ResetSymbols(void) {
      spc_iSpecimen = SPCE_NONE;
      spc_bSetHealth = FALSE;
      spc_fMulHealth = 1.0f;
    };

    // Check if the module will work in the game
    virtual BOOL ModuleActive(void) {
      return (spc_iSpecimen >= 0);
    };

    // Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      sp.sp_Specimen.iSpecimen = spc_iSpecimen;
      sp.sp_Specimen.bSetHealth = spc_bSetHealth;
      sp.sp_Specimen.fMulHealth = spc_fMulHealth;
    };
};

MODULE_INIT(Specimen);
