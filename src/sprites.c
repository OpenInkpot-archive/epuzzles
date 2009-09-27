#include <stdio.h>
#include <Evas.h>
#include <Edje.h>
#include <Eina.h>
#include "sprites.h"

struct sprites_t {
    int counter;
    Eina_List *sprites;
    Evas_Object *bg;
    Evas_Object *clip;
};

static void
_sprites_add(Evas_Object *obj) {
    sprites_t *sprites;
    sprites = calloc(1, sizeof(sprites_t));
    evas_object_smart_data_set(obj, sprites);
}

static void
_sprites_del(Evas_Object *obj) {
    sprites_t *sprites = evas_object_smart_data_get(obj);
    Evas_Object *sprite;
    if(sprites) {
        EINA_LIST_FREE(sprites->sprites, sprite)
            evas_object_del(sprite);
        if(sprites->bg)
            evas_object_del(sprites->bg);
        if(sprites->clip)
            evas_object_del(sprites->clip);
        free(sprites);
    }
}

static void
_sprites_init(Evas_Object *obj, Evas *evas, int w, int h) {

    sprites_t *sprites  = evas_object_smart_data_get(obj);
    if(!sprites) {
        printf("Can't initialize\n");
        return;
    }

    //sprites->bg = evas_object_image_add(evas);
    sprites->bg = evas_object_rectangle_add(evas);
    evas_object_smart_member_add(sprites->bg, obj);
    evas_object_name_set(sprites->bg, "sprites/bg");
    evas_object_move(sprites->bg, 0, 0);
    evas_object_resize(sprites->bg, w, h);
    //evas_object_image_size_set(sprites->bg, w, h);
    //evas_object_image_fill_set(sprites->bg, 0, 0, w, h);
    evas_object_color_set(sprites->bg, 255, 0, 0, 255);
    evas_object_show(sprites->bg);

    sprites->clip = evas_object_rectangle_add(evas);
    evas_object_color_set(sprites->clip, 255, 255, 255, 255);
    evas_object_name_set(sprites->clip, "sprites/clip");
    evas_object_clip_set(sprites->bg, sprites->clip);
    evas_object_move(sprites->clip, 0, 0);
    evas_object_resize(sprites->clip, w, h);
    evas_object_smart_member_add(sprites->clip, obj);

    sprites->sprites = NULL;
    printf("sprite obj done\n");
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
static void
_sprites_display(Evas_Object *obj, int x, int y, int w, int h) {
    sprites_t *drawable = evas_object_smart_data_get(obj);
    int iw, ih;
    evas_object_image_size_get(drawable->bg, &iw, &ih);
    evas_object_move(drawable->clip, x, y);
    evas_object_resize(drawable->clip, MIN(w, iw), MIN(h, ih));
}

static void
_sprites_show(Evas_Object *obj) {
    printf("sprite show\n");
    sprites_t *drawable = evas_object_smart_data_get(obj);
    evas_object_show(drawable->clip);
}

static void
_sprites_hide(Evas_Object *obj) {
    sprites_t *drawable = evas_object_smart_data_get(obj);
    evas_object_hide(drawable->clip);
}

static void
_sprites_move(Evas_Object *obj, Evas_Coord x, Evas_Coord y) {
    int ox, oy, ow, oh;
    sprites_t *sprites = evas_object_smart_data_get(obj);
    evas_object_geometry_get(sprites->clip, &ox, &oy, NULL, NULL);
    evas_object_move(sprites->clip, x, y);
    evas_object_move(sprites->bg, x, y);

    Eina_List* s;
    Evas_Object* sprite;
    EINA_LIST_FOREACH(sprites->sprites, s, sprite)
    {
        int sx, sy;
        evas_object_geometry_get(sprite, &sx, &sy, NULL, NULL);
        evas_object_move(sprite, x - ox + sx, y - oy + sy);
    }
    //_sprites_display(obj, x, y, ow, oh);
}


static void
_sprites_resize(Evas_Object *obj, Evas_Coord w, Evas_Coord h) {
    sprites_t *sprites = evas_object_smart_data_get(obj);
    evas_object_resize(sprites->clip, w, h);
    evas_object_resize(sprites->bg, w, h);
    //int ox, oy;
    //evas_object_geometry_get(obj, &ox, &oy, NULL, NULL);
    //_sprites_display(obj, ox, oy, w, h);
}

static void
_sprites_clip_set(Evas_Object *obj, Evas_Object *clip) {
    sprites_t *drawable = evas_object_smart_data_get(obj);
    evas_object_clip_set(drawable->clip, clip);
}

static void
_sprites_clip_unset(Evas_Object *obj) {
    sprites_t *drawable = evas_object_smart_data_get(obj);
    evas_object_clip_unset(drawable->clip);
}

static Evas_Smart * _sprites_smart_get() {
    static Evas_Smart * smart = NULL;
    static Evas_Smart_Class klass = {
        .name = "sprites",
        .version = EVAS_SMART_CLASS_VERSION,
        .add = _sprites_add,
        .del =  _sprites_del,
        .move = _sprites_move,
        .resize = _sprites_resize,
        .show = _sprites_show,
        .hide = _sprites_hide,
        .color_set = NULL,
        .clip_set = _sprites_clip_set,
        .clip_unset = _sprites_clip_unset,
        .calculate = NULL,
        .member_add = NULL,
        .member_del = NULL
    };

    if(!smart)
        smart = evas_smart_class_new(&klass);
    return smart;
}

Evas_Object *
sprites_add(Evas *evas, int w, int h) {
    Evas_Object *obj =  evas_object_smart_add(evas, _sprites_smart_get());
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(drawable)
        _sprites_init(obj, evas, w, h);
    return obj;
}

void
sprites_bg_file_set(Evas_Object *obj, const char *file, const char *key) {
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(!drawable)
        return;
    evas_object_image_file_set(drawable->bg, file, key);
}

static void
_move_sprite_relative(Evas_Object *obj, int x, int y)
{
    Evas_Object *clip = evas_object_clip_get(obj);
    if(!clip) {
        evas_object_move(obj, x, y);
        return;
    }
    int xo, yo;
    evas_object_geometry_get(clip, &xo, &yo, NULL, NULL);
    evas_object_move(obj, xo + x, yo + y);
    printf("Move relative %d %d %d %d\n", xo, x, yo, y);
}

int
sprites_add_sprite(Evas_Object *obj, const char *file, const char *key) {
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(!drawable) {
        printf("bad drawable\n");
        return -1;
    }
    Evas_Imaging_Image *im = evas_imaging_image_load(file, key);
    if(!im) {
        printf("Can't load %s\n", file);
        return -1;
    }
    int w, h;
    evas_imaging_image_size_get(im, &w, &h);
    Evas_Object *sprite = evas_object_image_add(evas_object_evas_get(obj));
    evas_object_image_file_set(sprite, file, key);
    evas_object_smart_member_add(sprite, obj);
    evas_object_image_size_set(sprite, w, h);
    evas_object_image_fill_set(sprite, 0, 0, w, h);
    evas_object_clip_set(sprite, drawable->clip);
    evas_object_stack_above(sprite, drawable->bg);
    _move_sprite_relative(sprite, 0, 0);
    evas_object_resize(sprite, w, h);
    evas_object_show(sprite);
    evas_imaging_image_free(im);
    drawable->sprites = eina_list_append(drawable->sprites, sprite);
    return drawable->counter++;
}

void
sprites_sprite_move(Evas_Object *obj, int index, int x, int y)
{
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(!drawable)
        return;
    Evas_Object *sprite = (Evas_Object *) eina_list_nth(drawable->sprites, index);
    if(sprite)
        _move_sprite_relative(sprite, x, y);
}

void
sprites_sprite_show(Evas_Object *obj, int index)
{
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(!drawable)
        return;
    Evas_Object *sprite = (Evas_Object *) eina_list_nth(drawable->sprites, index);
    if(sprite)
        evas_object_show(sprite);
}

void
sprites_sprite_hide(Evas_Object *obj, int index)
{
    sprites_t *drawable  = evas_object_smart_data_get(obj);
    if(!drawable)
        return;
    Evas_Object *sprite = (Evas_Object *) eina_list_nth(drawable->sprites, index);
    if(sprite)
        evas_object_hide(sprite);
}

