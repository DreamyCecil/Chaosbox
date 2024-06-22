#include "StdH.h"

#include "patcher.h"

#define __DO_NOT_SHOW_PATCHER_WARNINGS__

#ifdef  __DO_NOT_SHOW_PATCHER_WARNINGS__
  #pragma warning(disable : 4309 4310 4311 4312 4786)
#endif

// [Cecil] Define extensions
bool CPatch::_bDebugOutput = false;
CTString CPatch::_strPatcherLog = "";
int CPatch::_iForceRewriteLen = -1;

// [Cecil] Append some text to the patcher log
static inline void PushLog(const CTString &strOutput) {
  if (CPatch::_bDebugOutput) {
    CPatch::_strPatcherLog += strOutput + "\n";
  }
};

HANDLE CPatch::_hHeap = NULL;

bool CPatch::CanRewriteInstructionSet(long iAddress, int &iRewriteLen)
{
  // [Cecil] Force rewrite
  if (_iForceRewriteLen != -1) {
    if (_bDebugOutput) {
      CPrintF("Forced rewrite (%d bytes)\n", _iForceRewriteLen);
    }

    iRewriteLen = _iForceRewriteLen;
    _iForceRewriteLen = -1;

    return true;
  }

  // [Cecil] Reset output log (avoid ASSERT failure)
  _strPatcherLog.str_String = StringDuplicate("- Parsing instructions -\n");

  bool bInstructionFound;
  int iReadLen = 0;
  int iInstructionLen;

  // Byte at the current address
  UBYTE *pByte = NULL;

  do {
    iInstructionLen = 0;
    bInstructionFound = false;

    pByte = reinterpret_cast<UBYTE *>(iAddress);

    if (*pByte == 0xE9) // jmp DWORD
    {
      PushLog("jmp DWORD");
      iInstructionLen = 5;
      m_iOldJump = 5 + iAddress + *reinterpret_cast<long *>(iAddress + 1);

    } else if (*pByte == 0x68 // push???
     ||        *pByte == 0xB8 // mov eax, DWORD
     || !memcmp(pByte, "\xB8\x1E", 2))
    {
      PushLog("5 bytes");
      iInstructionLen = 5;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x8B\xFF", 2)
            || !memcmp(pByte, "\x8B\xDC", 2)
            || !memcmp(pByte, "\x8B\xEC", 2)
            || !memcmp(pByte, "\x8B\xF1", 2)
            || !memcmp(pByte, "\x8B\xF9", 2) // mov
            || *pByte == 0x6A)  // push XX
    {
      PushLog("mov / push XX");
      iInstructionLen = 2;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x8B\x45", 2)  // mov eax, [ebp + arg_0]
            || !memcmp(pByte, "\x8B\x46", 2)  // mov ecx, [ebp + arg_0]
            || !memcmp(pByte, "\x8B\x4D", 2)  // mov ecx, [ebp + arg_0]
            || !memcmp(pByte, "\x8B\x5D", 2)  // mov ebx, [ebp + arg_0]
            || !memcmp(pByte, "\x8B\x55", 2)  // mov edx, [ebp + arg_0]
            || !memcmp(pByte, "\x8B\x75", 2)) // mov esi, [ebp + arg_0]
    {
      PushLog("mov XX, [ebp + arg_0]");
      iInstructionLen = 3;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x8B\xB1", 2)) { // mov esi, [ecx + DWORD]
      PushLog("mov esi, [ecx + DWORD]");
      iInstructionLen = 6;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x8D\x45", 2)) // lea eax, [ebp + ...]
    {
      PushLog("lea eax, [ebp + ...]");
      iInstructionLen = 3;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x64\xA1", 2)) // mov eax, large FS
    {
      PushLog("mov eax, large FS");
      iInstructionLen = 6;
      bInstructionFound = true;

    } else if (*pByte == 0xA1) // mov eax, DWORD
    {
      PushLog("mov eax, DWORD");
      iInstructionLen = 5;
      bInstructionFound = true;

    } else if (*pByte >= 0x50 && *pByte < 0x58) // push
    {
      PushLog("push XX");
      iInstructionLen = 1;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x81\xEC", 2)) // sub esp, DWORD
    {
      PushLog("sub esp, DWORD");
      iInstructionLen = 6;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x83\xEC", 2)) // sub esp, byte + N 
    {
      PushLog("sub esp, byte + N");
      iInstructionLen = 3;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\x83\xE4", 2)) // and esp, byte
    {
      PushLog("and esp, byte");
      iInstructionLen = 3;
      bInstructionFound = true;

    } else if (*pByte == 0x89) // mov
    {
      PushLog("mov");
      iInstructionLen = 3;
      bInstructionFound = true;

    } else if (*pByte == 0xF3) // movss xmm0, [ebp + arg_0]
    {
      PushLog("movss xmm0, [ebp + arg_0]");
      iInstructionLen = 5;
      bInstructionFound = true;

    } else if (!memcmp(pByte, "\xF6\x46", 2)) // test byte ptr [esi+...]
    {
      PushLog("test byte ptr [esi + ...]");
      iInstructionLen = 4;
      bInstructionFound = true;

    } else if (*pByte == 0xD9  // fld
            || *pByte == 0xD8) // fmul
    {
      PushLog("fld / fadd / fmul");
      iInstructionLen = 6;
      bInstructionFound = true;
    }

    iReadLen += iInstructionLen;
    iAddress += iInstructionLen;

    if (iReadLen >= 5) {
      iRewriteLen = iReadLen;

      // [Cecil] Output patcher log
      if (_bDebugOutput) {
        CPutString(_strPatcherLog + "\nInstruction found! (iReadLen >= 5)\n");
      }

      return true;
    }

  } while (bInstructionFound);
  
  // [Cecil] Output patcher log
  if (_bDebugOutput) {
    // Output next 8 instruction bytes
    UBYTE *pInstr = reinterpret_cast<UBYTE *>(iAddress);

    CTString strError;
    strError.PrintF("\nInvalid instruction! Next bytes: %02X %02X %02X %02X %02X %02X %02X %02X",
                    *(pInstr + 0), *(pInstr + 1), *(pInstr + 2), *(pInstr + 3),
                    *(pInstr + 4), *(pInstr + 5), *(pInstr + 6), *(pInstr + 7));

    InfoMessage(_strPatcherLog + strError);
  }

  return false;
}

BOOL CPatch::HookFunction(long iFuncToHook, long iMyHook, long *piNewCallAddress, bool bPatchNow)
{
  if (_hHeap == NULL) {
    _hHeap = HeapCreate(
      /*HEAP_CREATE_ENABLE_EXECUTE*/ 0x00040000 | HEAP_NO_SERIALIZE, 
      1048576, // 1MB should be quite enough
      0); //not limited
  }

  BOOL bHooked = FALSE;

  if (iFuncToHook == iMyHook || iFuncToHook == NULL || iMyHook == NULL) {
    // [Cecil] Something went wrong
    if (_bDebugOutput) {
      InfoMessage("(iFuncToHook == iMyHook) = %d\n(iFuncToHook == NULL) = %d\n(iMyHook == NULL) = %d",
                   (iFuncToHook == iMyHook),      (iFuncToHook == NULL),      (iMyHook == NULL));
    }
    return FALSE;
  }

  DWORD dwOldProtect;

  if (::VirtualProtect(reinterpret_cast<void *>(iFuncToHook), 10, PAGE_READWRITE, &dwOldProtect))
  {
    m_iOldJump = 0;

    int iRewriteLen = 0;
    const int iLongJumpLen = 5;
    int iNewInstructionLen = 0;

    if (CanRewriteInstructionSet(iFuncToHook, iRewriteLen))
    {
      iNewInstructionLen = iRewriteLen;

      if (m_iOldJump == 0) {
        iNewInstructionLen += iLongJumpLen;
      }

      //m_PatchInstructionSet = new char[iNewInstructionLen]; // executable instructions
      m_pPatchInstructionSet = reinterpret_cast<char *>(HeapAlloc(_hHeap, HEAP_ZERO_MEMORY, iNewInstructionLen)); // executable instructions

      *piNewCallAddress = reinterpret_cast<long>(m_pPatchInstructionSet);
      m_pRestorePatchSet = new char[iRewriteLen]; // not executable memory backup
      
      // 5 bytes (jmp + address) = jmp XX XX XX XX
      char InstructionSet[iLongJumpLen] = { (char)0xE9, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
      //ZeroMemory(m_PatchInstructionSet, iNewInstructionLen);

      // generating code
      memcpy(m_pPatchInstructionSet, reinterpret_cast<char *>(iFuncToHook), iRewriteLen); // copy old bytes

      if (m_iOldJump == 0) {
        m_pPatchInstructionSet[iRewriteLen] = (char)0xE9; // long jmp
      }

      long iJumpNew = m_iOldJump ? m_iOldJump : iFuncToHook + iRewriteLen;

      // calculate and set address to jmp to old function
      *reinterpret_cast<int *>(m_pPatchInstructionSet + (iNewInstructionLen - iLongJumpLen) + 1) =
        (iJumpNew) - ((reinterpret_cast<long>(m_pPatchInstructionSet)) + iNewInstructionLen);

      // rewrite function
      // set a jump to my MyHook
      *reinterpret_cast<int *>(InstructionSet + 1) = iMyHook - (iFuncToHook + iLongJumpLen);

      // rewrite original function address
      memcpy(m_pRestorePatchSet, InstructionSet, iRewriteLen);

      m_iFuncToHook = iFuncToHook;
      m_iRestoreSize = iRewriteLen;
      m_iSize = iNewInstructionLen;
      m_bValid = true;

      ::VirtualProtect(m_pPatchInstructionSet, iNewInstructionLen, PAGE_EXECUTE_READWRITE, &m_dwProtect);

      if (bPatchNow) SetPatch();

      bHooked = TRUE;
    }

    ::VirtualProtect(reinterpret_cast<void *>(iFuncToHook), 5, dwOldProtect, &dwOldProtect);

  // [Cecil] Something went wrong
  } else if (_bDebugOutput) {
    InfoMessage("Cannot attempt to rewrite instructions: %s", GetWindowsError(GetLastError()));
  }

  return bHooked;
};

// Restore old function
void CPatch::RemovePatch(bool bForever)
{
  if (m_bSetForever) return;

  if (m_bPatched)
  {
    if (!m_bValid) return;

    m_bValid = false;
    DWORD dwOldProtect;

    if (!::VirtualProtect(m_pPatchInstructionSet, m_iSize, PAGE_READWRITE, &dwOldProtect)) return;

    DWORD dwFuncOldProtect;

    if (::VirtualProtect(reinterpret_cast<void *>(m_iFuncToHook), m_iRestoreSize, PAGE_READWRITE, &dwFuncOldProtect))
    {
      ::memcpy(reinterpret_cast<void *>(m_iFuncToHook), m_pPatchInstructionSet, m_iRestoreSize);

      if (m_iOldJump) {
        const long iAddress = (m_iFuncToHook + m_iRestoreSize);
        *reinterpret_cast<long *>(iAddress - 5 + 1) = m_iOldJump - iAddress;
      }

      ::VirtualProtect(reinterpret_cast<void*>(m_iFuncToHook), m_iRestoreSize, dwFuncOldProtect, &dwFuncOldProtect);
    }

    ::VirtualProtect(m_pPatchInstructionSet, m_iSize, m_dwProtect, &dwOldProtect);

    m_bPatched = false;
    m_bValid = true;
  }

  if (bForever) {
    m_bValid = false;

    delete[] m_pRestorePatchSet;
    //delete[] m_PatchInstructionSet;

    HeapFree(_hHeap, 0, m_pPatchInstructionSet);
    m_pRestorePatchSet = 0;
    m_pPatchInstructionSet = 0;
  }
};

// Set new function
void CPatch::SetPatch(void)
{
  if (!m_bValid) return;
  if (m_bPatched) return;

  m_bValid = false;
  DWORD dwOldProtect;

  if (::VirtualProtect(reinterpret_cast<void *>(m_iFuncToHook), m_iRestoreSize, PAGE_READWRITE, &dwOldProtect))
  {
    ::memcpy(reinterpret_cast<void *>(m_iFuncToHook), m_pRestorePatchSet, m_iRestoreSize);
    ::VirtualProtect(reinterpret_cast<void *>(m_iFuncToHook), m_iRestoreSize, dwOldProtect, &dwOldProtect);
  }

  m_bValid = true;
  m_bPatched = true;
};
