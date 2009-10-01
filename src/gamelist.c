#include <libintl.h>
#include <Evas.h>
#include "puzzles.h"
#include "frontend.h"

#define _(x) x

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

static const gamelist_t _gamelist[] = {
    { "fifteen", &fifteen, stdhelp, epuzzle_run_help },
    { "flip", &flip, stdhelp, epuzzle_run_help },
    { "twiddle", &twiddle, solohelp, epuzzle_run_help },
    { "pegs", &pegs, stdhelp, epuzzle_run_help },
    { "bridges", &bridges, stdhelp, epuzzle_run_help },
    { "unequal", &unequal, stdhelp, epuzzle_run_help },
    { "solo", &solo, solohelp, epuzzle_run_help },
    { NULL, NULL, NULL, NULL }
};


static gamelist_t *
_lookup(const char *name)
{
    struct gamelist_t *each;
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
    struct gamelist_t* item = _lookup(name);
    if(item)
        return item->puzzle;
    return NULL;
}

void epuzzles_ctxmenu_by_name(Evas_Object *obj, const char* name)
{
    struct gamelist_t* item = _lookup(name);
    if(item)
        item->ctxmenu(obj, item);
}

const char* epuzzles_hint_by_name(const char* name)
{
    struct gamelist_t* item = _lookup(name);
    if(item && item->hint)
        return item->hint;
    return "";
};
