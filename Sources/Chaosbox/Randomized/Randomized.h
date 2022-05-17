#pragma once

#include "EntitiesMP/Projectile.h"

// Get class ID
#define CLASS_ID(_Entity) (_Entity)->en_pecClass->ec_pdecDLLClass->dec_iID

// Random numbers (based on type or chaotically)
DECL_DLL ULONG RndNumber(CEntity *pen, ULONG ulFlag, ULONG ulID, ULONG ulSpecial);
DECL_DLL FLOAT RndFloat(CEntity *pen, ULONG ulFlag, ULONG ulID, ULONG ulSpecial);

// Random pitch level
#define RandomPitch() RandomPitch_internal(this)
DECL_DLL FLOAT RandomPitch_internal(CEntity *pen);

enum ERndProjectile {
  ERP_PLAYER,
  ERP_ENEMY,
  ERP_PL_THROW,
  ERP_EN_THROW,
};

enum ERndEffect {
  ERE_WAVE,
  ERE_EXP,
  ERE_HIT,
  ERE_OTHER,
};

// Random projectile type based on type
#define RandomProjectile(_Projectile, _Type) RandomProjectile_internal(this, _Projectile, _Type)
DECL_DLL ProjectileType RandomProjectile_internal(CEntity *pen, ProjectileType eDefault, ERndProjectile eType);

// Random effect based on type
#define RandomEffect(_Effect, _Type) RandomEffect_internal(this, _Effect, _Type)
DECL_DLL BasicEffectType RandomEffect_internal(CEntity *pen, BasicEffectType eDefault, ERndEffect eType);

// Random stretching
DECL_DLL void RandomStretch(CEntity *pen);

// Resource shuffling
DECL_DLL CTString RandomModel(CEntity *pen, const CTString &strModel);
DECL_DLL CTString RandomTexture(CEntity *pen, const CTString &strTex);

// Stronger weapons
DECL_DLL inline BOOL DamageUp(void) {
  return (GetSP()->sp_RND.ubOptions & OPT_DAMAGEUP);
};

// Enemy properties for copying
struct SEnemyProperties {
  CEntityPointer penDeath;
  CEntityPointer penPatrol;
  FLOAT fHealth;
  FLOAT fAttack;
  BOOL bBlind;
  BOOL bDeaf;
  FLOAT fStepUp;
  FLOAT fStepDn;
  FLOAT fGiveUp;
  FLOAT fReflexMin;
  FLOAT fReflexMax;
  FLOAT fActivity;
  RANGE fSense;
  FLOAT fView;

  // Replace entity pointer to an enemy
  BOOL ReplaceEnemy(CEntity *penEnemySpawner, CEntityPointer &penSpawn);

  // Gather properties from an enemy
  void GatherProperties(CEntity *pen);

  // Apply remembered properties to an enemy
  void ApplyProperties(CEntity *pen);
};