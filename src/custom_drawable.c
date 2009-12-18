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


typedef struct pegs_private pegs_private;
struct pegs_private {
    int cursor;
    int pegs[7*7];
};

void
custom_drawable_pegs_show(drawing* dr, int fx, int fy, int x, int y)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    pegs_private* private = evas_object_data_get(fe->area, "pegs-private");
    sprites_sprite_move(fe->area, private->pegs[7*fy+fx], x, y);
    sprites_sprite_show(fe->area, private->pegs[7*fy+fx]);
}

void
custom_drawable_pegs_hide(drawing* dr, int fx, int fy)
{
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    pegs_private* private = evas_object_data_get(fe->area, "pegs-private");
    sprites_sprite_hide(fe->area, private->pegs[7*fy+fx]);
}

void
custom_drawable_pegs_cursor_move(drawing* dr, int x, int y)
{
    int w, h;
    struct frontend * fe = (struct frontend *) drawing_handle(dr);
    pegs_private* private = evas_object_data_get(fe->area, "pegs-private");
    sprites_sprite_move(fe->area, private->cursor, x, y);
}


void
_private_cleanup(void *data, Evas* e, Evas_Object *obj, void *event_info)
{
    pegs_private* private = (pegs_private*) data;
    free(private);
}

Evas_Object *
custom_drawable_pegs_create(Evas *evas, int xy, int n)
{
    Evas_Object *sprites = sprites_add(evas, xy, xy);
    if(!sprites)
        err(1, "Can't alloc sprites\n");
    sprites_bg_file_set(sprites, PEGS "bg.png", NULL);
    pegs_private* private = calloc(1, sizeof(pegs_private));
    private->cursor = sprites_add_sprite(sprites, PEGS "cursor.png", NULL);
    evas_object_data_set(sprites, "pegs-private", private);
    evas_object_event_callback_add(sprites, EVAS_CALLBACK_DEL, _private_cleanup,
        private);
    int x, y;
    for(x=0; x < 7; x++)
        for(y=0; y< 7; y++)
        {
            private->pegs[7*y+x] = sprites_add_sprite(sprites, PEGS "peg.png", NULL);
            sprites_sprite_hide(sprites, private->pegs[7*y+x]);
        }
    return sprites;
}
