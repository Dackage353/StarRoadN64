#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <engine/n64-stdbool.h>

void optmenu_toggle(void);
void optmenu_draw(void);
void optmenu_draw_prompt(void);
void optmenu_check_buttons(void);

extern u8 optmenu_open;

extern bool configVIAntialiasing;
extern bool configVIDedither;
extern bool configNoFallDamage;
extern bool configFailWarp;
extern bool config45DegreeWallkicks;
extern bool configExtraWallkickFrames;
extern bool configFastSwimming;
extern bool configSteepSlopeJumps;
extern bool configNoLives;
extern bool configAllowExitLevel;
extern bool configFasterObjects;
extern bool configNoActSpecificObjects;
extern u32 configPreset;

#define configLives (!configNoLives)

#endif // OPTIONS_MENU_H
