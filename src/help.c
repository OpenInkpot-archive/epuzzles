#include <libintl.h>
#include <Evas.h>
#include <Edje.h>

#include <libchoicebox.h>
#include <libeoi.h>
#include <libeoi_help.h>

#include "frontend.h"

void epuzzles_help(struct frontend* fe, const char* topic)
{

    Evas* evas = evas_object_evas_get(fe->area);
    eoi_help_show(evas,
                    "epuzzles",
                    topic,
                    gettext("Puzzles: Help"),
                    fe->keys,
                    fe->name);
}
