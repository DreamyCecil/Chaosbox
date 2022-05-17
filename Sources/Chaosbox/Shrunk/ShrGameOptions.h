// Option flags
enum EShrunkOpts {
  SHR_PL_SPEED   = (1 << 0), // Multiply player speed using the shrink size
  SHR_EN_SPEED   = (1 << 1), // Multiply enemy speed using the shrink size
  SHR_PROJ_SPEED = (1 << 2), // Multiply projectile speed using the launcher's size
};

// [Cecil] Shrunk
struct SShrunk {
  FLOAT fPlayerSize;
  FLOAT fEnemySize;
  ULONG ulShrunk : 3;
};
