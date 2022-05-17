#include "StdH.h"
#include "Reshuffling.h"

// Resources
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CSoundData.h>

// Check resource path
static BOOL CheckPath(CTString str, CTString strPrefix) {
  if (strnicmp(str.str_String, strPrefix, strlen(strPrefix)) != 0) {
    return FALSE;
  }

  return TRUE;
};

// Reload sounds
static void ReloadSounds(void) {
  // Mute all sounds
  _pSound->Mute();

  // Go through the sound stock
  FOREACHINDYNAMICCONTAINER(_pSoundStock->st_ctObjects, CSoundData, itsd) {
    CSoundData &sd = *itsd;
    sd.Reload();
  }

  // All done
  CPrintF(TRANS("All sounds reloaded.\n"));
}

// Excluded resources from reshuffling
#define CT_EXCLUDE 4
static const CTString _astrExclude[CT_EXCLUDE] = {
  "Fonts\\",
  "Textures\\Interface",
  "Textures\\Logo",
  "TexturesMP\\Interface",
};

#define CT_MUSIC 2
static const CTString _astrMusic[CT_MUSIC] = {
  "Music\\",
  "MusicMP\\",
};

#define CT_VOICE 7
static const CTString _astrVoice[CT_VOICE] = {
  "Sounds\\Player\\Quotes\\",
  "Sounds\\Player\\Death",
  "Sounds\\Player\\Inhale",
  "Sounds\\Player\\Jump",
  "Sounds\\Player\\Wound",
  "SoundsMP\\Player\\Female\\",
  "SoundsMP\\Player\\Quotes\\",
};

// [Cecil] Shuffled: Get array of available textures
CStaticArray<CTString> GetTextures(void) {
  CStaticArray<CTString> astr;

  INDEX ct = _pTextureStock->st_ctObjects.Count();

  if (ct <= 0) {
    return astr;
  }

  astr.New(ct);

  for (INDEX i = 0; i < ct; i++) {
    astr[i] = _pTextureStock->st_ctObjects[i].ser_FileName;
  }

  return astr;
};

// [Cecil] Shuffled: Get array of available sounds
CStaticArray<CTString> GetSounds(void) {
  CStaticArray<CTString> astr;

  INDEX ct = _pSoundStock->st_ctObjects.Count();

  if (ct <= 0) {
    return astr;
  }

  astr.New(ct);

  for (INDEX i = 0; i < ct; i++) {
    astr[i] = _pSoundStock->st_ctObjects[i].ser_FileName;
  }

  return astr;
};

// [Cecil] Shuffled: Check if the resource is included
BOOL IncludedRes(CTString strResource, BOOL bSet) {
  // General excludes
  for (INDEX i = 0; i < CT_EXCLUDE; i++) {
    if (CheckPath(strResource, _astrExclude[i])) {
      return FALSE;
    }
  }

  // Music
  if (!(GetSP()->sp_Shuffled.ubShuffleMusic & (1 << bSet))) {
    for (INDEX i = 0; i < CT_MUSIC; i++) {
      if (CheckPath(strResource, _astrMusic[i])) {
        return FALSE;
      }
    }
  }

  // Voice
  if (!(GetSP()->sp_Shuffled.ubShuffleVoice & (1 << bSet))) {
    for (INDEX i = 0; i < CT_VOICE; i++) {
      if (CheckPath(strResource, _astrVoice[i])) {
        return FALSE;
      }
    }
  }
  
  return TRUE;
};

// [Cecil] Shuffled: Check if the resource exists
BOOL ResourceExists(CStaticArray<SResource> &aRes, CEntityComponent *pec) {
  for (INDEX i = 0; i < aRes.Count(); i++) {
    if (aRes[i].pec == pec) {
      return TRUE;
    }
  }

  return FALSE;
};

// [Cecil] Shuffled: Get all possible resources from the world
void ListResources(CEntity *pen, CStaticArray<SResource> *paTex, CStaticArray<SResource> *paSound) {
  // For all entities in this world
  FOREACHINDYNAMICCONTAINER(pen->GetWorld()->wo_cenEntities, CEntity, iten){
    // Get the DLL class of this entity
    CDLLEntityClass *pdecDLLClass = iten->en_pecClass->ec_pdecDLLClass;

    // For all classes in hierarchy of this entity
    for (; pdecDLLClass != NULL; pdecDLLClass = pdecDLLClass->dec_pdecBase) {
      // For all components
      for (INDEX i = 0; i < pdecDLLClass->dec_ctComponents; i++) {
        CEntityComponent &ec = pdecDLLClass->dec_aecComponents[i];

        if (!IncludedRes(ec.ec_fnmComponent, TRUE)) {
          continue;
        }

        if (paTex != NULL && ec.ec_ectType == ECT_TEXTURE) {
          if (ResourceExists(*paTex, &ec)) {
            continue;
          }

          INDEX ctTex = paTex->Count();
          paTex->Expand(ctTex+1);
          (*paTex)[ctTex] = SResource(ec.ec_fnmComponent, &ec);
        }

        if (paSound != NULL && ec.ec_ectType == ECT_SOUND) {
          if (ResourceExists(*paSound, &ec)) {
            continue;
          }

          INDEX ctSound = paSound->Count();
          paSound->Expand(ctSound+1);
          (*paSound)[ctSound] = SResource(ec.ec_fnmComponent, &ec);
        }
      }
    }
  }
};

