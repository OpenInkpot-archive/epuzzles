/* Coordinate holder:  container with identification by x,y of object
   (coordinates is fake and used only in identification purposes */

#include <Eina.h>
#include <Evas.h>
#include "sprites.h"
#include "holder.h"
#include "puzzles.h"

struct object_holder {
    Evas_Object* canvas; /* drawable canvas */
    Eina_List* objects;
};

typedef struct _object _object;
struct _object {
    int x; /* fake x, y for identification */
    int y;
    int id; /* id inside canvas */
    bool used;
    const char *filename;
};

object_holder*
epuzzle_oh_new(Evas_Object* drawable)
{
    //printf("Create new holder\n");
    object_holder* oh = calloc(sizeof(object_holder), 1);
    oh->canvas = drawable;
    return oh;
}

static _object*
epuzzle_oh_find_object(object_holder *oh, int fx, int fy,
                       const char * filename)
{
    Eina_List* tmp;
    _object* each;
    if(!oh->objects)
        return NULL;
    EINA_LIST_FOREACH(oh->objects, tmp, each)
    {
        if(each->x == fx && each->y == fy && each->used &&
            !strcmp(each->filename, filename))
        {
            //printf("match %s %d %d %d\n", each->filename, fx, fy, each->used);
            return each;
        }
    }
    return NULL;
}

static _object *
epuzzle_oh_find_unused(object_holder *oh, const char *filename)
{
    Eina_List* tmp;
    _object* each;
    if(!oh->objects)
        return NULL;
    EINA_LIST_FOREACH(oh->objects, tmp, each)
    {
        if(!each->used && !strcmp(each->filename, filename))
            return each;
    }
    return NULL;
}

static _object*
epuzzle_oh_insert_object(object_holder* oh, const char* filename,
                        int fx, int fy)
{
    _object* object = calloc(sizeof(_object), 1);
    object->x = fx;
    object->y = fy;
    object->used = true;
    object->id = sprites_add_sprite(oh->canvas, filename, NULL);
    object->filename = eina_stringshare_add(filename);
    oh->objects = eina_list_append(oh->objects, object);
    return object;
}

static void
epuzzle_oh_move(Evas_Object *canvas, _object *obj, int fx, int fy, int align)
{
    int w, h;
    int rx = fx;
    int ry = fy;
    obj->x = fx;
    obj->y = fy;
    sprites_sprite_size_get(canvas, obj->id, &w, &h);
    if(align & ALIGN_VCENTRE)
        ry += h / 2;
    if(align & ALIGN_HCENTRE)
        rx += w / 2;
    sprites_sprite_move(canvas, obj->id, rx, ry);
    sprites_sprite_raise(canvas, obj->id);
}

void
epuzzle_oh_put_object(object_holder* oh, int fx, int fy,
                      const char* filename, int align)
{
    int h, w;
    _object* object = epuzzle_oh_find_object(oh, fx, fy, filename);
    if(object)
    {
        //printf("Already has %s at %d %d\n", filename, fx, fy);
        sprites_sprite_raise(oh->canvas, object->id);
        return;
    }
    object = epuzzle_oh_find_unused(oh, filename);
    if(object)
    {
            sprites_sprite_show(oh->canvas, object->id);
            object->used = true;
            //printf("reuse recycled object %s at %d %d\n", filename, fx, fy);
    }
    else
    {
            //printf("Insert new object %s at %d %d\n", filename, fx, fy);
            object = epuzzle_oh_insert_object(oh, filename, fx, fy);
    }
    epuzzle_oh_move(oh->canvas, object, fx, fy, align);
}

#if 0
static void
_dump(object_holder* oh)
{
    Eina_List* tmp;
    _object* object;
    EINA_LIST_FOREACH(oh->objects, tmp, object)
    {
        if(object->used)
            printf("still alive: %d %d \n", object->x, object->y);
    }
}
#endif

void
epuzzle_oh_drop_all(object_holder *oh)
{
    Eina_List* tmp;
    _object* object;
    EINA_LIST_FOREACH(oh->objects, tmp, object)
    {
        object->used = false;
        sprites_sprite_hide(oh->canvas, object->id);
    }
}

void
epuzzle_oh_drop_object(object_holder *oh, int fx, int fy)
{
    while(1)
    {
        _object* object = epuzzle_oh_find_object(oh, fx, fy, NULL);
        if(!object)
        {
    //        printf("No more objects at  %d %d\n", fx, fy);
    //        _dump(oh);
            return;
        }
      //  printf("really hide %d %d\n", fx, fy);
        sprites_sprite_hide(oh->canvas, object->id);
        object->used = false;
    }
}

void
epuzzle_oh_del(object_holder* oh)
{
    Eina_List* tmp;
    _object* object;
    EINA_LIST_FOREACH(oh->objects, tmp, object)
    {
        eina_stringshare_del(object->filename);
        free(object);
    }
    eina_list_free(oh->objects);
    free(oh);
}
