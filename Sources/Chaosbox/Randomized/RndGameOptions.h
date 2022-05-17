// Randomizer flags
enum ERndFlags {
  RND_PITCH       = (1 << 0), // sound pitch
  RND_ENEMIES     = (1 << 1), // enemy spawners
  RND_ENEMY_PROJ  = (1 << 2), // enemy projectiles
  RND_PLAYER_PROJ = (1 << 3), // player projectiles
  RND_MODELS      = (1 << 4), // model holders
  RND_TEXTURES    = (1 << 5), // world textures
  RND_STRETCH     = (1 << 6), // model stretching
  RND_EFFECTS     = (1 << 7), // basic effects
  RND_ITEMS       = (1 << 8), // item types
  RND_LIGHTS      = (1 << 9), // light colors
};

// Other options
enum ERndOptions {
  OPT_DAMAGEUP = (1 << 0), // 4x damage for melee, colts, r/l, g/l and laser rifle
};

// Session properties
struct SRandomizer {
  ULONG iRandom    : 10;
  ULONG iTypeBased : 10;
  ULONG ubOptions  : 1;
};
