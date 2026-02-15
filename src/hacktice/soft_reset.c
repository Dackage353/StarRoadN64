#include "soft_reset.h"

#include "binary.h"
#include "game/level_update.h"
#include "cfg.h"

void SoftReset_onFrame()
{
    const int ResetCombo = L_TRIG | Z_TRIG;
    if (Hacktice_gConfig.softReset && (ResetCombo == (gPlayer1Controller->buttonDown & ResetCombo)))
    {
        sCurrPlayMode = 4;
        sWarpDest.type = 1;
        sWarpDest.levelNum = 1;
    }
}
