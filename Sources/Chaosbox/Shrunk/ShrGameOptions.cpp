#include "StdH.h"

// [Cecil] Shrunk: Option commands
static FLOAT shr_fPlayerSize = 0.2f;
static FLOAT shr_fEnemySize = 0.2f;
static INDEX shr_bPlayerSpeed = FALSE;
static INDEX shr_bEnemySpeed = FALSE;
static INDEX shr_bProjSpeed = FALSE;

class CShrunkModule : public CModModule {
  public:
    // [Cecil] Shrunk: Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user FLOAT shr_fPlayerSize;", &shr_fPlayerSize);
      _pShell->DeclareSymbol("persistent user FLOAT shr_fEnemySize;", &shr_fEnemySize);
      _pShell->DeclareSymbol("persistent user INDEX shr_bPlayerSpeed;", &shr_bPlayerSpeed);
      _pShell->DeclareSymbol("persistent user INDEX shr_bEnemySpeed;", &shr_bEnemySpeed);
      _pShell->DeclareSymbol("persistent user INDEX shr_bProjSpeed;", &shr_bProjSpeed);
    };
    
    // [Cecil] Shrunk: Reset options
    virtual void ResetSymbols(void) {
      shr_fPlayerSize = 0.2f;
      shr_fEnemySize = 0.2f;
      shr_bPlayerSpeed = FALSE;
      shr_bEnemySpeed = FALSE;
      shr_bProjSpeed = FALSE;
    };

    // [Cecil] Shrunk: Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      sp.sp_Shrunk.fPlayerSize = ClampDn(shr_fPlayerSize, 0.01f);
      sp.sp_Shrunk.fEnemySize  = ClampDn(shr_fEnemySize,  0.01f);

      sp.sp_Shrunk.ulShrunk = (shr_bPlayerSpeed ? SHR_PL_SPEED : 0)
                            | (shr_bEnemySpeed  ? SHR_EN_SPEED : 0)
                            | (shr_bProjSpeed   ? SHR_PROJ_SPEED : 0);
    };
};

MODULE_INIT(Shrunk);
