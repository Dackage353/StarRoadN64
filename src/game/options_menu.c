#include "sm64.h"
#include "gfx_dimensions.h"
#include "engine/math_util.h"
#include "audio/external.h"
#include "game/camera.h"
#include "game/level_update.h"
#include "game/print.h"
#include "game/segment2.h"
#include "game/save_file.h"
#include "game/mario_misc.h"
#include "game/game_init.h"
#include "game/ingame_menu.h"
#include "game/options_menu.h"

#include "engine/n64-stdbool.h"

u8 optmenu_open = 0;

static u8 optmenu_binding = 0;
static u8 optmenu_bind_idx = 0;

// How to add stuff:
// strings: add them to include/text_strings.h.in
//          and to optMainStr[] / opts*Str[]
// options: add them to the relevant options list
// menus:   add a new submenu definition and a new
//          option to the optsMain list

static const char toggleStr[][16] = {
    { "Disabled" },
    { "Enabled" },
};

static const char optSmallStr[][32] = {
    { "Ⓡ Options" },
    { "Ⓡ Return" },
    { "▶" },
    { "◀" },
};

enum OptType {
    OPT_INVALID = 0,
    OPT_TOGGLE,
    OPT_CHOICE,
    OPT_SCROLL,
    OPT_SUBMENU,
    OPT_BIND,
    OPT_BUTTON,
};

struct SubMenu;

struct Option {
    enum OptType type;
    const char *label;
    const char *explanation;
    union {
        u32 *uval;
        bool *bval;
    };
    union {
        struct {
            const char **choices;
            u32 numChoices;
        };
        struct {
            u32 scrMin;
            u32 scrMax;
            u32 scrStep;
        };
        struct SubMenu *nextMenu;
        void (*actionFn)(struct Option *, s32);
    };
};

struct SubMenu {
    struct SubMenu *prev; // this is set at runtime to avoid needless complication
    struct Option *opts;
    s32 numOpts;
    s32 select;
    s32 scroll;
};

/* helper macros */

#define DEF_OPT_TOGGLE(lbl, expl, bv) \
    { .type = OPT_TOGGLE, .label = lbl, .explanation = expl, .bval = bv }
#define DEF_OPT_SCROLL(lbl, expl, uv, min, max, st) \
    { .type = OPT_SCROLL, .label = lbl, .explanation = expl, .uval = uv, .scrMin = min, .scrMax = max, .scrStep = st }
#define DEF_OPT_CHOICE(lbl, expl, uv, ch) \
    { .type = OPT_CHOICE, .label = lbl, .explanation = expl, .uval = uv, .choices = ch, .numChoices = sizeof(ch) / sizeof(ch[0]) - 1 }
#define DEF_OPT_SUBMENU(lbl, expl, nm) \
    { .type = OPT_SUBMENU, .label = lbl, .explanation = expl, .nextMenu = nm }

#define DEF_SUBMENU(opt) \
    { .opts = opt, .numOpts = sizeof(opt) / sizeof(opt[0]) }

/* submenu definitions */

bool configVIAntialiasing       = true;
bool configVIDedither           = true;

bool configNoFallDamage         = false;
bool configFailWarp             = false;
bool config45DegreeWallkicks    = false;
bool configExtraWallkickFrames  = false;
bool configFastSwimming         = false;
bool configSteepSlopeJumps      = false;
bool configNoLives              = false;
bool configAllowExitLevel       = false;
bool configFasterObjects        = false;
bool configNoActSpecificObjects = false;

u32 configPreset = 0;

static const char sViAntialiasingExplanation[]       = "Smooths out jagged edges,\n"
                                                       "but adds more blur. Reduces performance.";
static const char sViDeditherExplanation[]           = "Reduces color dither patterns,\n"
                                                       "but adds more blur. Reduces performance.";
static const char sFallDamageExplanation[]           = "Disables fall damage from\n"
                                                       "high falls.";
static const char sFailWarpExplanation[]             = "Warps the player to the\n"
                                                       "last safe spot on death.";
