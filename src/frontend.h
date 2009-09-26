#ifndef _FRONTEND_H
#define _FRONTEND_H 1

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <libkeys.h>
#include <edrawable.h>

#include "puzzles.h"
/*
 * This structure holds all the data relevant to a single window.
 * In principle this would allow us to open multiple independent
 * puzzle windows, although I can't currently see any real point in
 * doing so. I'm just coding cleanly because there's no
 * particularly good reason not to.
 */

struct _Color_Set {
    int r;
    int g;
    int b;
    int a;
};

struct frontend {
    Evas_Object *window;
    Evas_Object *area;
    struct _Color_Set *colours;
    int ncolours;
    int default_alpha;

    const char *name;
    keys_t *keys;

    int w, h;
    midend *me;
    int bbox_l, bbox_r, bbox_u, bbox_d;
    int timer_active;
    Ecore_Timer * timer_id;
    struct timeval last_time;
    struct font *fonts;
    int nfonts, fontsize;
    config_item *cfg;
    int cfg_which, cfgret;
    void *paste_data;
    int paste_data_len;
    int pw, ph;                        /* pixmap size (w, h are area size) */
    int ox, oy;                        /* offset of pixmap in drawing area */
    int npresets;
    int first_time;
};

struct game * lookup_game_by_name(const char * name);
void terminate(struct frontend *);
#endif
