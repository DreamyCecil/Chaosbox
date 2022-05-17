#include "StdH.h"
#include "SpecimenEnemy.h"

// Enemies
#include "EntitiesMP/Beast.h"
#include "EntitiesMP/Elemental.h"
#include "EntitiesMP/Eyeman.h"
#include "EntitiesMP/Grunt.h"
#include "EntitiesMP/Headman.h"
#include "EntitiesMP/Scorpman.h"
#include "EntitiesMP/Walker.h"
#include "EntitiesMP/Woman.h"

// Create specimen enemy
CEntity *CreateSpecimen(CWorld *pwo) {
  const INDEX i = GetSP()->sp_Specimen.iSpecimen;

  // No specimen
  if (i < 0) {
    return NULL;
  }

  CEntity *pen = NULL;
  CPlacement3D plTemp = CPlacement3D(FLOAT3D(-32000.0f, -512.0f, -32000.0f), ANGLE3D(0.0f, 0.0f, 0.0f));

  switch (i) {
    case SPCE_BEASTGREEN:
    case SPCE_BEASTBIG: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Beast.ecl"));

      if (i == SPCE_BEASTBIG) {
        ((CBeast *)pen)->m_bcType = BT_BIG;
      }
    } break;

    case SPCE_BONEMAN: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Boneman.ecl"));
    } break;

    case SPCE_CHAINSAWFREAK: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\ChainsawFreak.ecl"));
    } break;

    case SPCE_DEMON: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Demon.ecl"));
    } break;

    case SPCE_ELEMENTALSMALL:
    case SPCE_ELEMENTALBIG: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Elemental.ecl"));

      if (i == SPCE_ELEMENTALBIG) {
        ((CElemental *)pen)->m_EecChar = ELC_BIG;
      }
    } break;

    case SPCE_EYEMANMALE:
    case SPCE_EYEMANFEMALE: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Eyeman.ecl"));
      ((CEnemyFly *)pen)->m_EeftType = EFT_GROUND_ONLY;

      if (i == SPCE_EYEMANFEMALE) {
        ((CEyeman *)pen)->m_EecChar = EYC_SERGEANT;
      }
    } break;

    case SPCE_FISH: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Fish.ecl"));
    } break;

    case SPCE_GIZMO: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Gizmo.ecl"));
    } break;

    case SPCE_GRUNTSOLDIER:
    case SPCE_GRUNTCOMMANDER: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Grunt.ecl"));

      if (i == SPCE_GRUNTCOMMANDER) {
        ((CGrunt *)pen)->m_gtType = GT_COMMANDER;
      }
    } break;

    case SPCE_GUFFY: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Guffy.ecl"));
    } break;

    case SPCE_BOMBERMAN:
    case SPCE_FIRECRACKER:
    case SPCE_KAMIKAZE:
    case SPCE_ROCKETMAN: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Headman.ecl"));

      switch (i) {
        case SPCE_BOMBERMAN:   ((CHeadman *)pen)->m_hdtType = HDT_BOMBERMAN; break;
        case SPCE_FIRECRACKER: ((CHeadman *)pen)->m_hdtType = HDT_FIRECRACKER; break;
        case SPCE_KAMIKAZE:    ((CHeadman *)pen)->m_hdtType = HDT_KAMIKAZE; break;
        case SPCE_ROCKETMAN:   ((CHeadman *)pen)->m_hdtType = HDT_ROCKETMAN; break;
      }
    } break;

    case SPCE_SCORPMANSMALL:
    case SPCE_SCORPMANBIG: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Scorpman.ecl"));

      if (i == SPCE_SCORPMANBIG) {
        ((CScorpman *)pen)->m_smtType = SMT_GENERAL;
      }
    } break;

    case SPCE_WALKERSMALL:
    case SPCE_WALKERBIG: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Walker.ecl"));

      if (i == SPCE_WALKERBIG) {
        ((CWalker *)pen)->m_EwcChar = WLC_SERGEANT;
      }
    } break;

    case SPCE_WEREBULL: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Werebull.ecl"));
    } break;

    case SPCE_WOMAN: {
      pen = pwo->CreateEntity_t(plTemp, CTFILENAME("Classes\\Woman.ecl"));
      ((CEnemyFly *)pen)->m_EeftType = EFT_FLY_GROUND_AIR;
    } break;
  }

  ((CEnemyBase *)pen)->m_bTemplate = TRUE;
  pen->Initialize();

  ((CEnemyBase *)pen)->m_strName = "SPECIMEN_TEMP";

  return pen;
};
