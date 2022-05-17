#include "StdH.h"

// Patcher
#include "Chaosbox/Patcher/patcher.h"

// Original function
typedef void (CSessionState::*CProcessFunc)(CNetworkMessage &);
static CProcessFunc _pProcessFunc = &CSessionState::ProcessGameStreamBlock;

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

// [Cecil] Slowdown: Option commands
static FLOAT tmr_fAddTime = 0.0f;

class CSlowdownModule : public CModModule {
  public:
    // [Cecil] Slowdown: Declare symbols
    virtual void DeclareSymbols(void) {
      _pShell->DeclareSymbol("persistent user FLOAT tmr_fAddTime;", &tmr_fAddTime);

      // Initialize the patch
      CPatch *pPatch = new CPatch(_pProcessFunc, &CCecilSessionState::P_ProcessGameStreamBlock, true, true);

      if (!pPatch->ok()) {
        FatalError("Cannot patch CSessionState::ProcessGameStreamBlock!");
      }
    };
    
    // [Cecil] Slowdown: Reset options
    virtual void ResetSymbols(void) {
      tmr_fAddTime = 0.0f;
    };

    // [Cecil] Slowdown: Set custom options
    virtual void SetOptions(CSessionProperties &sp) {
      sp.sp_Slowdown.fAddTime = tmr_fAddTime;
    };
};

MODULE_INIT(Slowdown);
