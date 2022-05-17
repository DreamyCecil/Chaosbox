#pragma once

// [Cecil] Shuffled: Get array of available textures
DECL_DLL CStaticArray<CTString> GetTextures(void);

// [Cecil] Shuffled: Get array of available sounds
DECL_DLL CStaticArray<CTString> GetSounds(void);

// [Cecil] Shuffled: Check if the resource is included
DECL_DLL BOOL IncludedRes(CTString strResource, BOOL bSet);

// [Cecil] Shuffled: Single resource
struct DECL_DLL SResource {
  CTString strFile;
  CEntityComponent *pec;

  SResource(void) : strFile(""), pec(NULL) {};
  SResource(CTString strSet, CEntityComponent *pecSet) : strFile(strSet), pec(pecSet) {};
};

// [Cecil] Shuffled: Check if the resource exists
DECL_DLL BOOL ResourceExists(CStaticArray<SResource> &aRes, CEntityComponent *pec);

// [Cecil] Shuffled: Get all possible resources from the world
DECL_DLL void ListResources(CEntity *pen, CStaticArray<SResource> *paTex, CStaticArray<SResource> *paSound);

// [Cecil] Shuffled: Reshuffle textures
DECL_DLL void ShuffleTextures(CEntity *pen);

// [Cecil] Shuffled: Reshuffle sounds
DECL_DLL void ShuffleSounds(CEntity *pen);

// [Cecil] Shuffled: Reshuffle everything
DECL_DLL void ShuffleResources(CEntity *pen);