// [Cecil] Shuffled: Reshuffle textures
void ShuffleTextures(CEntity *pen) {
  /*INDEX ctTextures = _pTextureStock->st_ctObjects.Count();

  if (ctTextures <= 0) {
    return;
  }
  
  CDynamicContainer<CTString> astrTextures;
  
  for (INDEX i = 0; i < ctTextures; i++) {
    CTString *pstr = new CTString(_pTextureStock->st_ctObjects[i].ser_FileName);

    // set resource
    if (IncludedRes(*pstr, TRUE)) {
      astrTextures.Add(pstr);
    }
    _pTextureStock->st_ctObjects[i].MarkUsed();
  }
  
  INDEX ctLoop = astrTextures.Count();
  
  for (INDEX iSet = 0; iSet < ctLoop; iSet++) {
    INDEX ct = astrTextures.Count();
    INDEX iTex = pen->IRnd() % ct;
    
    CTString *pstr = astrTextures.Pointer(iTex);
    
    CTFileName *pfnFile = &_pTextureStock->st_ctObjects[iSet].ser_FileName;
    
    // Replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      *pfnFile = CTFileName(*pstr);
    }
    
    astrTextures.Remove(pstr);
    delete pstr;
  }*/

  CStaticArray<SResource> aTex;
  ListResources(pen, &aTex, NULL);

  INDEX ctTex = aTex.Count();
  INDEX ctLoop = _pTextureStock->st_ctObjects.Count();
  
  // set globally
  for (INDEX iSet = 0; iSet < ctLoop; iSet++) {
    INDEX iTex = pen->IRnd() % ctTex;
    
    CTString *pstr = &aTex[iTex].strFile;
    CTFileName *pfnFile = &_pTextureStock->st_ctObjects[iSet].ser_FileName;
    
    // replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      *pfnFile = CTFileName(*pstr);
    }

    _pTextureStock->st_ctObjects[iSet].MarkUsed();
  }

  // Set for components
  /*for (INDEX iComp = 0; iComp < ctTex; iComp++) {
    INDEX iTex = pen->IRnd() % ctTex;
    
    CTString *pstr = &aTex[iTex].strFile;
    CTFileName *pfnFile = &aTex[iComp].pec->ec_fnmComponent;
    CTFileName fnRestore = *pfnFile;
    
    // Replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      aTex[iTex].pec->Release();
      *pfnFile = CTFileName(*pstr);
      aTex[iTex].pec->ObtainWithCheck();
      *pfnFile = fnRestore;
    }
  }*/
  
  _pShell->Execute("RefreshTextures();");
};

// [Cecil] Shuffled: Reshuffle sounds
void ShuffleSounds(CEntity *pen) {
  /*INDEX ctSounds = _pSoundStock->st_ctObjects.Count();

  if (ctSounds <= 0) {
    return;
  }
  
  CDynamicContainer<CTString> astrSounds;
  
  for (INDEX i = 0; i < ctSounds; i++) {
    CTString *pstr = new CTString(_pSoundStock->st_ctObjects[i].ser_FileName);

    // set resource
    if (IncludedRes(*pstr, TRUE)) {
      astrSounds.Add(pstr);
    }
    _pSoundStock->st_ctObjects[i].MarkUsed();
  }
  
  INDEX ctLoop = astrSounds.Count();
  
  for (INDEX iSet = 0; iSet < ctLoop; iSet++) {
    INDEX ct = astrSounds.Count();
    INDEX iTex = pen->IRnd() % ct;
    
    CTString *pstr = astrSounds.Pointer(iTex);
    
    CTFileName *pfnFile = &_pSoundStock->st_ctObjects[iSet].ser_FileName;
    
    // Replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      *pfnFile = CTFileName(*pstr);
    }
    
    astrSounds.Remove(pstr);
    delete pstr;
  }*/

  CStaticArray<SResource> aSound;
  ListResources(pen, NULL, &aSound);

  INDEX ctTex = aSound.Count();
  INDEX ctLoop = _pSoundStock->st_ctObjects.Count();
  
  // Set globally
  for (INDEX iSet = 0; iSet < ctLoop; iSet++) {
    INDEX iSound = pen->IRnd() % ctTex;
    
    CTString *pstr = &aSound[iSound].strFile;
    CTFileName *pfnFile = &_pSoundStock->st_ctObjects[iSet].ser_FileName;
    
    // Replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      *pfnFile = CTFileName(*pstr);
    }

    _pSoundStock->st_ctObjects[iSet].MarkUsed();
  }

  // Set for components
  /*for (INDEX iComp = 0; iComp < ctTex; iComp++) {
    INDEX iSound = pen->IRnd() % ctTex;
    
    CTString *pstr = &aSound[iSound].strFile;
    CTFileName *pfnFile = &aSound[iComp].pec->ec_fnmComponent;
    CTFileName fnRestore = *pfnFile;
    
    // replace resource
    if (IncludedRes(*pfnFile, FALSE)) {
      aSound[iSound].pec->Release();
      *pfnFile = CTFileName(*pstr);
      aSound[iSound].pec->ObtainWithCheck();
      *pfnFile = fnRestore;
    }
  }*/
  
  ReloadSounds();
};

// [Cecil] Shuffled: Reshuffle everything
void ShuffleResources(CEntity *pen) {
  if (GetSP()->sp_Shuffled.ubShuffle & SHFR_TEXTURES) {
    CPrintF("^cffff50Shuffling textures\n");
    ShuffleTextures(pen);
  }

  if (GetSP()->sp_Shuffled.ubShuffle & SHFR_SOUNDS) {
    CPrintF("^cffff50Shuffling sounds\n");
    ShuffleSounds(pen);
  }
};
