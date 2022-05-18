#include "StdH.h"
#include "Randomized.h"
#include "RandomLists.h"

#include "EntitiesMP/Player.h"
#include "EntitiesMP/PlayerWeapons.h"
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CTextureData.h>

#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/EnemySpawner.h"

ULONG CustomFactor(CEntity *pen) {
  ULONG ul = pen->GetWorld()->wo_fnmFileName.GetHash();
  
  // use certain factors for randomness
  if (IsOfClass(pen, "PlayerWeapons")) {
    ul = ((CPlayerWeapons*)pen)->m_iAvailableWeapons * ((CPlayerWeapons*)pen)->m_iCurrentWeapon;
  } else if (IsOfClass(pen, "Player")) {
    ul = ((CPlayer*)pen)->GetMyPlayerIndex();
  }
  
  return ul;
};

ULONG RndNumber(CEntity *pen, ULONG ulFlag, ULONG ulID, ULONG ulSpecial) {
  ULONG ulUse = ulID + CustomFactor(pen);
  
  return (GetSP()->sp_RND.iTypeBased & ulFlag ? (ulUse+1)*ulSpecial + ulUse : pen->IRnd());
};

FLOAT RndFloat(CEntity *pen, ULONG ulFlag, ULONG ulID, ULONG ulSpecial) {
  ULONG ulUse = ulID + CustomFactor(pen);
  
  ULONG ulValidID = (ulUse+1) % 256;
  return (GetSP()->sp_RND.iTypeBased & ulFlag ? FLOAT(ulValidID*ulSpecial % (ulValidID+1)) / FLOAT(ulValidID) : pen->FRnd());
};

FLOAT RandomPitch_internal(CEntity *pen) {
  // Base on simulation speed
  FLOAT fPitch = _pNetwork->ga_fGameRealTimeFactor * _pNetwork->GetRealTimeFactor();

  // From 0.75 to 1.5
  if (GetSP()->sp_RND.iRandom & RND_PITCH) {
    fPitch *= 0.75f + RndFloat(pen, RND_PITCH, pen->en_ulID, CLASS_ID(pen)) * 0.75f;
  }
  
  return fPitch;
};

ProjectileType RandomProjectile_internal(CEntity *pen, ProjectileType eDefault, ERndProjectile eType) {
  switch (eType) {
    case ERP_PLAYER:
      if (GetSP()->sp_RND.iRandom & RND_PLAYER_PROJ) {
        return _aeProjectiles[RndNumber(pen, RND_PLAYER_PROJ, pen->en_ulID, eDefault) % CT_PROJECTILES];
      }
      break;
      
    case ERP_ENEMY:
      if (GetSP()->sp_RND.iRandom & RND_ENEMY_PROJ) {
        return _aeEnemyProjectiles[RndNumber(pen, RND_ENEMY_PROJ, pen->en_ulID, eDefault) % CT_ENEMYPROJECTILES];
      }
      break;
      
    case ERP_PL_THROW:
      if (GetSP()->sp_RND.iRandom & RND_PLAYER_PROJ) {
        return _aeThrowables[RndNumber(pen, RND_PLAYER_PROJ, pen->en_ulID, eDefault) % CT_THROWABLES];
      }
      break;
      
    case ERP_EN_THROW:
      if (GetSP()->sp_RND.iRandom & RND_ENEMY_PROJ) {
        return _aeThrowables[RndNumber(pen, RND_ENEMY_PROJ, pen->en_ulID, eDefault) % CT_THROWABLES];
      }
      break;
  }
  
  return eDefault;
};

BasicEffectType RandomEffect_internal(CEntity *pen, BasicEffectType eDefault, ERndEffect eType) {
  if (!(GetSP()->sp_RND.iRandom & RND_EFFECTS)) {
    return eDefault;
  }
  
  switch (eType) {
    case ERE_WAVE:
      return _aeWaves[RndNumber(pen, RND_EFFECTS, CLASS_ID(pen), eDefault) % CT_WAVES];
      break;
      
    case ERE_EXP:
      return _aeExplosions[RndNumber(pen, RND_EFFECTS, CLASS_ID(pen), eDefault) % CT_EXPLOSIONS];
      break;
      
    case ERE_HIT:
      return _aeHits[RndNumber(pen, RND_EFFECTS, CLASS_ID(pen), eDefault) % CT_HITS];
      break;
      
    case ERE_OTHER:
      return _aeEffects[RndNumber(pen, RND_EFFECTS, CLASS_ID(pen), eDefault) % CT_EFFECTS];
      break;
  }
  
  return eDefault;
};

