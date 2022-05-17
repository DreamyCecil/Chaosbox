2000
%{
#include "StdH.h"

#include "EntitiesMP/Player.h"
#include "EntitiesMP/EnemyBase.h"

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

components:

functions:
  // Constructor
  void CGlobalController(void) {
    // Set pointer to this entity
    _penGlobalController = this;
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
  };

  // [Cecil] Actions to perform at the beginning of each level
  void WorldStart(void) {
    // [Cecil] RND: Remember enemies
    GetTemplateEnemies();
    // [Cecil] RND: Shuffle textures
    //ShuffleTextures(this);
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
