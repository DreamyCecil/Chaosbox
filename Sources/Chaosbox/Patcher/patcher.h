#ifndef ___C_CPP_PATCHER_H___
#define ___C_CPP_PATCHER_H___

#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <windows.h>

// Disable patcher warnings
#pragma warning(push)

#ifdef __DO_NOT_SHOW_PATCHER_WARNINGS__
  #pragma warning(disable : 4311)
#endif

class CPatch
{
  private:
    // Don't care about leaks, it is allocated only once
    static HANDLE _hHeap;

  private:
    bool m_bValid;
    bool m_bPatched;
    bool m_bSetForever;
    long m_iOldJump;
    char *m_pPatchInstructionSet;
    char *m_pRestorePatchSet;
    int m_iSize;
    int m_iRestoreSize;
    DWORD m_dwProtect;
    long m_iFuncToHook;

    // Default constructor (illegal)
    CPatch();

    // Copy constructor (illegal)
    CPatch(CPatch &);

  // [Cecil] Made public
  public:

    template<class FuncType1, class FuncType2> inline
    void HookClassFunctions(FuncType1 &funcToHook, FuncType2 funcToCall, bool bPatchNow)
    {
      // Get address of where to put the new call
      FuncType1 *pFuncOld = &funcToHook;
      long &iNewCallAddress = *reinterpret_cast<long *>(pFuncOld);

      // Get target address for the jump
      long iJumpAddress = *reinterpret_cast<long *>(&funcToCall);

      HookFunction(iNewCallAddress, iJumpAddress, &iNewCallAddress, bPatchNow);
    };

  // [Cecil] Made public
  public:
    virtual bool CanRewriteInstructionSet(long iAddress, int &iRewriteLen);
    virtual BOOL HookFunction(long iFuncToHook, long iFuncToCall, long *piNewCallAddress, bool bPatchNow = true);

  // Constructors with immediate patching
  public:

    template<class FuncType> explicit
    CPatch(FuncType funcToHook, FuncType funcToCall, FuncType &iNewCallAddress, bool bPatchNow = true, bool bSetForever = false)
    {
      SetDefaultArguments(bSetForever);
      HookFunction(reinterpret_cast<long  >(funcToHook),
                   reinterpret_cast<long  >(funcToCall),
                   reinterpret_cast<long *>(&iNewCallAddress), bPatchNow);
    };

    template<class FuncType> explicit
    CPatch(FuncType funcToHook, FuncType funcToCall, FuncType *piNewCallAddress, bool bPatchNow = true, bool bSetForever = false)
    {
      SetDefaultArguments(bSetForever);
      HookFunction(reinterpret_cast<long  >(funcToHook),
                   reinterpret_cast<long  >(funcToCall),
                   reinterpret_cast<long *>(piNewCallAddress), bPatchNow);
    };

    template<class FuncType> explicit
    CPatch(FuncType &iNewCallAddress, FuncType MyHook, bool bPatchNow = true, bool bSetForever = false)
    {
      SetDefaultArguments(bSetForever);
      HookFunction(reinterpret_cast<long  >(iNewCallAddress),
                   reinterpret_cast<long  >(MyHook),
                   reinterpret_cast<long *>(&iNewCallAddress), bPatchNow);
    };

    template<class FuncType> explicit
    CPatch(FuncType *piNewCallAddress, FuncType funcToCall, bool bPatchNow = true, bool bSetForever = false)
    {
      SetDefaultArguments(bSetForever);
      HookFunction(reinterpret_cast<long  >(*piNewCallAddress),
                   reinterpret_cast<long  >(funcToCall),
                   reinterpret_cast<long *>(*piNewCallAddress), bPatchNow);
    };

    #define ____C_CPP_PATCHER_DEFINISIONS_INCL____
    #include "patcher_defines.h"

    // Destructor
    virtual ~CPatch() {
      if (!m_bSetForever) {
        RemovePatch(true);
      }
    };

    // Check if patch has been set
    inline bool IsPatched(void) {
      return m_bPatched;
    };

    // Check if the patch is valid
    inline bool IsValid(void) {
      return m_bValid;
    };

    // Set patch validity
    inline bool Valid(bool bSetValid) {
      m_bValid = bSetValid;
      return m_bValid;
    };

    // Restore old function
    virtual void RemovePatch(bool bForever = false);

    // Set new function
    virtual void SetPatch(void);

  // [Cecil] Extensions
  public:
    static bool _bDebugOutput; // Display debug output
    static CTString _strPatcherLog; // Information to display

    // Allowed to rewrite anything of this length
    static int _iForceRewriteLen;

  public:
    // Constructor without immediate function hooking
    CPatch(bool bSetForever) {
      SetDefaultArguments(bSetForever);
    };

    // Default constructor arguments
    __forceinline void SetDefaultArguments(bool bSetForever) {
      m_bValid = false;
      m_bPatched = false;
      m_bSetForever = bSetForever;
      m_pPatchInstructionSet = NULL;
      m_pRestorePatchSet = NULL;
    };

    // Set debug logging state
    static inline void SetDebug(bool bState) {
      _bDebugOutput = bState;
    };
};

// Restore warnings state
#pragma warning(pop)

#endif
