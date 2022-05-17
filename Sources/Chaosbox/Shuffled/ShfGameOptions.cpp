#include "StdH.h"

// [Cecil] Shrunk: Option commands
static INDEX shf_bShuffleTextures = TRUE;
static INDEX shf_bShuffleSounds = TRUE;
static INDEX shf_iShuffleMusic = 2;
static INDEX shf_iShuffleVoice = 2;
static INDEX shf_bGlobalReshuffle = FALSE;

class CShuffledModule : public CModModule {
  public:
    // [Cecil] Shuffled: Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user INDEX shf_bShuffleTextures;", &shf_bShuffleTextures);
      _pShell->DeclareSymbol("persistent user INDEX shf_bShuffleSounds;", &shf_bShuffleSounds);
      _pShell->DeclareSymbol("persistent user INDEX shf_iShuffleMusic;", &shf_iShuffleMusic);
      _pShell->DeclareSymbol("persistent user INDEX shf_iShuffleVoice;", &shf_iShuffleVoice);
      _pShell->DeclareSymbol("persistent user INDEX shf_bGlobalReshuffle;", &shf_bGlobalReshuffle);
    };
    
    // [Cecil] Shuffled: Reset options
    virtual void ResetSymbols(void) {
      shf_bShuffleTextures = TRUE;
      shf_bShuffleSounds = TRUE;
      shf_iShuffleMusic = 2;
      shf_iShuffleVoice = 2;
      shf_bGlobalReshuffle = FALSE;
    };

    // [Cecil] Shuffled: Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      // Resources
      sp.sp_Shuffled.ubShuffle = 0;

      if (shf_bShuffleTextures) {
        sp.sp_Shuffled.ubShuffle |= SHFR_TEXTURES;
      }
      if (shf_bShuffleSounds) {
        sp.sp_Shuffled.ubShuffle |= SHFR_SOUNDS;
      }

      // Music
      sp.sp_Shuffled.ubShuffleMusic = 0;

      if (shf_iShuffleMusic >= 1) {
        sp.sp_Shuffled.ubShuffleMusic |= SHFS_REPLACE;
      }
      if (shf_iShuffleMusic == 2) {
        sp.sp_Shuffled.ubShuffleMusic |= SHFS_SET;
      }

      // Voice
      sp.sp_Shuffled.ubShuffleVoice = 0;

      if (shf_iShuffleVoice >= 1) {
        sp.sp_Shuffled.ubShuffleVoice |= SHFS_REPLACE;
      }
      if (shf_iShuffleVoice == 2) {
        sp.sp_Shuffled.ubShuffleVoice |= SHFS_SET;
      }

      sp.sp_Shuffled.bGlobalReshuffle = (shf_bGlobalReshuffle != 0);
    };
};

MODULE_INIT(Shuffled);
