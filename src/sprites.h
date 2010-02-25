#ifndef _SPRITES_H
#define _SPRITES_H 1

typedef struct sprites_t sprites_t;

Evas_Object *
sprites_add(Evas *evas, int w, int h);

void
sprites_bg_file_set(Evas_Object *obj, const char *file, const char *key);


void
sprites_bg_object_set(Evas_Object *obj, Evas_Object *bg);

int
sprites_add_sprite(Evas_Object *obj, const char *file, const char *key);

void
sprites_sprite_move(Evas_Object *obj, int index, int x, int y);

void
sprites_sprite_show(Evas_Object *obj, int index);

void
sprites_sprite_hide(Evas_Object *obj, int index);

void
sprites_sprite_size_get(Evas_Object *obj, int index, int *w, int *h);

void
sprites_stack_external_object_lower(Evas_Object *obj, Evas_Object *another);

void
sprites_stack_external_object_upper(Evas_Object *obj, Evas_Object *another);

void
sprites_del_externals(Evas_Object *obj);


void
sprites_sprite_raise(Evas_Object *obj, int n);
#endif