static const char s45DegreeWallkicksExplanation[]    = "Allows wallkicks on\n"
                                                       "45 degree walls jumps.";
static const char sExtraWallkickFramesExplanation[]  = "Gives 3 extra frames to\n"
                                                       "perform wallkicks.";
static const char sFastSwimmingExplanation[]         = "Increases swimming movement speed.\n"
                                                       "Hold A for fastest swim.";
static const char sSteepSlopeJumpsExplanation[]      = "Allows proper jumps on slopes\n"
                                                       "that are very steep.";
static const char sLivesExplanation[]                = "Disables lives system";
static const char sAllowExitLevelExplanation[]       = "Adds extra pause option to exit\n"
                                                       "immediately outside the level or\n"
                                                       "back to the castle grounds.";
static const char sFasterObjectsExplanation[]        = "Increases speed of push blocks and boat.\n"
                                                       "Moving platforms speed depends on Mario position.\n";
static const char sNoActSpecificObjectsExplanation[] = "Removes unnecessary act specific objects.";
static const char sPresetExplanation[]               = "Classic - close to original hack\n"
                                                       "QoL Only - minimal changes to enhance gameplay\n"
                                                       "Modern - more changes for a modern experience";
static const char sAdvancedExplanation[]             = "Select exact patches you want to use.\n"
                                                       "Overrides the preset selection.";

static struct Option optsAudio[] = {
    DEF_OPT_TOGGLE("No fall damage"         , sFallDamageExplanation          , &configNoFallDamage),
    DEF_OPT_TOGGLE("Fail warps"             , sFailWarpExplanation            , &configFailWarp),
    DEF_OPT_TOGGLE("45 degree wallkicks"    , s45DegreeWallkicksExplanation   , &config45DegreeWallkicks),
    DEF_OPT_TOGGLE("Extra wallkick frames"  , sExtraWallkickFramesExplanation , &configExtraWallkickFrames),
    DEF_OPT_TOGGLE("Faster swimming"        , sFastSwimmingExplanation        , &configFastSwimming),
    DEF_OPT_TOGGLE("Steep slope jumps"      , sSteepSlopeJumpsExplanation     , &configSteepSlopeJumps),
    DEF_OPT_TOGGLE("No lives"               , sLivesExplanation               , &configNoLives),
    DEF_OPT_TOGGLE("Allow extra exit level" , sAllowExitLevelExplanation      , &configAllowExitLevel),
    DEF_OPT_TOGGLE("Faster objects"         , sFasterObjectsExplanation       , &configFasterObjects),
    DEF_OPT_TOGGLE("No act specific objects", sNoActSpecificObjectsExplanation, &configNoActSpecificObjects),
};

static struct SubMenu menuAudio    = DEF_SUBMENU( optsAudio );

static const char* sPresets[] = {
    "Classic",
    "QoL Only",
    "Modern",
    "Custom",
};

static struct Option optsMain[] = {
    DEF_OPT_TOGGLE("VI antialiasing"        , sViAntialiasingExplanation      , &configVIAntialiasing),
    DEF_OPT_TOGGLE("VI dedither"            , sViDeditherExplanation          , &configVIDedither),
    DEF_OPT_CHOICE("Patches preset"         , sPresetExplanation              , &configPreset, sPresets),
    DEF_OPT_SUBMENU("Advanced patches menu..." , sAdvancedExplanation, &menuAudio ),
};

static struct SubMenu menuMain = DEF_SUBMENU( optsMain );

/* implementation */

static s32 optmenu_option_timer = 0;
static u8 optmenu_hold_count = 0;
static s32 optmenu_sin_timer = 0;

static struct SubMenu *currentMenu = &menuMain;

static inline s32 wrap_add(s32 a, const s32 b, const s32 min, const s32 max) {
    a += b;
    if (a < min) a = max - (min - a) + 1;
    else if (a > max) a = min + (a - max) - 1;
    return a;
}

static void uint_to_hex(u32 num, char *dst) {
    sprintf(dst, "%04X", num);
}

