2000
%{
#include "StdH.h"

#include "EntitiesMP/Player.h"

// [Cecil] RND: Randomize these classes
#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/ModelHolder2.h"
#include "EntitiesMP/Light.h"

// Pointer to this entity
extern CEntity *_penGlobalController = NULL;

// [Cecil] RND: List of template enemies
extern CDynamicContainer<CEntity> _cenEnemies = CDynamicContainer<CEntity>();
%}

class export CGlobalController : CRationalEntity {
name      "GlobalController";
thumbnail "";
features  "IsImportant";

properties:
  // [Cecil] Specimen: Specific enemy to spawn
  1 CEntityPointer m_penSpecimenEnemy,

components:

functions:
  // Constructor
  void CGlobalController(void) {
    // Set pointer to this entity
    _penGlobalController = this;
  };
  
  // Write global controller
  void Write_t(CTStream *ostr) {
    CRationalEntity::Write_t(ostr);

    // [Cecil] RND: Write template enemies
    *ostr << _cenEnemies.Count();

    FOREACHINDYNAMICCONTAINER(_cenEnemies, CEntity, iten) {
      *ostr << (INDEX)iten->en_ulID;
    }
  };

  // Read global controller
  void Read_t(CTStream *istr) {
    CRationalEntity::Read_t(istr);

    // [Cecil] RND: Read template enemies
    INDEX ctTemp;
    *istr >> ctTemp;

    while (--ctTemp >= 0) {
      INDEX iEntity;
      *istr >> iEntity;

      CEntity *penTemp = FindEntityByID(GetWorld(), iEntity);

      if (penTemp != NULL) {
        _cenEnemies.Add(penTemp);
      }
    }
  };

  // Initialization
  void OnInitialize(const CEntityEvent &eeInput) {
    CRationalEntity::OnInitialize(eeInput);
    
    // Process new world
    WorldStart();
  };

  // Destruction
  void OnEnd(void) {
    CRationalEntity::OnEnd();

    // Reset pointer
    _penGlobalController = NULL;

    // [Cecil] RND: Reset template enemies
    _cenEnemies.Clear();
  };

  // Actions to perform at the beginning of each level
  void WorldStart(void) {
    // [Cecil] RND: Remember enemies
    GetTemplateEnemies();

    // [Cecil] Specimen: Create specimen enemy
    m_penSpecimenEnemy = CreateSpecimen(GetWorld());

    // [Cecil] Shuffled: Reshuffle resources
    ShuffleResources(this);

    const BOOL bModels = (GetSP()->sp_RND.iRandom & RND_MODELS | RND_TEXTURES);
    const BOOL bLight = (GetSP()->sp_RND.iRandom & RND_LIGHTS);

    if (bModels || bLight) {
      {FOREACHINDYNAMICCONTAINER(GetWorld()->wo_cenEntities, CEntity, iten) {
        CEntity *pen = iten;

        if (bModels && IsOfClass(pen, "ModelHolder2")) {
          ((CModelHolder2*)pen)->ReloadModel();
        }

        if (bLight && IsOfClass(pen, "Light")) {
          ((CLight*)pen)->RandomLight();
        }
      }}
    }
  };

  // [Cecil] RND: Get template enemies
  void GetTemplateEnemies(void) {
    _cenEnemies.Clear();

    FOREACHINDYNAMICCONTAINER(GetWorld()->wo_cenEntities, CEntity, iten) {
      CEntity *pen = iten;

      if (!IsDerivedFromClass(pen, "Enemy Base") || IsOfClass(pen, "Fish") || IsOfClass(pen, "BigHead")) {
        continue;
      }

      CEnemyBase *penEnemy = (CEnemyBase *)pen;

      if (penEnemy->m_bTemplate && !penEnemy->m_bBoss) {
        _cenEnemies.Add(pen);
      }
    }
  };

procedures:
  Main() {
    InitAsVoid();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // travel between levels
    SetFlags(GetFlags() | ENF_CROSSESLEVELS | ENF_NOTIFYLEVELCHANGE);

    wait() {
      on (EBegin) : { resume; }

      on (EPreLevelChange) : {
        resume;
      }

      on (EPostLevelChange) : {
        // Process new world
        WorldStart();
        resume;
      }

      otherwise() : { resume; }
    }

    return;
  };
};
