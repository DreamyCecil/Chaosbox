// Resource flags
enum EShuffleRes {
  SHFR_TEXTURES = (1 << 0),
  SHFR_SOUNDS   = (1 << 1),
};

// Sound flags
enum EShuffleSnd {
  SHFS_REPLACE = (1 << 0),
  SHFS_SET     = (1 << 1),
};

// [Cecil] Shrunk
struct SShuffled {
  UBYTE ubShuffle       : 2;
  UBYTE ubShuffleMusic  : 2;
  UBYTE ubShuffleVoice  : 2;
  BOOL bGlobalReshuffle : 1;
};