void RandomStretch(CEntity *pen) {
  FLOAT3D vModelStretch = pen->GetModelObject()->mo_Stretch * 123.0f;
  FLOAT fX = RndFloat(pen, RND_STRETCH, CLASS_ID(pen), INDEX(vModelStretch(1)*1));
  FLOAT fY = RndFloat(pen, RND_STRETCH, CLASS_ID(pen), INDEX(vModelStretch(2)*2));
  FLOAT fZ = RndFloat(pen, RND_STRETCH, CLASS_ID(pen), INDEX(vModelStretch(3)*3));

  FLOAT3D vStretch = FLOAT3D(Lerp(0.75f, 1.5f, fX), Lerp(0.75f, 1.5f, fY), Lerp(0.75f, 1.5f, fZ));
  pen->GetModelObject()->StretchModelRelative(vStretch);
  pen->ModelChangeNotify();
};

CTString RandomModel(CEntity *pen, const CTString &strModel) {
  if (!(GetSP()->sp_RND.iRandom & RND_MODELS)) {
    return strModel;
  }

  INDEX ctModels = _pModelStock->st_ctObjects.Count();
  if (ctModels <= 0) {
    return "";
  }
  
  INDEX iModel = RndNumber(pen, RND_MODELS, CLASS_ID(pen), strModel.GetHash()) % ctModels;
  CTString strFile = _pModelStock->st_ctObjects[iModel].ser_FileName;
  
  return strFile;
};

CTString RandomTexture(CEntity *pen, const CTString &strTex) {
  if (!(GetSP()->sp_RND.iRandom & RND_TEXTURES)) {
    return strTex;
  }

  INDEX ctTextures = _pTextureStock->st_ctObjects.Count();
  if (ctTextures <= 0) {
    return "";
  }
  
  INDEX iTex = RndNumber(pen, RND_TEXTURES, CLASS_ID(pen), strTex.GetHash()) % ctTextures;
  CTString strFile = _pTextureStock->st_ctObjects[iTex].ser_FileName;
  
  return strFile;
};

// Replace entity pointer to an enemy
BOOL SEnemyProperties::ReplaceEnemy(CEntity *penEnemySpawner, CEntityPointer &penSpawn) {
  ASSERT(IsDerivedFromClass(penEnemySpawner, "Enemy Spawner") && IsDerivedFromClass(penSpawn, "Enemy Base"));

  // Not randomized
  if (!(GetSP()->sp_RND.iRandom & RND_ENEMIES)) {
    return FALSE;
  }
    
  CEnemySpawner &enSpawner = (CEnemySpawner &)*penEnemySpawner;

  // Gather enemy properties
  GatherProperties(penSpawn);

  // Replace pointer to an enemy
  extern CDynamicContainer<CEntity> _cenEnemies;
  INDEX ctTemplates = _cenEnemies.Count();

  // If there are any templates and it's not a fish
  if (ctTemplates > 0 && !IsOfClass(enSpawner.m_penTarget, "Fish")) {
    INDEX iRandomEnemy = RndNumber(penEnemySpawner, RND_ENEMIES, enSpawner.en_ulID, enSpawner.m_penTarget->en_ulID) % ctTemplates;
    penSpawn = _cenEnemies.Pointer(iRandomEnemy);
  }

  return TRUE;
};

// Gather properties from an enemy
void SEnemyProperties::GatherProperties(CEntity *pen) {
  ASSERT(IsDerivedFromClass(pen, "Enemy Base"));

  CEnemyBase &enEnemy = (CEnemyBase &)*pen;

  penDeath   = enEnemy.m_penDeathTarget;
  penPatrol  = enEnemy.m_penMarker;
  fHealth    = enEnemy.m_fMaxHealth;
  fAttack    = enEnemy.m_fAttackRadius;
  bBlind     = enEnemy.m_bBlind;
  bDeaf      = enEnemy.m_bDeaf;
  fStepUp    = enEnemy.m_fStepHeight;
  fStepDn    = enEnemy.m_fFallHeight;
  fGiveUp    = enEnemy.m_tmGiveUp;
  fReflexMin = enEnemy.m_tmReflexMin;
  fReflexMax = enEnemy.m_tmReflexMax;
  fActivity  = enEnemy.m_fActivityRange;
  fSense     = enEnemy.m_fSenseRange;
  fView      = enEnemy.m_fViewAngle;
};

// Apply remembered properties to an enemy
void SEnemyProperties::ApplyProperties(CEntity *pen) {
  ASSERT(IsDerivedFromClass(pen, "Enemy Base"));

  CEnemyBase &enEnemy = (CEnemyBase &)*pen;

  enEnemy.m_penDeathTarget = penDeath;
  enEnemy.m_penMarker      = penPatrol;
  enEnemy.m_fAttackRadius  = fAttack;
  enEnemy.m_bBlind         = bBlind;
  enEnemy.m_bDeaf          = bDeaf;
  enEnemy.m_fStepHeight    = fStepUp;
  enEnemy.m_fFallHeight    = fStepDn;
  enEnemy.m_tmGiveUp       = fGiveUp;
  enEnemy.m_tmReflexMin    = fReflexMin;
  enEnemy.m_tmReflexMax    = fReflexMax;
  enEnemy.m_fActivityRange = fActivity;
  enEnemy.m_fSenseRange    = fSense;
  enEnemy.m_fViewAngle     = fView;
};
