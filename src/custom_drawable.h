#ifndef _CUSTOM_DRAWABLE_H
#define _CUSTOM_DRAWABLE_H 1

#ifndef HIDE_EVAS
#include <Evas.h>
#endif

#include "puzzles.h"
#include "holder.h"

#ifndef HIDE_EVAS
Evas_Object *
custom_drawable_edrawable(Evas *, int);

Evas_Object *
custom_drawable_fifteen(Evas *, int);

Evas_Object *
custom_drawable_twiddle(Evas *, int);

#endif

void
custom_drawable_fifteen_move(drawing *dr, int tile, int x, int y);

void custom_drawable_hide_unneeded(drawing *dr, int tile);



void
custom_drawable_pegs_show(object_holder* oh, int fx, int fy);
void
custom_drawable_pegs_hide(object_holder* oh, int fx, int fy);
void
custom_drawable_pegs_cursor_move(drawing* dr, int cursor, int x, int y);
int
custom_drawable_pegs_cursor_create(drawing* dr);
void
custom_drawable_object_holder_destroy(object_holder* oh);
object_holder*
custom_drawable_object_holder_create(drawing* dr);
Evas_Object *
custom_drawable_pegs_create(Evas *evas, int xy, int n);
#endif
