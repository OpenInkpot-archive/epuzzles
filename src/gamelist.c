#define _GNU_SOURCE 1
#include <err.h>
#include <libintl.h>
#include <Evas.h>
#include "puzzles.h"
#include "frontend.h"
#include "custom_drawable.h"

#define _(x) x

extern struct drawing_api e_drawing_api;
extern struct drawing_api fake_drawing_api;

extern game fifteen;
extern game bridges;
extern game twiddle;
extern game flip;
extern game pegs;
extern game unequal;
extern game solo;

void
epuzzle_run_help(Evas_Object* gui, const gamelist_t* item)
{
    epuzzles_help(gui, item->name);
}

const char  stdhelp[] = _("Press \"0\" to get help");
const char  solohelp[] = _("Press \"M\" to get help");

static gamelist_t _gamelist[] = {
    { "fifteen", &fifteen, stdhelp, epuzzle_run_help, custom_drawable_fifteen,
       &fake_drawing_api, 0 },
    { "flip", &flip, stdhelp, &epuzzle_run_help,
        &custom_drawable_edrawable,  &e_drawing_api, 1},
    { "twiddle", &twiddle, solohelp, epuzzle_run_help , &custom_drawable_twiddle,
       &fake_drawing_api, 0 },
    { "pegs", &pegs, stdhelp, epuzzle_run_help ,
        &custom_drawable_edrawable,  &e_drawing_api, 1},
    { "bridges", &bridges, stdhelp, epuzzle_run_help ,
        &custom_drawable_edrawable,  &e_drawing_api, 1},
    { "unequal", &unequal, stdhelp, epuzzle_run_help ,
        &custom_drawable_edrawable,  &e_drawing_api, 1},
    { "solo", &solo, solohelp, epuzzle_run_help ,
        &custom_drawable_edrawable,  &e_drawing_api, 1},
    { NULL, NULL, NULL, NULL, NULL, NULL, 1 }
};


static gamelist_t *
_lookup(const char *name)
{
    gamelist_t *each;
    each = _gamelist;
    while(each->name)
    {
        if(!strcasecmp(each->name, name))
                return each;
        ++each;
    }
    return NULL;
}

struct game *
lookup_game_by_name(const char * name)
{
    gamelist_t* item = _lookup(name);
    if(item)
        return item->puzzle;
    return NULL;
}

void epuzzles_ctxmenu_by_name(Evas_Object *obj, const char* name)
{
    gamelist_t* item = _lookup(name);
    if(item)
        item->ctxmenu(obj, item);
}

const char* epuzzles_hint_by_name(const char* name)
{
    gamelist_t* item = _lookup(name);
    if(item && item->hint)
        return gettext(item->hint);
    return "";
};

void epuzzle_create_canvas(struct frontend* fe, Evas* evas, int xy)
{
    gamelist_t* item = _lookup(fe->name);
    if(item)
    {
        fe->area = item->create(evas, xy);
        fe->draw_api = item->draw_api;
        fe->support_colors = item->support_colors;
    }
    else
        err(1, "Wrong puzzle name");
}
