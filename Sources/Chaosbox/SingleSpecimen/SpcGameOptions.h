// Enemy types
enum ESpecimenEnemy {
  SPCE_NONE           = -1,
  SPCE_BEASTGREEN     = 0,
  SPCE_BEASTBIG       = 1,
  SPCE_BONEMAN        = 2,
  SPCE_CHAINSAWFREAK  = 3,
  SPCE_DEMON          = 4,
  SPCE_ELEMENTALSMALL = 5,
  SPCE_ELEMENTALBIG   = 6,
  SPCE_EYEMANMALE     = 7,
  SPCE_EYEMANFEMALE   = 8,
  SPCE_FISH           = 9,
  SPCE_GIZMO          = 10,
  SPCE_GRUNTSOLDIER   = 11,
  SPCE_GRUNTCOMMANDER = 12,
  SPCE_GUFFY          = 13,
  SPCE_BOMBERMAN      = 14,
  SPCE_FIRECRACKER    = 15,
  SPCE_KAMIKAZE       = 16,
  SPCE_ROCKETMAN      = 17,
  SPCE_SCORPMANSMALL  = 18,
  SPCE_SCORPMANBIG    = 19,
  SPCE_WALKERSMALL    = 20,
  SPCE_WALKERBIG      = 21,
  SPCE_WEREBULL       = 22,
  SPCE_WOMAN          = 23,
};

// [Cecil] Specimen
struct SSpecimen {
  INDEX iSpecimen;
  BOOL bSetHealth : 1;
  FLOAT fMulHealth;
};
