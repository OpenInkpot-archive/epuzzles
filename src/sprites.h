#ifndef _SPRITES_H
#define _SPRITES_H 1

typedef struct sprites_t sprites_t;

Evas_Object *
sprites_add(Evas *evas, int w, int h);

void
sprites_bg_file_set(Evas_Object *obj, const char *file, const char *key);

int
sprites_add_sprite(Evas_Object *obj, const char *file, const char *key);

void
sprites_sprite_move(Evas_Object *obj, int index, int x, int y);

void
sprites_sprite_show(Evas_Object *obj, int index);

void
sprites_sprite_hide(Evas_Object *obj, int index);

#endif