//Displays a box.
static void optmenu_draw_box(s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(r, g, b, 255));
    gDPFillRectangle(gDisplayListHead++, x1, y1, x2 - 1, y2 - 1);
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

static void optmenu_draw_text(s16 x, s16 y, const char *str, u8 col) {
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    print_generic_string_aligned(x+1, y-1, str, TEXT_ALIGN_CENTER);
    if (col == 0) {
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    } else {
        gDPSetEnvColor(gDisplayListHead++, 255, 32, 32, 255);
    }
    print_generic_string_aligned(x, y, str, TEXT_ALIGN_CENTER);
}

static void optmenu_draw_opt(const struct Option *opt, s16 x, s16 y, u8 sel) {
    char buf[32] = { 0 };

    if (opt->type == OPT_SUBMENU || opt->type == OPT_BUTTON)
        y -= 6;

    optmenu_draw_text(x, y, opt->label, sel);

    switch (opt->type) {
        case OPT_TOGGLE:
            optmenu_draw_text(x, y-13, toggleStr[(int)*opt->bval], sel);
            break;

        case OPT_CHOICE:
            optmenu_draw_text(x, y-13, opt->choices[*opt->uval], sel);
            break;

        case OPT_SCROLL:
            sprintf(buf, "%d", *opt->uval);
            optmenu_draw_text(x, y-13, buf, sel);
            break;

        default: break;
    };
}

extern void set_vi_mode_from_config();
static void optmenu_opt_change(struct Option *opt, s32 val) {
    switch (opt->type) {
        case OPT_TOGGLE:
            *opt->bval = !*opt->bval;
            if (opt->bval == &configVIAntialiasing || opt->bval == &configVIDedither)
            {
                set_vi_mode_from_config();
            }
            else
            {
                configPreset = 3; // Custom
            }
            break;

        case OPT_CHOICE:
            *opt->uval = wrap_add(*opt->uval, val, 0, opt->numChoices - 1);
            if (opt->uval == &configPreset) {
                switch (configPreset) {
                    case 0: // Classic
                        configNoFallDamage         = false;
                        configFailWarp             = false;
                        config45DegreeWallkicks    = false;
                        configExtraWallkickFrames  = false;
                        configFastSwimming         = false;
                        configSteepSlopeJumps      = false;
                        configNoLives              = false;
                        configAllowExitLevel       = false;
                        configFasterObjects        = false;
                        configNoActSpecificObjects = false;
                        break;
                    case 1: // QoL Only
                        configNoFallDamage         = false;
                        configFailWarp             = false;
                        config45DegreeWallkicks    = false;
                        configExtraWallkickFrames  = false;
                        configFastSwimming         = false;
                        configSteepSlopeJumps      = false;
                        configNoLives              = true;
                        configAllowExitLevel       = true;
                        configFasterObjects        = true;
                        configNoActSpecificObjects = true;
                        break;
                    case 2: // Modern
                        configNoFallDamage         = true;
                        configFailWarp             = true;
                        config45DegreeWallkicks    = true;
                        configExtraWallkickFrames  = true;
                        configFastSwimming         = true;
                        configSteepSlopeJumps      = true;
                        configNoLives              = true;
                        configAllowExitLevel       = true;
                        configFasterObjects        = true;
                        configNoActSpecificObjects = true;
                        break;
                }
            }
            break;

        case OPT_SCROLL:
            *opt->uval = wrap_add(*opt->uval, opt->scrStep * val, opt->scrMin, opt->scrMax);
            break;

        case OPT_SUBMENU:
            opt->nextMenu->prev = currentMenu;
            currentMenu = opt->nextMenu;
            break;

        case OPT_BUTTON:
            if (opt->actionFn)
                opt->actionFn(opt, val);
            break;

        default: break;
    }
}

