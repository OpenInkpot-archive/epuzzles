#define _GNU_SOIRCE 1
#include <err.h>
#include <Evas.h>
#include "custom_drawable.h"
#include "frontend.h"
#include "sprites.h"

#define FIFTEEN "/usr/share/epuzzles/sprites/fifteen/"
#define TWIDDLE "/usr/share/epuzzles/sprites/twiddle/"
#define PEGS "/usr/share/epuzzles/sprites/pegs/"

Evas_Object *
custom_drawable_edrawable(Evas* evas, int xy)
{
    return edrawable_add(evas, xy, xy);
}

static Evas_Object *
custom_drawable_common(Evas *evas, int xy, int n)
{
    Evas_Object *sprites = sprites_add(evas, xy, xy);
    if(!sprites)
        err(1, "Can't alloc sprites\n");
    sprites_bg_file_set(sprites, FIFTEEN "bg.png", NULL);
    int i;
    for(i=1; i <= n; i++)
    {
        char buf[1024];
        snprintf(buf, 1024, FIFTEEN "%02d.png", i);
        printf("Loading %s\n", buf);
        sprites_add_sprite(sprites, buf, NULL);
    };
    return sprites;
}

Evas_Object *
custom_drawable_fifteen(Evas *evas, int xy)
{
    return custom_drawable_common(evas, xy, 15);
}

void
custom_drawable_fifteen_move(drawing *dr, int tile, int x, int y)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    sprites_sprite_move(fe->area, tile, x, y);
}

void
custom_drawable_hide_unneeded(drawing *dr, int tile)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    sprites_sprite_hide(fe->area, tile);
}

Evas_Object *
custom_drawable_twiddle(Evas *evas, int xy)
{
    Evas_Object* sprites =  custom_drawable_common(evas, xy, 16);
    sprites_add_sprite(sprites, TWIDDLE "frame_x4.png", NULL);
    sprites_add_sprite(sprites, TWIDDLE "frame_x9.png", NULL);
    return sprites;
}

void
custom_drawable_pegs_show(object_holder* oh, int fx, int fy)
{
    epuzzle_oh_put_object(oh, fx, fy, fx, fy, PEGS "peg.png", 0);
}

void
custom_drawable_pegs_hide(object_holder* oh, int fx, int fy)
{
    epuzzle_oh_drop_object(oh, fx, fy);
}

void
custom_drawable_pegs_cursor_move(drawing* dr, int cursor, int x, int y)
{
    int w, h;
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    sprites_sprite_size_get(fe->area, cursor, &w, &h);
    x-= w / 2;
    h-= h / 2;
    sprites_sprite_move(fe->area, cursor, x, y);
}

int
custom_drawable_pegs_cursor_create(drawing* dr)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    return sprites_add_sprite(fe->area, PEGS "cursor.png", NULL);
}

object_holder*
custom_drawable_object_holder_create(drawing* dr)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    return epuzzle_oh_new(fe->area);
}

void
custom_drawable_object_holder_destroy(object_holder* oh)
{
    epuzzle_oh_del(oh);
}

Evas_Object *
custom_drawable_pegs_create(Evas *evas, int xy, int n)
{
    Evas_Object *sprites = sprites_add(evas, xy, xy);
    if(!sprites)
        err(1, "Can't alloc sprites\n");
    sprites_bg_file_set(sprites, PEGS "bg.png", NULL);
    return sprites;
}
