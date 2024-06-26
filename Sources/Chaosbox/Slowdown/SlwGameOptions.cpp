#include "StdH.h"

// Patcher
#include "Chaosbox/Patcher/patcher.h"

// Original function
typedef void (CSessionState::*CProcessFunc)(CNetworkMessage &);
static CProcessFunc _pProcessFunc = NULL;

// Session state patch
class CCecilSessionState : public CSessionState {
  public:
    void P_ProcessGameStreamBlock(CNetworkMessage &nmMessage) {
      _pTimer->SetCurrentTick(ses_tmLastProcessedTick);
      
      // [Cecil] Handle all actions
      if (nmMessage.GetType() == MSG_SEQ_ALLACTIONS) {
        // read time from packet
        TIME tmPacket;
        nmMessage >> tmPacket; // packet time

        // time must be greater by one than that on the last packet received
        TIME tmTickQuantum = _pTimer->TickQuantum;
        TIME tmPacketDelta = tmPacket - ses_tmLastProcessedTick;

        if (!(Abs(tmPacketDelta - tmTickQuantum) < tmTickQuantum * 0.1f)) {
          // report debug info
          CPrintF(TRANS("Session state: Mistimed MSG_ALLACTIONS: Last received tick %g, this tick %g\n"),
                  ses_tmLastProcessedTick, tmPacket);
        }

        // remember the received tick
        ses_tmLastProcessedTick = tmPacket;

        // NOTE: if we got a tick, it means that all players have joined
        // don't wait for new players any more
        ses_bWaitAllPlayers = FALSE;

        // delete all predictors
        _pNetwork->ga_World.DeletePredictors();

        // process the tick
        ProcessGameTick(nmMessage, tmPacket + GetSP()->sp_Slowdown.fAddTime); // [Cecil] Add some time

        return;
      }

      // [Cecil] Call original function
      (this->*_pProcessFunc)(nmMessage);
    };
};

// Option commands
static FLOAT tmr_fAddTime = 0.0f;

class CSlowdownModule : public CModModule {
  public:
    // Get module name
    virtual CTString GetName(void) {
      return "Slowdown Bug: Ultra Deluxe";
    };

    // Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user FLOAT tmr_fAddTime;", &tmr_fAddTime);

      // Initialize the patch
      _pProcessFunc = &CSessionState::ProcessGameStreamBlock;
      CPatch *pPatch = new CPatch(_pProcessFunc, &CCecilSessionState::P_ProcessGameStreamBlock, true, true);

      if (!pPatch->IsValid()) {
        FatalError("Cannot patch CSessionState::ProcessGameStreamBlock!");
      }
    };
    
    // Reset options
    virtual void ResetSymbols(void) {
      tmr_fAddTime = 0.0f;
    };

    // Check if the module will work in the game
    virtual BOOL ModuleActive(void) {
      return (tmr_fAddTime != 0.0f);
    };

    // Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      sp.sp_Slowdown.fAddTime = tmr_fAddTime;
    };
};

MODULE_INIT(Slowdown);