//Options menu
void optmenu_draw(void) {
    u8 i;
    s16 scroll;
    s16 scrollpos;
    f32 sinpos;

    if (currentMenu->numOpts > 4) {
        optmenu_draw_box(272, 90, 280, 208, 0x80, 0x80, 0x80);
        scrollpos = 54 * ((f32)currentMenu->scroll / (currentMenu->numOpts-4));
        optmenu_draw_box(272, 90+scrollpos, 280, 154+scrollpos, 0xFF, 0xFF, 0xFF);
    }

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    optmenu_draw_text(160, 190, currentMenu->opts[currentMenu->select].explanation, 0);
    
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    for (i = 0; i < currentMenu->numOpts; i++) {
        scroll = 140 - 32 * i + currentMenu->scroll * 32;
        // FIXME: just start from the first visible option bruh
        if (scroll <= 140 && scroll > 32)
            optmenu_draw_opt(&currentMenu->opts[i], 160, scroll, (currentMenu->select == i));
    }

    sinpos = sins(optmenu_sin_timer*5000)*4;
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_generic_string(80 - sinpos, 132 - (32 * (currentMenu->select - currentMenu->scroll)), optSmallStr[2]);
    print_generic_string(224 + sinpos, 132 - (32 * (currentMenu->select - currentMenu->scroll)), optSmallStr[3]);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

//This has been separated for interesting reasons. Don't question it.
void optmenu_draw_prompt(void) {
    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    optmenu_draw_text(264, 212, optSmallStr[optmenu_open], 0);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

void optmenu_toggle(void) {
    if (optmenu_open == 0) {
        #ifndef nosound
        play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
        #endif

        currentMenu = &menuMain;
        optmenu_open = 1;
    } else {
        #ifndef nosound
        play_sound(SOUND_MENU_MARIO_CASTLE_WARP, gGlobalSoundSource);
        #endif
        optmenu_open = 0;
#ifndef TARGET_N64
        controller_reconfigure(); // rebind using new config values
        configfile_save(configfile_name());
#endif
    }
}

void optmenu_check_buttons(void) {
    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        optmenu_toggle();
    }
    
    optmenu_sin_timer++;

    if (!optmenu_open) return;

    u8 allowInput = 0;

    optmenu_option_timer--;
    if (optmenu_option_timer <= 0) {
        if (optmenu_hold_count == 0) {
            optmenu_hold_count++;
            optmenu_option_timer = 10;
        } else {
            optmenu_option_timer = 3;
        }
        allowInput = 1;
    }

    if (ABS(gPlayer1Controller->stickY) > 60) {
        if (allowInput) {
            #ifndef nosound
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
            #endif

            if (gPlayer1Controller->stickY >= 60) {
                currentMenu->select--;
                if (currentMenu->select < 0)
                    currentMenu->select = currentMenu->numOpts-1;
            } else {
                currentMenu->select++;
                if (currentMenu->select >= currentMenu->numOpts)
                    currentMenu->select = 0;
            }

            if (currentMenu->select < currentMenu->scroll)
                currentMenu->scroll = currentMenu->select;
            else if (currentMenu->select > currentMenu->scroll + 3)
                currentMenu->scroll = currentMenu->select - 3;
        }
    } else if (ABS(gPlayer1Controller->stickX) > 60) {
        if (allowInput) {
            #ifndef nosound
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
            #endif
            if (gPlayer1Controller->stickX >= 60)
                optmenu_opt_change(&currentMenu->opts[currentMenu->select], 1);
            else
                optmenu_opt_change(&currentMenu->opts[currentMenu->select], -1);
        }
    } else if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        if (allowInput) {
            #ifndef nosound
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
            #endif
            optmenu_opt_change(&currentMenu->opts[currentMenu->select], 0);
        }
    } else if (gPlayer1Controller->buttonPressed & B_BUTTON) {
        if (allowInput) {
            if (currentMenu->prev) {
                #ifndef nosound
                play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
                #endif
                currentMenu = currentMenu->prev;
            } else {
                // can't go back, exit the menu altogether
                optmenu_toggle();
            }
        }
    } else if (gPlayer1Controller->buttonPressed & START_BUTTON) {
        if (allowInput) optmenu_toggle();
    } else {
        optmenu_hold_count = 0;
        optmenu_option_timer = 0;
    }
}
