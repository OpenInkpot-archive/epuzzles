#ifndef _FRONTEND_H
#define _FRONTEND_H 1

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Ewl.h>
#include <edrawable.h>

#include "puzzles.h"
/*
 * This structure holds all the data relevant to a single window.
 * In principle this would allow us to open multiple independent
 * puzzle windows, although I can't currently see any real point in
 * doing so. I'm just coding cleanly because there's no
 * particularly good reason not to.
 */


struct frontend {
    Ewl_Window *window;
    Ewl_Drawable *area;
    Ewl_Statusbar *statusbar;
    Ewl_Widget *submenu;
    Ewl_Color_Set *colours;
    int ncolours;
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
    Ewl_Widget *cfgbox;
    void *paste_data;
    int paste_data_len;
    int pw, ph;                        /* pixmap size (w, h are area size) */
    int ox, oy;                        /* offset of pixmap in drawing area */
#ifdef OLD_FILESEL
    char *filesel_name;
#endif
    int npresets;
/*    GtkWidget **preset_bullets;
    GtkWidget *preset_custom_bullet;
    GtkWidget *copy_menu_item; */
};

void setup_gamelist(Ewl_Widget *);
void append_game(Ewl_Widget*, struct game *);

#endif
