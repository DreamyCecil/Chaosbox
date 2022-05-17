2000
%{
#include "StdH.h"

#include "EntitiesMP/Player.h"
#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/ModelHolder2.h"
#include "EntitiesMP/Light.h"

// Texture stock
#include <Engine/Templates/Stock_CTextureData.h>

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

    // [Cecil] Random world textures
    /*CEntity *penRandom = _pNetwork->ga_World.wo_cenAllEntities.Pointer(0);

    // go through all brush polygons
    FOREACHINSTATICARRAY(_pNetwork->ga_World.wo_baBrushes.ba_apbpo, CBrushPolygon *, itbpo) {
      CBrushPolygon *pbpo = itbpo.Current();

      for (INDEX iLayer = 0; iLayer < 3; iLayer++) {
        INDEX ctTextures = _pTextureStock->st_ctObjects.Count();
        if (ctTextures <= 0) {
          break;
        }

        ULONG ulID = pbpo->bpo_abptTextures[iLayer].bpt_toTexture.GetData()->ser_FileName.GetHash();

        INDEX iTex = (GetSP()->sp_iTypeBased & RND_TEXTURES ? (ulID+1)*iLayer + ulID : penRandom->IRnd()) % ctTextures;
        CTString strTexture = _pTextureStock->st_ctObjects[iTex].ser_FileName;

        pbpo->bpo_abptTextures[iLayer].bpt_toTexture.SetData_t(CTFileName(strTexture));
      }
    }*/

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
