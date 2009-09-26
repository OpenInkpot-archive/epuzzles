#ifndef _CUSTOM_DRAWABLE_H
#define _CUSTOM_DRAWABLE_H 1

#ifndef HIDE_EVAS
#include <Evas.h>
#endif

#include "puzzles.h"

#ifndef HIDE_EVAS
Evas_Object *
custom_drawable_fifteen(Evas *, int);

#endif

void
custom_drawable_fifteen_move(drawing *dr, int tile, int x, int y);

#endif
