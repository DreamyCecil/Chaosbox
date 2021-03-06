#include "StdH.h"

// Option commands
static INDEX rnd_iSoundPitch = 0;
static INDEX rnd_iSpawners = 0;
static INDEX rnd_iEnemyProjectiles = 0;
static INDEX rnd_iPlayerProjectiles = 0;
static INDEX rnd_iModels = 0;
static INDEX rnd_iTextures = 0;
static INDEX rnd_iStretch = 0;
static INDEX rnd_iEffects = 0;
static INDEX rnd_iItems = 0;
static INDEX rnd_iLightColors = 0;
static INDEX rnd_bDamageUp = FALSE;

class CRndModule : public CModModule {
  public:
    // Get module name
    virtual CTString GetName(void) {
      return "Randomized";
    };

    // Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user INDEX rnd_iSoundPitch;", &rnd_iSoundPitch);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iSpawners;", &rnd_iSpawners);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iEnemyProjectiles;", &rnd_iEnemyProjectiles);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iPlayerProjectiles;", &rnd_iPlayerProjectiles);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iModels;", &rnd_iModels);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iTextures;", &rnd_iTextures);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iStretch;", &rnd_iStretch);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iEffects;", &rnd_iEffects);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iItems;", &rnd_iItems);
      _pShell->DeclareSymbol("persistent user INDEX rnd_bDamageUp;", &rnd_bDamageUp);
      _pShell->DeclareSymbol("persistent user INDEX rnd_iLightColors;", &rnd_iLightColors);
    };
    
    // Reset options
    virtual void ResetSymbols(void) {
      rnd_iSoundPitch = 0;
      rnd_iSpawners = 0;
      rnd_iEnemyProjectiles = 0;
      rnd_iPlayerProjectiles = 0;
      rnd_iModels = 0;
      rnd_iTextures = 0;
      rnd_iStretch = 0;
      rnd_iEffects = 0;
      rnd_iItems = 0;
      rnd_iLightColors = 0;
      rnd_bDamageUp = FALSE;
    };

    // Check if the module will work in the game
    virtual BOOL ModuleActive(void) {
      return (rnd_iSoundPitch > 0
           || rnd_iSpawners > 0
           || rnd_iEnemyProjectiles > 0
           || rnd_iPlayerProjectiles > 0
           || rnd_iModels > 0
           || rnd_iTextures > 0
           || rnd_iStretch > 0
           || rnd_iEffects > 0
           || rnd_iItems > 0
           || rnd_iLightColors > 0);
    };

    // Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      sp.sp_RND.iRandom    = (rnd_iSoundPitch > 0        ? RND_PITCH : 0)
                           | (rnd_iSpawners > 0          ? RND_ENEMIES : 0)
                           | (rnd_iEnemyProjectiles > 0  ? RND_ENEMY_PROJ : 0)
                           | (rnd_iPlayerProjectiles > 0 ? RND_PLAYER_PROJ : 0)
                           | (rnd_iModels > 0            ? RND_MODELS : 0)
                           | (rnd_iTextures > 0          ? RND_TEXTURES : 0)
                           | (rnd_iStretch > 0           ? RND_STRETCH : 0)
                           | (rnd_iEffects > 0           ? RND_EFFECTS : 0)
                           | (rnd_iItems > 0             ? RND_ITEMS : 0)
                           | (rnd_iLightColors > 0       ? RND_LIGHTS : 0);

      sp.sp_RND.iTypeBased = (rnd_iSoundPitch > 1        ? RND_PITCH : 0)
                           | (rnd_iSpawners > 1          ? RND_ENEMIES : 0)
                           | (rnd_iEnemyProjectiles > 1  ? RND_ENEMY_PROJ : 0)
                           | (rnd_iPlayerProjectiles > 1 ? RND_PLAYER_PROJ : 0)
                           | (rnd_iModels > 1            ? RND_MODELS : 0)
                           | (rnd_iTextures > 1          ? RND_TEXTURES : 0)
                           | (rnd_iStretch > 1           ? RND_STRETCH : 0)
                           | (rnd_iEffects > 1           ? RND_EFFECTS : 0)
                           | (rnd_iItems > 1             ? RND_ITEMS : 0)
                           | (rnd_iLightColors > 1       ? RND_LIGHTS : 0);

      sp.sp_RND.ubOptions = (rnd_bDamageUp ? OPT_DAMAGEUP : 0);
    };
};

MODULE_INIT(Rnd);
